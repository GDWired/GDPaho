<p align="center"><img src="https://user-images.githubusercontent.com/4105962/204161994-7b135944-4e3a-47af-8cf2-0c7a2b13701a.png" width="256"></p>

# <p align="center">GDPaho (Godot 3.5)</p>

A wrapping of paho cpp (https://www.eclipse.org/paho/) able to make MQTT clients in Godot.

Works on Windows, macOS and Linux but there are only few functions exposed.

Paho lib is available on the folder lib, but it is better to compile your own lib paho.

Windows
 - OpenSSL must be installed: https://slproweb.com/products/Win32OpenSSL.html
 - Copy past <Installation dir>\OpenSSL-Win64\lib\VC\static libcrypto64MD.lib and libssl64MD.lib to lib
 - Run scons target=debug (or release)

Linux: 
 - OpenSSL must be installed
 - Run scons target=debug (or release)

``` gdscript
# Initialize the client
# @param id string to use as the client id.
# @param host the broker address
# @param port the broker port
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
initialise(id, host, port)

# Return true if connected 
# @return true if connected 
is_connected_to_broker()

# Connect the client to the broker (server)
# @param clean_session set to true to instruct the broker to clean all messages and subscriptions on disconnect, false to instruct it to keep them
# @param broker_keep_alive keep alive time
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
broker_connect(clean_session, keep_alive)

# Reconnect the client to the broker (server)
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
broker_reconnect()

# Disconnect the client
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
broker_disconnect()

# Publish to a topic
# @param topic the name of the topic
# @param data the data to send
# @param qos the QoS used
# @param retain if true the data is retained
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
publish(topic, data, qos, retain)

# Subscribe to a topic
# @param topic the name of the topic
# @param qos the QoS used
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
subscribe(topic, qos)

# Unsubscribe to a topic
# @param topic the name of the topic
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
unsubscribe(topic)

````
