recv_stream written by Daniel Wirick 12-28-14 as a tcp client to receive video from the send_stream program.

display_video written by Daniel Wirick 12-28-14 as a front-end to recv_stream.

This program can be used and distributed without restrictions.

Currnet Usage:
	python display_video.py

	-Note: recv_stream binary needs to be in the same folder as display_video.py

	-Note: you can run recv_stream by itself as follows:
	./recv_stream (will output received video to stdout)

	- Warning:  In the current version all settings are hard coded.  If you need to use this program you will need to change the code or wait for a configurable version to come out.

There is much yet to be done:

	-display_video
	1: Use a config file to get default settings
	3: Ping available IP addresses for possible servers
	4: Handshake with server and negotiate settings
	5: Add UI to expand on plain video screen, or make modular to use in top-level robot program

	-recv_stream
	1: Use a config file to get default settings
	3: Ping available IP addresses for possible servers
	4: Handshake with server and negotiate settings
	5: Add UDP back in as an option?

Known Issues:
	1: In this version I'll just stick with the todo list above.

Build How To:
	Pre-Requisites:
	-Unknown, I haven't built from a clean OS in a while (for sure build-essentials or equivelant)

	./build_recv_stream

V 0.1  inital commit
V 0.2  Added working YUYV to RGB decoder
