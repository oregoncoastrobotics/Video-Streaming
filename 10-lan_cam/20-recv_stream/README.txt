recv_stream written by Daniel Wirick 12-28-14 as a tcp client to receive video from the send_stream program.

This program can be used and distributed without restrictions.

Currnet Usage:
	./recv_stream (will output received video to stdout)

	- Warning:  In the current version all settings are hard coded.  If you need to use this program you will need to change the code or wait for a configurable version to come out.

There is much yet to be done:

	1: Use a config file to get default settings
	3: Ping available IP addresses for possible servers
	4: Handshake with server and negotiate settings
	5: Add UDP back in as an option?
	6: Add real decoder for YUYV video

Known Issues:
	1: In this version I'll just stick with the todo list above.

Build How To:
	Pre-Requisites:

	./build_recv_stream
