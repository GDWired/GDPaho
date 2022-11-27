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
	initialise()


func _process(delta: float) -> void:
	# Publish loop
	_time += delta
	_x += delta
	if _loop and _time >= (1.0 / _publish_frequency):
		var _rc = publish("SIN", "[" + str(_x) + "," + str(sin(_x)) + "]", 1)
		_time = 0.0


func _on_Publish_toggled(button_pressed: bool) -> void:
	_loop = button_pressed


func _on_PublishFrequency_value_changed(value: float) -> void:
	_publish_frequency = value

