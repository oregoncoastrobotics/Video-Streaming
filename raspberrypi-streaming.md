## Method 1 - Streaming with mjpg-streamer (only works on your local network)


###Results

-q 5 (raspistill quality set to 5)
* Was skipping a lot of frames at both 320x240 and 640x480.  I remember getting better performance than this before.
* Resolution: 640x480, over a lan, ~0.5s lag, maybe slightly slower than ffmpeg at 320x240. CPU at ~3.5%, ~15 fps
* Resolution: 960x720. Same lag, CPU ~3.5%,
* Resolution: 3240x240: This has the best lag, image quality is pretty low. but is useful for reaction time. CPU ~ < 3%. 17 fps.

-q 90 (raspistill quality set to 90)
* Resolution: 960x720:  Great image quality but lag ~1s. CPU 6-7%

* Resolution: 640x480:  Better lag, slighlty less but still noticable. CPU 4%


###Instructions
Here is a video showing how to connect the camera to the Raspberry Pi http://youtu.be/GImeVqHQzsE 

Follow the instruction here http://blog.miguelgrinberg.com/post/how-to-build-and-run-mjpg-streamer-on-the-raspberry-pi to install the software on the RPi. They are summarized below

**NOTE** First make sure the camera is enabled. Run `sudo raspi-config` and enable the camera.

1. Install software packages. 
	```sh
	cd ~/
	sudo apt-get install libjpeg8-dev imagemagick libv4l-dev
	sudo ln -s /usr/include/linux/videodev2.h /usr/include/linux/videodev.h
	
	```
2. Then make a temp directory to download the MJPEG-Streamer zip file

	```sh
	mkdir ~/temp
	cd /temp
	wget http://sourceforge.net/code-snapshots/svn/m/mj/mjpg-streamer/code/mjpg-streamer-code-182.zip
	unzip mjpg-streamer-code-182.zip
	cd mjpg-streamer-code-182/mjpg-streamer
	make mjpg_streamer input_file.so output_http.so
	
	sudo cp mjpg_streamer /usr/local/bin
	sudo cp output_http.so input_file.so /usr/local/lib/
	
	```

3. Now we should be able to  start the camera. The code below will start 
the camera and a webserver on port 8080 that will stream the video. 
To see run the code and open a browser and point it at `http://RASPBERRYPI_IP:8080`

	```sh
	mkdir /tmp/stream
	raspistill --nopreview -w 640 -h 480 -bm -q 5 -o /tmp/stream/pic.jpg -tl 100 -t 9999999 -th 0:0:0
	
	LD_LIBRARY_PATH=/usr/local/lib mjpg_streamer -i "input_file.so -f /tmp/stream -n pic.jpg" -o "output_http.so -w /home/pi/temp/mjpg-streamer-code-182/mjpg-streamer/www"

	```
	----

###Commands used

```
raspistill --nopreview -w 960 -h 720 -bm -q 5 -o /tmp/stream/pic.jpg -tl 100 -t 9999999 -th 0:0:0

raspistill --nopreview -w 960 -h 720 -bm -q 90 -o /tmp/stream/pic.jpg -tl 100 -t 9999999 -th 0:0:0

raspistill --nopreview -w 640 -h 480 -bm -q 90 -o /tmp/stream/pic.jpg -tl 100 -t 9999999 -th 0:0:0

raspistill --nopreview -w 640 -h 480 -bm -q 10 -o /tmp/stream/pic.jpg -tl 100 -t 9999999 -th 0:0:0

raspistill --nopreview -w 320 -h 240 -bm -q 10 -o /tmp/stream/pic.jpg -tl 100 -t 9999999 -th 0:0:0

LD_LIBRARY_PATH=/usr/local/lib mjpg_streamer -i "input_file.so -f /tmp/stream -n pic.jpg" -o "output_http.so -w /home/pi/temp/mjpg-streamer-code-182/mjpg-streamer/www"
```

## Method 2 - Websockets streaming with ffmpeg
This method requires a server the accept the streaming output from the Raspberry Pi and then broadcasts it over  web sockets to browsers. The instruction for setting up this streaming server can be found here [https://github.com/wilblack/ardyh-streamer](https://github.com/wilblack/ardyh-streamer)

###Results
* I was able to stream 320x240 over the Internet with about ~0.5 seconds of lag for over 30 mins using about ~78% CPU.
* Streaming at 640x480 over the Internet didn't really work. The lag just kept increasing and was well over a minute 1 after only a couple of minutes cpu was ~95%. This resolution is unusable

###Instructions 

1. On the Raspberry Pi build and configure [ffmpeg](http://ffmpeg.org/). It takes a while (~3hrs). I followed the instructions [here](http://sirlagz.net/2012/08/04/how-to-stream-a-webcam-from-the-raspberry-pi/). Below are the steps from that link.
 
  0. Make sure the camera is enabled on the Pi.
  
  ```
  sudo apt-get update
  sudo apt-get upgrade
  sudo raspi-config
  # Then follow the menu to enable the camera.
  ```

  1. Clone the repo and and build the binaries.
    
   ```
   cd /usr/src/
   git clone git://source.ffmpeg.org/ffmpeg.git
   sudo chown -R pi:users ffmpeg
   cd ffmpeg
   ./configure    # This takes a minute or so. 
   make           # This takes about 3+ hours
   make install
   ```

  To test this run 
  `raspivid -t 5000 -w 960 -h 540 -fps 25 -b 500000 -vf -o - | ffmpeg -i - -vcodec copy -an -r 25 -f flv test.flv`. 
  IF you get an error about `ffmpeg` add it to your path. #TODO Add instruction for adding ffmpeg to the path. 
   
2. Set up and start the streaming server. This can be local or over the Internet.  [Instructions for setting up the streaming server](https://github.com/wilblack/ardyh-streamer). Once everything is installed start your server with a secret password.
	
    ```
    node stream-server.js <your-password>
    ```
     
     Note the IP address it is listening on. 


3. Then on the Raspberry Pi start the camera.
	```
	ffmpeg -s 320x240 -f video4linux2 -i /dev/video0 -f mpeg1video -b 800k -r 30 http://<domain:port>/<your-password>/320/240/

	```
