'''
display_video written by Daniel Wirick 12-28-14 as a front-end to recv_stream.

This program can be used and distributed without restrictions.

Version: 0.1
'''
import pygame
import sys
import time
import subprocess
import os

pygame.init ()

wrk_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(wrk_dir)

# A 640x480 frame should be 921600 bytes

buff = ""

display = pygame.display.set_mode ((640,480))

#need to subprocess recv_stream
recv_stream = subprocess.Popen(["Stream_Converter/recv_stream"], shell=False, bufsize=0, stdout=subprocess.PIPE)
pipe = recv_stream.stdout #921600

running = True
while running:
	#imgfile = open ("/home/daniel/A10/video_streaming/outfile")
	data = pipe.read (921600)
	if len(data) == 921600:
		image = pygame.image.frombuffer (data, (640,480), "RGB")
		display.blit (image, (0, 0))
		pygame.display.flip ()
	for event in pygame.event.get ():
		if event.type == pygame.QUIT:
			recv_stream.terminate ()
			running = False

	time.sleep (.01)
