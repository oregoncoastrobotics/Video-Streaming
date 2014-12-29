'''
display_video written by Daniel Wirick 12-28-14 as a front-end to recv_stream.

This program can be used and distributed without restrictions.

Version: 0.2
'''
import pygame
import sys
import time
import subprocess
import os

pygame.init ()

font = pygame.font.Font(None, 36)

wrk_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(wrk_dir)

# A 640x480 frame should be 921600 bytes

buff = ""

display = pygame.display.set_mode ((640,480))

#need to subprocess recv_stream
recv_stream = subprocess.Popen(["../20-recv_stream/bin/recv_stream"], shell=False, bufsize=0, stdout=subprocess.PIPE)
pipe = recv_stream.stdout #921600

running = True
oldtime = time.time ()
fps = 0.0
while running:
	data = pipe.read (921600) #blocking
	image = pygame.image.frombuffer (data, (640,480), "RGB")
	text = font.render(str(fps)[0:3], 1, (10, 220, 220))
	display.blit (image, (0, 0))
	display.blit(text, (0,0))
	pygame.display.flip ()
	newtime = time.time ()
	fps = 1.0/(newtime - oldtime)
	oldtime = newtime

	for event in pygame.event.get ():
		if event.type == pygame.QUIT:
			recv_stream.terminate ()
			running = False

	#time.sleep (.01) read is blocking so sleep isn't needed.
