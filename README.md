<p align="center"><img src="https://user-images.githubusercontent.com/4105962/204161994-7b135944-4e3a-47af-8cf2-0c7a2b13701a.png" width="256"></p>

# <p align="center">GDPaho (Godot 4.1 & 3.5)</p>

Tested on macOS, Linux and Windows using cmake. Must work on Android but not tested.
Branch godot-3 (for Godot 3.X developments) will be maintained bug corrections only, no new features.

A wrapping of paho cpp (https://www.eclipse.org/paho/) able to make MQTT clients in Godot, is a part of GDWired (https://github.com/GDWired).

Works on Windows, macOS and Linux but there are only few functions exposed. The Godot build dependencies must be installed first, you can find everything about it here https://docs.godotengine.org/en/4.1/development/compiling/index.html.

Dependencies:
 - Godot build dependencies (https://docs.godotengine.org/en/4.1/development/compiling/index.html)
 - OpenSSL
   - Windows (https://slproweb.com/products/Win32OpenSSL.html)
   - Linux `apt install libssl-dev` or equivalent
   - macOS `brew install openssl@3`
 - Paho lib (see specific section below)
 - cmake
   - Windows (https://cmake.org/download) or maybe using Visual Studio Installer
   - Linux `apt install cmake` or equivalent
   - macOS `brew install cmake`
 - git
   - Windows (https://git-scm.com/downloads)
   - Linux `apt install git` or equivalent
   - macOS `brew install git`

The editor uses the debug version, but if you want to export your project as a release, you have to compile everything as a release as well:
 - Run `git clone --recurse-submodules https://github.com/GDWired/GDPaho.git -b godot-4`

Compile everything in release and copy libs to the demo folder ready to use by Godot4.
```cmake
cmake -DBUILD_FOR_EDITOR=True . -B build
cmake --build build
```

## Paho build

Should be done in root

**paho.c**
```console
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
[macOS] cmake . -DPAHO_BUILD_SHARED=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_HIGH_PERFORMANCE=TRUE -DPAHO_WITH_SSL=TRUE -DPAHO_ENABLE_TESTING=FALSE -DOPENSSL_ROOT_DIR=$(brew --prefix openssl) -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON -B build
[Windows] cmake . -DPAHO_BUILD_SHARED=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_HIGH_PERFORMANCE=TRUE -DPAHO_WITH_SSL=TRUE -DPAHO_ENABLE_TESTING=FALSE -DOPENSSL_ROOT_DIR="C:\Program Files\OpenSSL-Win64" -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON -B build
[Linux] cmake . -DPAHO_BUILD_SHARED=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_HIGH_PERFORMANCE=TRUE -DPAHO_WITH_SSL=TRUE -DPAHO_ENABLE_TESTING=FALSE -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON -B build
cmake --build build --target install --config "MinSizeRel"
```

**paho.cpp**
```console
git clone https://github.com/eclipse/paho.mqtt.cpp.git
cd paho.mqtt.cpp
[macOS] cmake . -DPAHO_BUILD_SHARED=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_WITH_SSL=TRUE -DOPENSSL_ROOT_DIR=$(brew --prefix openssl) -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON -B build
[Windows] cmake . -DPAHO_BUILD_SHARED=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_WITH_SSL=TRUE -DOPENSSL_ROOT_DIR="C:\Program Files\OpenSSL-Win64" -DPAHO_MQTT_C_INCLUDE_DIRS="C:\Program Files (x86)\Eclipse Paho C\include" -DPAHO_MQTT_C_LIBRARIES="C:\Program Files (x86)\Eclipse Paho C\lib" -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON -B build
[Linux] cmake . -DPAHO_BUILD_SHARED=FALSE -DPAHO_BUILD_STATIC=TRUE -DPAHO_WITH_SSL=TRUE -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON -B build
cmake --build build --target install --config "MinSizeRel"
```

## Demo

In the "demo" folder there is a sample project, after compiling the project you can open it with Godot1 4.1. (WIP you can also download a precompiled version of the plugin from the asset store or in the release section of github WIP)

<img width="1264" alt="Capture d’écran 2023-11-03 à 23 03 35" src="https://github.com/GDWired/GDPaho/assets/4105962/6c65e9e1-abcc-44d4-af39-d0c06b554a0a">

The picture represent the demo project:
 - The first line is the subscription parameters, subscribe to the SIN topic and expect JSON formatted data like [t,sin(t)] and plot it.
 - The second line is the pulisher parameters, publish JSON formatted data to the SIN topic [t,sin(t)].
 - The last line sends the text from the first edit line to the topic DATA (the second edit line subscribes to DATA and displays the sent text).

## API

``` gdscript
# Initialize the client
# @param id string to use as the client id
# @param host the broker address
# @param port the broker port
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
initialise(id: String, host: String, port: String) -> int

# Set username & password
# @param username username
# @param pasword password
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
username_pw_set(username: String, password: String) -> int

# Return true if connected 
# @return true if connected 
is_connected_to_broker() -> bool

# Connect the client to the broker (server)
# @param clean_session set to true to instruct the broker to clean all messages and subscriptions on disconnect, false to instruct it to keep them
# @param broker_keep_alive keep alive time
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
broker_connect(clean_session: bool, keep_alive: int) -> int

# Reconnect the client to the broker (server)
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
broker_reconnect() -> int

# Disconnect the client
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
broker_disconnect() -> int

# Publish to a topic
# @param topic the name of the topic
# @param data the data to send
# @param qos the QoS used
# @param retain if true the data is retained
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
publish(topic: String, data: String, qos: int, retain: bool) -> int

# Subscribe to a topic
# @param topic the name of the topic
# @param qos the QoS used
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
subscribe(topic: String, qos: int) -> int

# Unsubscribe to a topic
# @param topic the name of the topic
# @return the reason code, if something wrong happen. 0 = OK (see https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html#_Toc3901031)
unsubscribe(topic: String) -> int

````
