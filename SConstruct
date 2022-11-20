#!/usr/bin/env python

import os
import sys

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=['src/'])
sources = Glob('src/*.cpp')

if env['platform'] == "windows":
    env.Append(CPPPATH=["include"])
    env.Append(LIBPATH=["lib"])
    env.Append(LIBS=["{}/paho-mqtt3as-static".format(env['target']), "{}/paho-mqttpp3-static".format(env['target']), "libcrypto64MD.lib", "libssl64MD.lib", "ws2_32.lib", "Advapi32.lib", "User32.lib"])

# Create lib
library = env.SharedLibrary("demo/addons/GDPaho/{}/libgdpaho{}".format(env['platform'], env["SHLIBSUFFIX"]), source=sources)
Default(library)
