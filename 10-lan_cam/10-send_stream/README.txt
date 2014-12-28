send_stream program modified for TCP streaming and display of pixel format settings by Daniel Wirick 12-28-14 but source is based on code from boneCV and thus retains the desired copyright notice below.

Currnet Usage:
        Usage: send_stream [options]
        Version 1.3
        Options:
        -d | --device name [device] Video device name
        -h | --help                 Print this message
        -m | --mmap                 Use memory mapped buffers *default
        -r | --read                 Use read() calls
        -u | --userp                Use application allocated buffers
        -o | --output [ip address]  Outputs stream to stdout, or optionally ip address
        -f | --format               Force format to 640x480 YUYV
	-F | --formatH264           Force format to 1920x1080 H264
	-c | --count [frames]       Number of frames to grab - use 0 for infinite

Example usage: send_stream -f -o 192.168.1.106 -c 0
Starts TCP server for video stream of YUYV at 640x480

There is much yet to be done:

	1: Make Server listen again after client has closed socket
	2: Use a config file to get default settings (take away command line settings, I don't like them)
	3: List available video settings
	4: Handshake with client and negotiate settings
	5: Add UDP back in as an option?

Known Issues:
	1: command line input -O [ip] doesn't actually use the ip address anymore (needed it when streaming on udp)

Build How To:
	Pre-Requisites:

	./build_send_stream

boneCV
======

Beaglebone Webcam and OpenCV Examples Repository.

Copyright Derek Molloy, School of Electronic Engineering, Dublin City University

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that source code redistributions retain this notice.

This software is provided AS IS and it comes with no warranties of any type. 

The description of the code presented here is on the website: www.derekmolloy.ie. If you use this code 
or the content of the associated video in your research, please cite:

Molloy, D. [DerekMolloyDCU]. (2013, May, 25). Beaglebone: Video Capture and Image Processing 
on Embedded Linux using OpenCV [Video file]. Retrieved from http://www.youtube.com/watch?v=8QouvYMfmQo

Please find more information on these videos at: http://www.derekmolloy.ie/  


