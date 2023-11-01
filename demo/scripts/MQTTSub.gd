extends PahoClient


var _refresh_time: float = 1
# This Chart will plot 3 different functions
var _chart_func: Function

@onready var _chart: Chart = $Chart # Not a part of this plugin (https://github.com/fenix-hub/godot-engine.easy-charts)
@onready var _subscribe_button : Node = $Buttons/Subscribe


func _ready():
	# Init chart
	
	# Let's create our @x values
	var x: PackedFloat32Array = ArrayOperations.multiply_float(range(-1, 1, 100), 1.0)
	
	# And our y values. It can be an n-size array of arrays.
	# NOTE: `x.size() == y.size()` or `x.size() == y[n].size()`
	var y: Array = ArrayOperations.multiply_int(ArrayOperations.sin(x), 1)
	
	# Let's customize the chart properties, which specify how the chart
	# should look, plus some additional elements like labels, the scale, etc...
	var cp: ChartProperties = ChartProperties.new()
	cp.colors.frame = Color("#161a1d")
	cp.colors.background = Color.TRANSPARENT
	cp.colors.grid = Color("#283442")
	cp.colors.ticks = Color("#283442")
	cp.colors.text = Color.WHITE_SMOKE
	cp.draw_bounding_box = false
	cp.title = "Sinus"
	cp.x_label = "t"
	cp.y_label = "sin(t)"
	cp.x_scale = 10
	cp.y_scale = 10
	# and interecept clicks on the plot

	# Let's add values to our functions
	_chart_func = Function.new(
		x, y, "Sinus", # This will create a function with x and y values taken by the Arrays 
						# we have created previously. This function will also be named "Pressure"
						# as it contains 'pressure' values.
						# If set, the name of a function will be used both in the Legend
						# (if enabled thourgh ChartProperties) and on the Tooltip (if enabled).
		# Let's also provide a dictionary of configuration parameters for this specific function.
		{ 
			color = Color("#36a2eb"), 		# The color associated to this function
			marker = Function.Marker.CIRCLE, 	# The marker that will be displayed for each drawn point (x,y)
											# since it is `NONE`, no marker will be shown.
			type = Function.Type.LINE, 		# This defines what kind of plotting will be used, 
											# in this case it will be a Linear Chart.
		}
	)

	# Now let's plot our data
	_chart_func.add_point(0, 0)
	_chart.plot([_chart_func], cp)
	#_refresh_time = _refresh_time_spin.value
	initialise()


func _process(_delta: float) -> void:
	# Refresh loop
	_chart.queue_redraw() # This will force the Chart to be updated


func check_subscribe(button_pressed: bool) -> void:
	if button_pressed:
		var _rc = subscribe("SIN", 1)
	else:
		var _rc = unsubscribe("SIN")


func _on_MQTTClient_received(_topic: String, payload: String) -> void:
	# MQTT data received
	if payload:
		var test_json_conv = JSON.new()
		var _result = test_json_conv.parse(payload)
		var data = test_json_conv.get_data()
		_chart_func.add_point(data[0], data[1])


func _on_Subscribe_toggled(button_pressed: bool) -> void:
	check_subscribe(button_pressed)


func _on_RefreshFrequency_value_changed(value: float) -> void:
	_refresh_time = value


func _on_MQTTSub_connected(_reason_code: int):
	check_subscribe(_subscribe_button.pressed)
	
