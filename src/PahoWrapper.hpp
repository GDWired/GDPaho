#ifndef PAHOWRAPPER
#define PAHOWRAPPER

#include <mqtt/async_client.h>

#include "utils.h"
#include "GDPaho.h"

/**
 * Callbacks for the success or failures of requested actions.
 * This could be used to initiate further action
 */
class ActionListener : public virtual mqtt::iaction_listener {
	protected:

		/**
		 * Reference to the main class of the plugin
		 */
		GDPaho& m_gd_paho;

	public:

		/**
		 * Constructor
		 * @param p_name the name of the callback
		 */
		ActionListener(GDPaho& p_gd_paho) : 
			m_gd_paho(p_gd_paho) {}

	protected:

		/**
		 * This method is invoked when an action fails.
		 * @param p_token the token
		 */
		void on_failure(const mqtt::token& p_token) override {
			m_gd_paho.emit_on_error("Connection attempt failed", p_token.get_reason_code());
		}

		/**
		 * This method is invoked when an action has completed successfully.
		 * @param p_token the token
		 */
		void on_success(const mqtt::token& p_token) override {
			m_gd_paho.emit_on_subscribe(p_token.get_message_id(), (*p_token.get_topics())[0]);
		}
};

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class Callback : public virtual mqtt::callback, public virtual mqtt::iaction_listener {
	protected:

		/**
		 * Retry count
		 */
		int m_retry_count;

		/**
		 * Retry attemps
		 */
		int m_retry_attempts;

		/**
		 * Reference to the main class of the plugin
		 */
		GDPaho& m_gd_paho;

		/**
		 * The client
		 */
		mqtt::async_client& m_client;

		/**
		 * Options to use if we need to reconnect
		 */
		mqtt::connect_options& m_connecton_options;
	
	public:

		/**
		 * Constructor
		 * @param p_client the client
		 * @param p_retry_attempts retry attemps
		 * @param p_connecton_options options to use if we need to reconnect
		 */
		Callback(GDPaho& p_gd_paho, mqtt::async_client& p_client, const int p_retry_attempts, mqtt::connect_options& p_connecton_options) : 
			m_gd_paho(p_gd_paho),
			m_retry_count(0),
			m_retry_attempts(p_retry_attempts),
			m_client(p_client), 
			m_connecton_options(p_connecton_options) {}

		/**
		 * Reconnecting to the broker by calling connect() again. 
		 * 
		 * This is a possibility for an application that keeps
		 * a copy of it's original connect_options, or if the app wants to
		 * reconnect with different options.
		 * Another way this can be done manually, if using the same options, is
		 * to just call the async_client::reconnect() method.
		 */
		void reconnect() {
			std::this_thread::sleep_for(std::chrono::milliseconds(2500));
			try {
				m_client.connect(m_connecton_options, nullptr, *this);
			} catch (const mqtt::exception& p_exception) {
				m_gd_paho.emit_on_error(p_exception.what(), p_exception.get_reason_code());
			}
		}

		/**
		 * Re-connection failure
		 * @param p_token the token
		 */
		void on_failure(const mqtt::token& p_token) override {
			m_gd_paho.emit_on_error("Connection attempt failed", p_token.get_reason_code());
			if (++m_retry_count < m_retry_attempts) {
				reconnect();
			}
		}

		/**
		 * (Re)connection success
		 * Either this or connected() can be used for callbacks.
		 * @param p_token the token
		 */
		void on_success(const mqtt::token& p_token) override {
			// Nothing ...
		}

		/**
		 * (Re)connection success
		 * @param p_cause the cause
		 */		
		void connected(const std::string& p_cause) override {
			m_gd_paho.emit_on_log(PahoLogLevel::PAHO_LOG_INFO, p_cause);
			m_gd_paho.emit_on_connect(mqtt::ReasonCode::SUCCESS);
		}

		/**
		 * Callback for when the connection is lost.
		 * This will initiate the attempt to manually reconnect.
		 * @param p_cause the cause
		 */	
		void connection_lost(const std::string& p_cause) override {
			m_gd_paho.emit_on_log(PahoLogLevel::PAHO_LOG_ERR, p_cause);
			m_gd_paho.emit_on_disconnect();
			reconnect();
		}

		/**
		 * Callback for when a message arrives.
		 * @param p_message the message
		 */	
		void message_arrived(mqtt::const_message_ptr p_message) override {
			m_gd_paho.emit_on_message(p_message);
		}

		/**
		 * Called when delivery for a message has been completed, and all
		 * acknowledgments have been received.
		 * @param p_token the token
		 */	
		void delivery_complete(mqtt::delivery_token_ptr p_token) override {
			m_gd_paho.emit_on_publish((p_token ? p_token->get_message_id() : -1));
		}
};

/**
 * The MQTTClient wrapper
 */
class PahoWrapper : public mqtt::async_client {
		protected:

			/**
			 * Reference to the main class of the plugin
			 */
			GDPaho& m_gd_paho;

			/**
			 * The server address.
			 */
			std::string m_server_address;

			/**
			 * An action listener to display the result of actions.
			 */
			ActionListener m_subscription_listener;

			/**
			 * Connection options.
			 */
			mqtt::connect_options m_connection_options;

			/**
			 * Callback.
			 */
			Callback* m_callback;

		public:

			/**
			 * Constructor
			 * @param p_gd_paho main class of the plugin
			 * @param p_id the client ID
			 * @param p_host the hostname or ip address of the broker to connect to
			 */
			PahoWrapper (GDPaho& p_gd_paho, const std::string& p_id, const std::string& p_host = "tcp://localhost:1883") :
				mqtt::async_client(p_host, p_id),
				m_server_address(p_host),
				m_gd_paho(p_gd_paho),
				m_subscription_listener(p_gd_paho),
				m_connection_options(),
				m_callback(nullptr) {
			}

			/**
			 * Destructor
			 */
			virtual ~PahoWrapper() {
				delete m_callback;
			}
			
			/**
			 * Connect to the broker
			 * @param p_clean_session set to true to instruct the broker to clean all messages and subscriptions on disconnect, false to instruct it to keep them
			 * @param p_keep_alive keep alive time
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int connect_to_broker(const bool p_clean_session = true, const int p_keep_alive = 60) {
				m_connection_options.set_clean_session(p_clean_session);
				m_connection_options.set_keep_alive_interval(p_keep_alive);
				m_callback = new Callback(m_gd_paho, *this, 5, m_connection_options);

				set_callback(*m_callback);

				try {
					const auto& l_token = connect(m_connection_options, nullptr, *m_callback);
					return l_token->get_reason_code();
				} catch (const mqtt::exception& p_exception) {
					return p_exception.get_reason_code();
				}
			}

			/**
			 * Reconnect to the broker
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int reconnect_to_broker() {
				const auto& l_token = reconnect();
				return l_token->get_reason_code();
			}

			/**
			 * Connect to the broker
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int disconnect_to_broker() {
				const auto& l_token = disconnect();
				return l_token->get_reason_code();
			}

			/**
			 * Connect to the broker
			 * @param p_topic the name of the topic
			 * @param p_qos the QoS used
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int subscribe_to(const std::string& p_topic, const int p_qos = 1) {
				const auto& l_token = subscribe(p_topic, p_qos, nullptr, m_subscription_listener);
				return l_token->get_reason_code();
			}

			/**
			 * Connect to the broker
			 * @param p_topic the name of the topic
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int unsubscribe_to(const std::string& p_topic) {
				const auto& l_token = unsubscribe(p_topic);
				if (!l_token->get_reason_code()) {
					m_gd_paho.emit_on_unsubscribe(l_token->get_message_id(), p_topic);
				}
				return l_token->get_reason_code();
			}

			/**
			 * Connect to the broker
			 * @param p_topic the name of the topic
			 * @param p_data data to send
			 * @param p_qos the QoS used
			 * @param p_retain if true the data is retained
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int publish_to(const std::string& p_topic, const std::string& p_data, const int p_qos = 1, const bool p_retain = false)  {
				const auto& l_token = publish(p_topic, p_data, p_qos, p_retain);
				return l_token->get_reason_code();
			}

};

#endif //PAHOWRAPPER
