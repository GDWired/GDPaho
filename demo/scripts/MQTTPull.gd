extends PahoClient


@onready var _data : Node = $Data


func _ready():
	initialise()


func _on_MQTTClient_received(_topic: String, payload: String) -> void:
	# MQTT data received
	if payload == "":
		_data.text = "<empty>"
	else:
		_data.text = payload


func _on_MQTTPull_connected(_reason_code: int) -> void:
	var _rc = subscribe("DATA", 1)
