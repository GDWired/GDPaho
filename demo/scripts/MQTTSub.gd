extends PahoClient


var _time: float = 0.0
var _data: Array = []
var _refresh_time: float = 1

onready var _chart : Node = $GDCharts # Not a part of this plugin (https://github.com/binogure-studio/chart-gd)
onready var _subscribe_button : Node = $Buttons/Subscribe
onready var _refresh_time_spin : SpinBox = $Buttons/RefreshTime
  

func _ready():
	# Init chart
	_chart.initialize(_chart.LABELS_TO_SHOW.NO_LABEL, {sinus_x = Color(1.0, 0.18, 0.18)})
	_chart.set_max_values(100)
	_refresh_time = _refresh_time_spin.value
	initialise()


func _process(delta: float) -> void:
	# Refresh loop
	_time += delta
	if _time >= _refresh_time:
		for d in _data:
			if d:
				_chart.create_new_point({label = d[0], values = { sinus_x = d[1] }})
		_data.clear()
		_time = 0.0


func check_subscribe(button_pressed: bool) -> void:
	if button_pressed:
		var _rc = subscribe("SIN", 1)
	else:
		var _rc = unsubscribe("SIN")


func _on_MQTTClient_received(_topic: String, payload: String) -> void:
	# MQTT data received
	if payload:
		var result = JSON.parse(payload).result
		_data.append(result)


func _on_Subscribe_toggled(button_pressed: bool) -> void:
	check_subscribe(button_pressed)


func _on_RefreshFrequency_value_changed(value: float) -> void:
	_refresh_time = value


func _on_MQTTSub_connected(_reason_code: int):
	check_subscribe(_subscribe_button.pressed)
	
