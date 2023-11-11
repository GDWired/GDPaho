@tool
##
# Plugin main
# If you have any questions/remarks
# 	- https://github.com/jferdelyi/GDPaho
#	- jferdelyi@gmail.com
##

extends EditorPlugin

func _enter_tree():
	add_autoload_singleton("GDPahoUtils", "res://addons/GDPaho/GDPahoUtils.gd")

func _exit_tree():
	remove_autoload_singleton("GDPahoUtils")
