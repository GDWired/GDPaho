extends PahoClient


onready var _text_to_send : LineEdit = $Text


func _ready():
	initialise()


func _on_Publish_pressed() -> void:
	var _rc = publish("DATA", _text_to_send.text, 1)

