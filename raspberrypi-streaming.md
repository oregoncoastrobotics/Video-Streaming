## Method 1 - Streaming with mjpg-streamer (only works on your local network)

Here is a video showinghow to connect the camera to the Raspberry Pi http://youtu.be/GImeVqHQzsE 

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
	cd ~/temp
	wget http://sourceforge.net/code-snapshots/svn/m/mj/mjpg-streamer/code/mjpg-streamer-code-182.zip
	unzip mjpg-streamer-code-182.zip
	cd mjpg-streamer-code-182/mjpg-streamer
	make mjpg_streamer input_file.so output_http.so
	
	sudo cp mjpg_streamer /usr/local/bin
	sudo cp output_http.so input_file.so /usr/local/lib/

	cp -R www ~/projects/lilybot/rpi_client/camera_stream/  # This is what I did but I don't think its right.
	
	```

3. Now we should be able to  start the camera. The code below will start 
the camera and a webserver on port 8080 that will stream the video. 
To see run the code and open a browser and point it at `http://RASPBERRYPI_IP:8080`

	```sh
	mkdir /tmp/stream
	raspistill --nopreview -w 640 -h 480 -q 5 -o /tmp/stream/pic.jpg -tl 100 -t 9999999 -th 0:0:0
	
	LD_LIBRARY_PATH=/usr/local/lib mjpg_streamer -i "input_file.so -f /tmp/stream -n pic.jpg" -o "output_http.so -w /home/pi/Projects/lilybot/jjbot/www"

	```
	----
	
	
	## Method 2 - Websockets streaming with ffmpeg


1. On the Raspberry Pi Build and configure [ffmpeg](http://ffmpeg.org/) on the Rapsberry Pi. It takes a while. I followed the instructions [here](http://sirlagz.net/2012/08/04/how-to-stream-a-webcam-from-the-raspberry-pi/). It takes a while. Below are the steps from that link.
 
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
  `raspivid -t 5000 -w 960 -h 540 -fps 25 -b 500000 -vf -o - | /usr/src/ffmpeg/ffmpeg -i - -vcodec copy -an -r 25 -f flv test.flv`. 
  
  NOTE: Still need to add `ffmpeg` to the path.
  
2. On the socket server install the stream-server.js script from https://github.com/phoboslab/jsmpeg
	
    ```
    npm install ws
    node stream-server.js yourpassword
    ```
3. Then on the Raspberry Pi start the camera.
	```
	ffmpeg -s 320x240 -f video4linux2 -i /dev/video0 -f mpeg1video -b 800k -r 30 http://localhost:8082/password/320/240/

	```
