#!/bin/bash

export C_INCLUDE_PATH=/opt/X11/include:$C_INCLUDE_PATH

gcc -o egui E_gui2.c -L/opt/X11/lib -lX11

./egui