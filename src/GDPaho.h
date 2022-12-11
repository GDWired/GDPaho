#ifndef GDPAHO
#define GDPAHO

#include <Godot.hpp>
#include <Object.hpp>
#include <string.h>

#include <mqtt/async_client.h>

#include "utils.h"

using namespace godot;
using namespace std;

class PahoWrapper;

namespace godot {

	/**
	 * First iteration a lot of functionality is needed (like password, TLS etc.)
	 */
	class GDPaho : public Object {
		GODOT_CLASS(GDPaho, Object)

		// Private attributs
		private:
			/**
			 * The paho wrapper
			 */
			PahoWrapper* m_wrapper;

			/**
			 * If true, the connection parameter has been set at least once
			 */
			bool m_connection_initialized_once;

		// Private methods
		private:

			//###############################################################
			//	Internals
			//###############################################################


			//###############################################################
			//	Wrapped methods
			//###############################################################

			/**
			 * Create a new paho client instance
			 * @param p_id string to use as the client id. If NULL, a random client id will be generated. If id is NULL, clean_session must be true
			 * @param p_host the hostname or ip address of the broker to connect to
			 * @param p_port the network port to connect to. Usually 1883
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
		 	 */
			int initialise(const String p_id, const String p_host, const String p_port);

			/**
			 * Return true if connected
			 * @return true if connected
		 	 */
			bool is_connected_to_broker();

			/**
 			 * Connect to an MQTT broker
			 * @param p_clean_session set to true to instruct the broker to clean all messages and subscriptions on disconnect, false to instruct it to keep them.
			 * @param p_keepalive the number of seconds after which the broker should send a PING message to the client if no other messages have been exchanged in that time
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int connect(const bool p_clean_session, const int p_keep_alive = 60);

			/**
			 * Reconnect to a broker
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int reconnect();

			/**
			 * Disconnect from the broker
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int disconnect();

			/**
			 * Publish a message on a given topic (Message ID emit message_id_updated)
			 * @param p_topic the topic on which to publish the will
			 * @param p_payload the data to send, can be empty
			 * @param p_qos integer value 0, 1 or 2 indicating the Quality of Service to be used
			 * @param p_retain set to true to make the will a retained message
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int publish(const String p_topic, const String p_payload = "", const int p_qos = 0, const bool p_retain = false);

			/**
			 * Subscribe to a topic (Message ID emit message_id_updated)
			 * @param p_sub the subscription pattern
			 * @param p_qos integer value 0, 1 or 2 indicating the Quality of Service to be used
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int subscribe(const String p_sub, const int p_qos = 0);

			/**
			 * Unsubscribe from a topic (Message ID emit message_id_updated)
			 * @param p_sub the unsubscription pattern
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int unsubscribe(const String p_sub);

			/**
			 * Blocking unsubscribe from a topic (Message ID emit message_id_updated)
			 * @param p_sub the unsubscription pattern
			 * @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			int unsubscribe_wait(const String p_sub);

			/**
			 * Get string reason code
			 * @param p_rc the reason code
			 * @return the string reason code
			 */
			String reason_code_string(const int p_rc);

		// Public methods
		public:

			//###############################################################
			//	Constructor
			//###############################################################

			/**
			 * Constructor
			 */
			GDPaho();

			/**
			 * Destructor
			 */
			~GDPaho();

			//###############################################################
			//	Godot methods
			//###############################################################

			/**
			 * Bind methods, signals etc.
			 */
			static void _register_methods();

			/**
			 * Init the class
			 */
			void _init();

			//###############################################################
			//	Callbacks
			//###############################################################


			//###############################################################
			//	Emit handlers
			//###############################################################

			/**
			 * Emit on_connect
			 * @param p_rc the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			void emit_on_connect(const int p_rc);

			/**
			 * Emit on_disconnect
			 */
			void emit_on_disconnect();

			/**
			 * Emit on_publish
			 * @param p_mid the message id of the subscribe message
			 */
			void emit_on_publish(const int p_mid);

			/**
			 * Emit on_message
			 * @param p_message the received message
			 */
			void emit_on_message(const mqtt::const_message_ptr p_message);
			
			/**
			 * Emit on_subscribe
			 * @param p_mid the message id of the subscribe message
			 * @param p_topic the topic
			 */
			void emit_on_subscribe(const int p_mid, const std::string& p_topic);

			/**
			 * Emit on_unsubscribe
			 * @param p_mid the message id of the subscribe message
			 * @param p_topic the topic
			 */
			void emit_on_unsubscribe(const int p_mid, const std::string& p_topic);

			/**
			 * Emit on_log
			 * @param p_level the log message level from the values: PAHO_LOG_INFO PAHO_LOG_NOTICE PAHO_LOG_WARNING PAHO_LOG_ERR PAHO_LOG_DEBUG
			 * @param p_message the message string
			 */
			void emit_on_log(const int p_level, const std::string& p_message);

			/**
			 * Emit on_error
			 * @param p_message the message string
			 * @param p_rc the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
			 */
			void emit_on_error(const std::string& p_message, const int p_rc);
	};
}


#endif // GDPAHO
