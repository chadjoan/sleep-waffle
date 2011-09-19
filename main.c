#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <string.h>
#include <zeo_packet.h>
#include <zeo_packet_loader.h>

int main(int argc,char** argv)
{
	struct termios tio, oldtio;
	/*struct termios stdio;*/
	int tty_fd;
	/*fd_set rdset;

	unsigned char c='D';*/

/*
	printf("Please start with %s /dev/ttyS1 (for example)\n",argv[0]);
	memset(&stdio,0,sizeof(stdio));
	stdio.c_iflag=0;
	stdio.c_oflag=0;
	stdio.c_cflag=0;
	stdio.c_lflag=0;
	stdio.c_cc[VMIN]=1;
	stdio.c_cc[VTIME]=0;
	tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
	tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking
*/
	zeo_packet zp;
	zeo_packet_loader zp_loader;
	int n_bytes_read, n_bytes_written, sync_mode;

	init_zeo_packet(&zp);
	init_zeo_packet_loader(&zp_loader);
	zp_loader.packet = &zp;
	
	sync_mode = 1;


	memset(&tio,0,sizeof(tio));
	tio.c_iflag=0;
	tio.c_oflag=0;
	tio.c_cflag= CS8 | CREAD;
	tio.c_lflag=0;
	tio.c_cc[VMIN]=1;
	tio.c_cc[VTIME]=0;

	printf("argc == %d\n",argc);
	if ( argc == 2 )
		tty_fd=open(argv[1], O_RDONLY | O_NONBLOCK);
	else
		tty_fd=open("/dev/ttyUSB0", O_RDONLY | O_NONBLOCK);
	
	tcgetattr(tty_fd,&oldtio); /* save current port settings */
	
	cfsetispeed(&tio,B38400);
	tcsetattr(tty_fd,TCSANOW,&tio);
	
	char buf[255];
	char packet_out[4096];
	int done = 0;
	while (!done)
	{
		printf(".\n");
		//usleep(100000);
		usleep(1000000);
		
		n_bytes_read = read(tty_fd,buf,255);
		if (n_bytes_read <= 0)
			continue;
		
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
		
		/* TODO: scan_for_packets(...) */
		if ( sync_mode )
		{
			/* BUG: what if the 'A' is coincidental, and the packet failed.
			 *   This code will be unable to seek back to the character directly
			 *     after the 'A' and continue scanning.  Use a scan_for_packets
			 *     function to deal with this.  A context holder will probably
			 *     be necessary, and will probably be a serial_port or
			 *     zeo_connection object. */
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
		
		//if (read(tty_fd,&c,1)>0)        write(STDOUT_FILENO,&c,1);              // if new data is available on the serial port, print it out
		/*if (read(STDIN_FILENO,&c,1)>0)  write(tty_fd,&c,1);                     // if new data is available on the console, send it to the
		 */
		//serial port
	}

	/* restore old port settings */
	tcsetattr(tty_fd,TCSANOW,&oldtio);
	
	/* cleanup. */
	close(tty_fd);
}

/* TODO: scan_for_packets(...) */

