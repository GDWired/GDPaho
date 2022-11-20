extends PahoClient


var _loop: bool = true
var _time: float = 0.0
var _x: float = 0.0
var _publish_frequency: float = 10

onready var _publish_button : Button = $Publish
onready var _publish_frequency_spin : SpinBox = $PublishFrequency


func _ready():
	# Init values from UI
	_loop = _publish_button.pressed
	_publish_frequency = _publish_frequency_spin.value


func _process(delta: float) -> void:
	# Publish loop
	_time += delta
	_x += delta
	if _loop and _time >= (1.0 / _publish_frequency):
		var _rc = publish("SIN", "[" + str(_x) + "," + str(sin(_x)) + "]", 1)
		_time = 0.0


func _on_Publish_toggled(button_pressed: bool) -> void:
	_loop = button_pressed
	if not is_connected_to_broker():
		broker_connect()


func _on_PublishFrequency_value_changed(value: float) -> void:
	_publish_frequency = value



func _on_MQTTPub_unsubscribed(message_id, topic) -> void:
	print("UNS: " + str(message_id) + ": " + topic)
func _on_MQTTPub_subscribed(message_id, topic) -> void:
	print("SUB: " + str(message_id) + ": " + topic)
func _on_MQTTPub_log_received(level, message) -> void:
	print("LOG: " + str(level) + ": " + message)
func _on_MQTTPub_error_received(message, reason_code) -> void:
	print("ERR: " + str(reason_code) + ": " + message)
func _on_MQTTPub_published(message_id) -> void:
	print("PUB: " + str(message_id))
func _on_MQTTPub_connected(reason_code) -> void:
	print("CON: " + str(reason_code))
func _on_MQTTPub_received(topic, payload) -> void:
	print("REC: " + topic + ": " + payload)
func _on_MQTTPub_disconnected() -> void:
	print("DIS")
