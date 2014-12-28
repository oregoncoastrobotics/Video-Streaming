#!/bin/bash

echo "Building Video Stream Client"
gcc -O2 -Wall -W -Werror recv_stream.c -o bin/recv_stream

echo "All Done :)"
