#!/usr/bin/env python

import os
import sys

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=['src/'])
sources = Glob('src/*.cpp')

if env['platform'] == "windows":
    env.Append(CPPPATH=["include"])
    env.Append(LIBPATH=["lib"])
    env.Append(LIBS=["paho-mqtt3as", "paho-mqttpp3-static"])

# Create lib
library = env.SharedLibrary("demo/addons/GDPaho/{}/libgdpaho{}".format(env['platform'], env["SHLIBSUFFIX"]), source=sources)
Default(library)
