/*
recv_stream written by Daniel Wirick 12-28-14 as a tcp client to receive video from the send_stream program.

This program can be used and distributed without restrictions.

Version: 0.2
*/

//Standard Imports
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

//Sys Imports
#include <sys/socket.h>
#include <sys/param.h>		//MIN macro

#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h> //inet_pton

#include <errno.h> //errno
#include <unistd.h>
#include <string.h> //strerror

#define IN_BUFF_SIZE 614400
#define OUT_BUFF_SIZE 921600
#define SERVER_SOCKET 27777
#define SERVER_IP "192.168.1.115"

//Video Encodings, only set one to 1 the rest should be 0
#define RGBR 0
#define RGBP 0
#define YUVP 0
#define YUYV 1
#define DUMB 0

//All RGB values need to be between 0 and 255
#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// YCbCr -> RGB
#define CYCbCr2R(Y, Cb, Cr) CLIP( Y + ( 91881 * Cr >> 16 ) - 179 )
#define CYCbCr2G(Y, Cb, Cr) CLIP( Y - (( 22544 * Cb + 46793 * Cr ) >> 16) + 135)
#define CYCbCr2B(Y, Cb, Cr) CLIP( Y + (116129 * Cb >> 16 ) - 226 )

//YUYV -> RGB
#define CYUYV2R(c, e)    CLIP(( 298 * c + 409 * e + 128) >> 8); // red
#define CYUYV2G(c, d, e) CLIP(( 298 * c - 100 * d - 208 * e + 128) >> 8) // green
#define CYUYV2B(c, d)    CLIP(( 298 * c + 516 * d + 128) >> 8) // blue

struct buffer 
{
        void   *start;
        size_t  length;
};

//Ints
static int              sd = -1;
FILE *fp;

//Structs
struct buffer	*in_buff;  //Buffer for image data, a buffer instance of above type
struct buffer	*out_buff;
struct sockaddr_in serv_addr;

static void errno_exit(const char *s)
{
        fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
        exit(EXIT_FAILURE);
}

static void init_net (void) //Need to update for receiving
{

	//set up data values in myaddr struct
	memset((char *)&serv_addr, 0, sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET;   //Network type
	//serv_addr.sin_addr.s_addr = htonl(atol()); //expects long integer
	serv_addr.sin_port = htons(SERVER_SOCKET);
	if (0 == inet_pton(AF_INET, SERVER_IP, &(serv_addr.sin_addr.s_addr))) //&(sa.sin_addr)))
	{
		errno_exit("cannot set server address");
		return;
	}

	//Create a tcp socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{ 
		errno_exit("cannot create socket");
		return;
	}

	//Connect to server
	if (connect(sd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	{ 
		errno_exit("ERROR connecting");
		return;
	}
}

static void send_image(void)
{
	//Send the image to standard out
        fwrite(out_buff[0].start, out_buff[0].length, 1, stdout);
	fflush(stderr);
	//fprintf(stderr, ".");  //write a . to standard error
	fflush(stdout);
}

//Seperate the stream into its Y, Cb, and Cr parts from a v4l2 stream
//Each word has the following format (from the v4l2 docs http://linuxtv.org/downloads/v4l-dvb-apis/packed-yuv.html)
//V4L2_PIX_FMT_YUV565	'YUVP'	 Cb2 Cb1 Cb0 Cr4 Cr3 Cr2 Cr1 Cr0	Y'4 Y'3 Y'2 Y'1 Y'0 Cb5 Cb4 Cb3

//this function requires YUVP (16 bit)
static void YUVP_to_RGB3 (uint8_t* out, const uint8_t* in, int in_size)
{

	int x;
	int Y, Cb, Cr;

	for (x = 0; x < in_size/2; x++, in += 2, out += 3)
	{

	        Y = (in[1] & 248);
	        Cb = (((in[0] & 224) >> 3) + (in[1] << 5));
	        Cr = (in[0] << 3);

	        out[0] = CYCbCr2R(Y, Cb, Cr);
	        out[1] = CYCbCr2G(Y, Cb, Cr);
	        out[2] = CYCbCr2B(Y, Cb, Cr);
	}
}

static void RGBR_to_RGB3 (uint8_t* out, const uint8_t* in, int in_size)
{
	int x;

	for (x = 0; x < in_size/2; x++, in += 2, out += 3)
	{

	        out[0] = (in[0] & 248);
	        out[1] = (((in[0] & 7) << 5) + ((in[1] >> 3) & 28));
	        out[2] = (in[1] << 3);
	}
}

static void RGBP_to_RGB3 (uint8_t* out, const uint8_t* in, int in_size)
{
	int x;

	for (x = 0; x < in_size/2; x++, in += 2, out += 3)
	{

	        out[0] = (in[1] & 248);
	        out[1] = (((in[0] & 224) >> 3) + (in[1] << 5));
	        out[2] = (in[0] << 3);
	}
}

static void DUMB_to_RGB3 (uint8_t* out, const uint8_t* in, int in_size)
{
	int x;

	for (x = 0; x < in_size/2; x++, in += 2, out += 3)
	{

	        out[0] = in[0];
	        out[1] = in[1];
	        out[2] = 0;
	}
}

static void YUYV_to_RGB3 (uint8_t* out, const uint8_t* in, int in_size)
{

	int x;
	int Y1, U, Y2, V;
	int c, d, e;

	for (x = 0; x < in_size/4; x++, in += 4, out += 6)
	{

	        Y1 = in[0];
		U = in [1];
		Y2 = in [2];
		V = in [3];

		c = Y1 - 16;
		d = U - 128;
		e = V - 128;

	        out[0] = CYUYV2R(c, e);
	        out[1] = CYUYV2G(c, d, e);
	        out[2] = CYUYV2B(c, d);
		
		c = Y2 - 16;

	        out[0] = CYUYV2R(c, e);
	        out[1] = CYUYV2G(c, d, e);
	        out[2] = CYUYV2B(c, d);
	}
}

//Creates the buffers in memory to hold frame data (need to negotiate with device for true buffer size)
static void init_buffers (void)//unsigned int buffer_size)
{
        in_buff = calloc(1, sizeof(*in_buff)); //Allocates 1 pointer size of memory and sets it to zero (malloc doesn't set to zero)

        if (!in_buff)
	{
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }

        in_buff[0].length = IN_BUFF_SIZE;  //Sets length variable to the size of the buffer
        in_buff[0].start = malloc(IN_BUFF_SIZE); //Allocate memory to the size required

        if (!in_buff[0].start)
	{
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }

        out_buff = calloc(1, sizeof(*out_buff)); //Allocates 1 pointer size of memory and sets it to zero (malloc doesn't set to zero)

        if (!in_buff)
	{
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }

        out_buff[0].length = OUT_BUFF_SIZE;  //Sets length variable to the size of the buffer
        out_buff[0].start = malloc(OUT_BUFF_SIZE); //Allocate memory to the size required

        if (!out_buff[0].start)
	{
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
        }
}


static void image_read (int size)
{
	size_t total_read = 0;
	size_t total_left  = size; //remaining length to send
	char *buffer_pointer = in_buff[0].start; //cursor position in data

	while (total_left > 0)
	{
		ssize_t current = read(sd, buffer_pointer, total_left);

		if (current <= 0)
		{
			if (current < 0)
			{
				switch (errno) 
				{
					case EAGAIN:
						return;
					case EIO:
		                       		//Could ignore EIO, see spec.
			                        //fall through
					default:
						errno_exit("read");
				}
			}
		}
		else
		{
			total_read += current;
       			total_left -= current;
			buffer_pointer += current;
		}
  	}

	//if (-1 == read(sd, in_buff[0].start, in_buff[0].length))
		//performs read, if result is a fail perform following:
		

}

/*
static void test_file (void)
{
	fprintf (stderr, "open imgfile\n");
	fp=fopen("/home/daniel/A10/video_streaming/yuv_pic", "r");
	fprintf (stderr, "seeking to place 1\n");
	if (-1 == fseek (fp, 1, SEEK_SET))
	{
		errno_exit ("Can not seek to place 1 in test file");
	}
	fprintf (stderr, "reading file data\n");
	if (0 == fread (in_buff[0].start, 1, in_buff[0].length, fp))
	{
		errno_exit ("Can not read test file");
	}
	fprintf (stderr, "closing imgfile\n");
	fclose(fp);
}*/

//Need to negotiate with device on image size and format (use to decode and size buffer)
//Need to let device know when socket will be closed?
int main()
{
	int loopIsInfinite = 1;
	fprintf(stderr, "initializing buffer\n");
	init_buffers();
	//fprintf(stderr, "opening test file\n");
	//test_file ();
	fprintf (stderr, "opening network connection\n");
	init_net ();
	fprintf (stderr, "starting receive loop\n");
	while (loopIsInfinite)
	{
		//fprintf (stderr, "reading stream data\n");
		image_read (IN_BUFF_SIZE);
		//fprintf(stderr, "parsing file data\n");
		if (RGBR)
			RGBR_to_RGB3 (out_buff[0].start, in_buff[0].start, in_buff[0].length);
		if (RGBP)
			RGBP_to_RGB3 (out_buff[0].start, in_buff[0].start, in_buff[0].length);
		if (YUVP)
			YUVP_to_RGB3 (out_buff[0].start, in_buff[0].start, in_buff[0].length);
		if (DUMB)
			DUMB_to_RGB3 (out_buff[0].start, in_buff[0].start, in_buff[0].length);
		if (YUYV)
			YUYV_to_RGB3 (out_buff[0].start, in_buff[0].start, in_buff[0].length);
		//fprintf(stderr, "sending image data\n");
		send_image ();
	}
	return(0);
}
