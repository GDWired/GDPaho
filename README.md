<p align="center"><img src="https://user-images.githubusercontent.com/4105962/204161994-7b135944-4e3a-47af-8cf2-0c7a2b13701a.png" width="256"></p>

# <p align="center">GDPaho (Godot 3.5)</p>

A wrapping of paho cpp (https://www.eclipse.org/paho/) able to make MQTT clients in Godot, is a part of GDWired (https://github.com/GDWired).

Works on Windows, macOS and Linux but there are only few functions exposed.

**Windows**
 - OpenSSL must be installed: https://slproweb.com/products/Win32OpenSSL.html
 - Build paho lib
 - run scons target=release paho_cpp="C:\Program Files (x86)\paho-mqtt-cpp\lib" paho_c="C:\Program Files (x86)\Eclipse Paho C\lib" ssl="C:\Program Files\OpenSSL-Win64\lib\VC\static"

**Linux**
 - OpenSSL must be installed (example: apt install libssl-dev)
 - Build paho lib
 - run scons target=release

**macOS**
 - OpenSSL must be installed using homebrew (brew install openssl@3)
 - Build paho lib
 - run scons target=release
 
## Paho build

Should be done in root

**paho.c**
```console
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
mkdir build
cd build
[macOS] cmake .. -DPAHO_BUILD_SHARED=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_HIGH_PERFORMANCE=TRUE -DPAHO_WITH_SSL=TRUE -DPAHO_ENABLE_TESTING=FALSE -DOPENSSL_ROOT_DIR=$(brew --prefix openssl) -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON
[Windows] cmake .. -DPAHO_BUILD_SHARED=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_HIGH_PERFORMANCE=TRUE -DPAHO_WITH_SSL=TRUE -DPAHO_ENABLE_TESTING=FALSE -DOPENSSL_ROOT_DIR="C:\Program Files\OpenSSL-Win64" -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON
[Linux] cmake .. -DPAHO_BUILD_SHARED=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_HIGH_PERFORMANCE=TRUE -DPAHO_WITH_SSL=TRUE -DPAHO_ENABLE_TESTING=FALSE -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON
cmake --build . --target install --config "MinSizeRel"
```

**paho.cpp**
```console
git clone https://github.com/eclipse/paho.mqtt.cpp.git
cd paho.mqtt.cpp
mkdir build
cd build
[macOS] cmake .. -DPAHO_BUILD_SHARED=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_WITH_SSL=TRUE -DOPENSSL_ROOT_DIR=$(brew --prefix openssl) -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON
[Windows] cmake .. -DPAHO_BUILD_SHARED=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_WITH_SSL=TRUE -DOPENSSL_ROOT_DIR="C:\Program Files\OpenSSL-Win64" -DPAHO_MQTT_C_INCLUDE_DIRS="C:\Program Files (x86)\Eclipse Paho C\include" -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON
[Linux] cmake .. -DPAHO_BUILD_SHARED=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_WITH_SSL=TRUE -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON
cmake --build . --target install --config "MinSizeRel"
```

## API

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
