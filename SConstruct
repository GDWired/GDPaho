#!/usr/bin/env python

import os
import sys


env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=['src/'])
sources = Glob('src/*.cpp')
env.Append(CPPPATH=["include"])
if env['platform'] == "windows":
    env.Append(LIBPATH=["{}".format("C:\Program Files (x86)\paho-mqtt-cpp\lib")])
    env.Append(LIBPATH=["{}".format("C:\Program Files (x86)\Eclipse Paho C\lib")])
    env.Append(LIBPATH=["{}".format("C:\Program Files\OpenSSL-Win64\lib\VC\static")])
    env.Append(LIBS=["paho-mqtt3as-static.lib", "paho-mqttpp3-static.lib", "libcrypto64MD.lib", "libssl64MD.lib", "ws2_32.lib", "Advapi32.lib", "User32.lib"])
if env['platform'] == "osx":
    env.Append(CPPPATH=["/opt/homebrew/opt/openssl@3/include"])
    env.Append(LIBPATH=["/opt/homebrew/opt/openssl@3/lib"])
    env.Append(LIBS=["paho-mqtt3as", "paho-mqttpp3", "ssl"])
elif env['platform'] in ('x11', 'linux'):
    env.Append(LIBS=["paho-mqtt3as", "paho-mqttpp3", "crypto", "ssl"])

# Create lib
library = env.SharedLibrary("demo/addons/GDPaho/{}/libgdpaho{}".format(env['platform'], env["SHLIBSUFFIX"]), source=sources)
Default(library)
