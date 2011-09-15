/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */

/* The code for setting up termios originally came from 
 * http://tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html */

#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

#include <string.h>
#include <zeo_packet.h>

#define BAUDRATE B38400
/* #define BAUDRATE 38400 */

volatile int STOP=0; 

void signal_handler_IO (int status);   /* definition of signal handler */
int wait_flag=1;                    /* TRUE while no signal received */

int main(int argc, char *argv[])
{
	/*FILE *ifp, *ofp;

	ifp = fopen("/dev/ttyUSB0", "r");

	if (ifp == NULL) {
		fprintf(stderr, "Can't open input file in.list!\n");
		return 1;
	}*/

	zeo_packet zp;
	zeo_packet_loader zp_loader;
	int fd,c, n_bytes_read, n_bytes_written, sync_mode;
	struct termios oldtio,newtio;
	struct sigaction saio;           /* definition of signal action */
	char buf[255];
	char packet_out[4096];

	/* open the device to be non-blocking (read will return immediatly) */
	fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd <0) {perror("/dev/ttyUSB0"); return 1; }

	/* install the signal handler before making the device asynchronous */
	saio.sa_handler = signal_handler_IO;
	sigemptyset(&saio.sa_mask);
	saio.sa_flags = 0;
	saio.sa_restorer = NULL;
	sigaction(SIGIO,&saio,NULL);

	/* allow the process to receive SIGIO */
	fcntl(fd, F_SETOWN, getpid());
	/* Make the file descriptor asynchronous (the manual page says only 
	O_APPEND and O_NONBLOCK, will work with F_SETFL...) */
	fcntl(fd, F_SETFL, FASYNC);

	tcgetattr(fd,&oldtio); /* save current port settings */
	/* set new port settings for canonical input processing */
	/*newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;*/
	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	/*newtio.c_iflag = IGNPAR | ICRNL | ICANON;*/
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	/*newtio.c_lflag = ICANON; */
	newtio.c_cc[VMIN]=1;
	newtio.c_cc[VTIME]=0;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);

	init_zeo_packet(&zp);
	init_zeo_packet_loader(&zp_loader);
	zp_loader.packet = &zp;
	
	sync_mode = 1;
	
	/* loop while waiting for input. normally we would do something
	useful here */ 
	while (STOP==0) {
		printf(".\n");
		usleep(100000);
		/* after receiving SIGIO, wait_flag = FALSE, input is available
			and can be read */
		if (wait_flag==0) { 
			n_bytes_read = read(fd,buf,255);
			buf[n_bytes_read]=0;
			
			/*if ( n_bytes_read >= sizeof(zeo_header) )
			{
				memcpy( &zp, buf, sizeof(zeo_header) );
				zp.data = buf + sizeof(zeo_header);
				
				n_bytes_written = zeo_packet_to_string( &zp, packet_out, 4096 );
				packet_out[n_bytes_written] = '\0';
				printf("%s", packet_out);
			}
			else
			{*/
				int i;
				for ( i = 0; i < n_bytes_read; i++ )
					printf("%02x ", (uint8_t)buf[i]);
				printf("\n");
			/*}*/
			
			if ( sync_mode )
			{
				/* Look for the 'A' character that starts all Zeo packets. */
				for ( i = 0; i < n_bytes_read; i++ )
				{
					if ( buf[i] == 'A' )
					{
						printf("Found packet start.\n");
						printf("Accumulating %d bytes into packet.\n",n_bytes_read-i);
						accumulate_zeo_packet(&zp_loader, buf+i, n_bytes_read-i);
						printf("Done\n");
						sync_mode = 0;
						break;
					}
				}
			}
			else
			{
						printf("Accumulating %d bytes into packet.\n",n_bytes_read);
				accumulate_zeo_packet(&zp_loader, buf, n_bytes_read);
						printf("Done\n");
			}
				
			if ( zp_loader.err_str != NULL )
			{
				printf("Zeo Error: %s\n",zp_loader.err_str);
				
				init_zeo_packet(&zp);
				init_zeo_packet_loader(&zp_loader);
				zp_loader.packet = &zp;
				sync_mode = 1;
			}
			else if ( zp_loader.bytes_loaded == zp_loader.packet_len )
			{
				n_bytes_written = zeo_packet_to_string( &zp, packet_out, 4096 );
				packet_out[n_bytes_written] = '\0';
				printf("%s", packet_out);
				
				init_zeo_packet(&zp);
				init_zeo_packet_loader(&zp_loader);
				zp_loader.packet = &zp;
				sync_mode = 1;
			}
			
			/*if (n_bytes_read==1) STOP=1; /* stop loop if only a CR was input */
			wait_flag = 1;      /* wait for new input */
		}
	}
	/* restore old port settings */
	tcsetattr(fd,TCSANOW,&oldtio);
	
	return 0;
}
        
/*******************************************************************************
* signal handler. sets wait_flag to FALSE, to indicate above loop that         *
* characters have been received.                                               *
*******************************************************************************/

void signal_handler_IO (int status)
{
	printf("received SIGIO signal.\n");
	wait_flag = 0;
}
    

/*
Returns 1 if 'c' is one of the characters in 'delimiters'.
Otherwise, returns 0.
*/
int isOneOf( char c, const char* delimiters )
{
	int i = 0;
	while ( delimiters[i] != '\0' )
	{
		if ( c == delimiters[i] )
			return 1;
		
		i++;
	}
	
	return 0;
}

/*
Scans 'str' for instances of any characters found in 'delimiters', and upon 
  reaching the first instance calculates the location of the next character
  that is not in 'delimiters'.
'slen' is the length of 'str' and no characters in 'str' will be read once
  the scanner is that far in.
The return value is the length of the substring just before the first
  instance of something in 'delimiters', and *tail is made to point to the
  string beginning just after the first instances of 'delimiters'.
If 'slen' is 0, then the return value will be 0 and *tail will be 0.
If 'slen' is nonzero but there are no characters from 'delimiters' to be
  found in 'str', then the return value will be the same as 'slen' and *tail 
  will be 0.
*/
size_t split( const char *str, int slen, const char *delimiters, char **tail )
{
	if ( slen == 0 )
	{
			*tail = 0;
			return 0;
	}
	
	int hlen = 0;
	int i;
	for ( i = 0; i < slen; i++ )
	{
		if ( isOneOf(str[i], delimiters) )
		{
				hlen = i;
				break;
		}
	}
	
	if ( hlen == 0 )
	{
		*tail = 0;
		hlen = slen;
	}
	else
	{
		for ( ; i < slen; i++ )
		{
			if ( !isOneOf(str[i], delimiters) )
			{
					*tail = (char*)(str + i);
					break;
			}
		}
	}
	
	return hlen;
}


