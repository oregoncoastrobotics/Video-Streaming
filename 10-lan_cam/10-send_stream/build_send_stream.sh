#!/bin/bash

echo "Building V4L2 Video Stream Server"
gcc -O2 -Wall -W -Werror `pkg-config --cflags --libs libv4l2` send_stream.c -o bin/send_stream

echo "All Done :)"
