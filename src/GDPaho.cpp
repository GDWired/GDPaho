#include "GDPaho.h"

#include <Godot.hpp>
#include "PahoWrapper.hpp"

using namespace godot;

GDPaho::GDPaho() : 
	m_wrapper(nullptr),
	m_connection_initialized_once(false) {
}

GDPaho::~GDPaho() {
	if (m_wrapper) {
		if (m_wrapper->is_connected()) {
			m_wrapper->disconnect()->wait();
		}
		delete m_wrapper;
	}
}

//###############################################################
//	Godot methods
//###############################################################

void GDPaho::_register_methods() {
	// Methods
	register_method("initialise", &GDPaho::initialise);
	register_method("is_connected_to_broker", &GDPaho::is_connected_to_broker);
	register_method("broker_connect", &GDPaho::connect);
	register_method("broker_reconnect", &GDPaho::reconnect);
	register_method("broker_disconnect", &GDPaho::disconnect);
	register_method("publish", &GDPaho::publish);
	register_method("subscribe", &GDPaho::subscribe);
	register_method("unsubscribe", &GDPaho::unsubscribe);
	register_method("reason_code_string", &GDPaho::reason_code_string);

	// Signals
	register_signal<GDPaho>("connected", 
		"reason_code", GODOT_VARIANT_TYPE_INT
	);
	register_signal<GDPaho>("disconnected");
	register_signal<GDPaho>("published", 
		"message_id", GODOT_VARIANT_TYPE_INT
	);
	register_signal<GDPaho>("received", 
		"topic", GODOT_VARIANT_TYPE_STRING,
		"payload", GODOT_VARIANT_TYPE_STRING
	);
	register_signal<GDPaho>("subscribed", 
		"message_id", GODOT_VARIANT_TYPE_INT,
		"topic", GODOT_VARIANT_TYPE_STRING
	);
	register_signal<GDPaho>("unsubscribed", 
		"message_id", GODOT_VARIANT_TYPE_INT,
		"topic", GODOT_VARIANT_TYPE_STRING
	);
	register_signal<GDPaho>("log", 
		"level", GODOT_VARIANT_TYPE_INT,
		"message", GODOT_VARIANT_TYPE_STRING
	);
	register_signal<GDPaho>("error", 
		"message", GODOT_VARIANT_TYPE_STRING,
		"reason_code", GODOT_VARIANT_TYPE_INT
	);
}

void GDPaho::_init() {
}

//###############################################################
//	Internals
//###############################################################


//###############################################################
//	Wrapped methods
//###############################################################

int GDPaho::initialise(const String p_id, const String p_host, const String p_port) {
	try {
		String m_connection_address = "tcp://" + p_host + ":" + p_port;
		m_wrapper = new PahoWrapper(*this, p_id.utf8().get_data(), m_connection_address.utf8().get_data());
		return mqtt::ReasonCode::SUCCESS;
	} catch (std::exception& p_exception) {
		return atoi(p_exception.what());
	}
}

bool GDPaho::is_connected_to_broker() {
	if (m_wrapper) {
		return m_wrapper->is_connected();
	}
	return false;
}

int GDPaho::connect(const bool p_clean_session, const int p_keep_alive) {
	if (!m_wrapper) {
		return PAHO_ERR_NOT_INIT;
	}
	if (m_wrapper->is_connected()) {
		return PAHO_ERR_ALREADY_CONNECTED;
	}
	m_connection_initialized_once = true;
	return m_wrapper->connect_to_broker(p_clean_session, p_keep_alive);
}

int GDPaho::reconnect() {
	if (!m_wrapper) {
		return PAHO_ERR_NOT_INIT;
	}
	if (m_wrapper->is_connected()) {
		return PAHO_ERR_ALREADY_CONNECTED;
	}
	if (!m_connection_initialized_once) {
		return PAHO_ERR_CONNECTION_NOT_INITIALIZED_ONCE;
	}
	return m_wrapper->reconnect_to_broker();
}

int GDPaho::disconnect() {
	if (!m_wrapper) {
		return PAHO_ERR_NOT_INIT;
	}
	if (!m_wrapper->is_connected()) {
		return PAHO_ERR_NOT_CONNECTED;
	}
	return m_wrapper->disconnect_to_broker();
}	

int GDPaho::publish(const String p_topic, const String p_payload, const int p_qos, const bool p_retain) {
	if (!m_wrapper) {
		return PAHO_ERR_NOT_INIT;
	}
	if (!m_wrapper->is_connected()) {
		return PAHO_ERR_NOT_CONNECTED;
	}
	return m_wrapper->publish_to(p_topic.utf8().get_data(), p_payload.utf8().get_data(), p_qos, p_retain);
}	

int GDPaho::subscribe(const String p_sub, const int p_qos) {
	if (!m_wrapper) {
		return PAHO_ERR_NOT_INIT;
	}
	if (!m_wrapper->is_connected()) {
		return PAHO_ERR_NOT_CONNECTED;
	}
	return m_wrapper->subscribe_to(p_sub.utf8().get_data(), p_qos);
}	

int GDPaho::unsubscribe(const String p_sub) {
	if (!m_wrapper) {
		return PAHO_ERR_NOT_INIT;
	}
	if (!m_wrapper->is_connected()) {
		return PAHO_ERR_NOT_CONNECTED;
	}
	return m_wrapper->unsubscribe_to(p_sub.utf8().get_data());
}

String GDPaho::reason_code_string(const int p_rc) {
	if (p_rc == PAHO_ERR_NOT_INIT) {
		return "MQTT client is not initialized, initialise() must be call first";
	} else if (p_rc == PAHO_ERR_CONNECTION_NOT_INITIALIZED_ONCE) {
		return "MQTT client is not completely initialized, connect() must be called at least once";
	} else if (p_rc == PAHO_ERR_ALREADY_CONNECTED) {
		return "MQTT client is already connected";
	} else if (p_rc == PAHO_ERR_NOT_CONNECTED) {
		return "MQTT client is not connected";
	}
	return String(mqtt::exception::reason_code_str(p_rc).c_str());
}

//###############################################################
//	Callback
//###############################################################


//###############################################################
//	Signals
//###############################################################

void GDPaho::emit_on_connect(const int p_rc) {
	emit_signal("connected", p_rc);
}

void GDPaho::emit_on_disconnect() {
	emit_signal("disconnected");
}

void GDPaho::emit_on_publish(const int p_mid) {
	emit_signal("published", p_mid);
}

void GDPaho::emit_on_message(const mqtt::const_message_ptr p_message) {
	emit_signal("received", String(p_message->get_topic().c_str()),  String(p_message->to_string().c_str()));
}

void GDPaho::emit_on_subscribe(const int p_mid, const std::string& p_topic) {
	emit_signal("subscribed", p_mid, String(p_topic.c_str()));
}

void GDPaho::emit_on_unsubscribe(const int p_mid, const std::string& p_topic) {
	emit_signal("unsubscribed", p_mid, String(p_topic.c_str()));
}

void GDPaho::emit_on_log(const int p_level, const std::string& p_message) {
	emit_signal("log", p_level, String(p_message.c_str()));
}

void GDPaho::emit_on_error(const std::string& p_message, const int p_rc) {
	emit_signal("error", String(p_message.c_str()), p_rc);
}
