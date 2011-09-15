/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <zeo_packet.h>

void init_zeo_packet( zeo_packet *packet )
{
	// etc
	memset(&(packet->header), '\0', sizeof(zeo_header));
	packet->data = NULL;
}

void init_zeo_packet_loader( zeo_packet_loader *loader )
{
	/* loader->packet       = NULL; // Don't do this. The caller may not expect it. */
	loader->packet_len   = sizeof(zeo_header);
	loader->bytes_loaded = 0;
	loader->err_str      = NULL;
	loader->err_len      = 0;
}
	
int accumulate_zeo_packet( zeo_packet_loader *loader, void *buffer, size_t n_bytes )
{
	size_t bytes_loaded_this_call = 0;
	
	if ( n_bytes == 0 )
		return 0;
	
	if ( loader->bytes_loaded == loader->packet_len )
	{
		loader->err_str = "Packet already loaded.";
		loader->err_len = strlen(loader->err_str);
		return bytes_loaded_this_call;
	}
	
	printf("accumulate_zeo_packet(%16lx,%16lx,%ld)\n",(size_t)loader,(size_t)buffer,n_bytes);
	printf("buffer == %ld\n",(size_t)buffer);
	printf("%d\n",__LINE__);
		printf("bytesLoad == %ld\n", loader->bytes_loaded);
	if ( loader->bytes_loaded < sizeof(zeo_header) )
	{
		zeo_header *header_ptr;
		size_t copy_size = sizeof(zeo_header) - loader->bytes_loaded;
		if ( copy_size < n_bytes )
			copy_size = n_bytes;
		
	printf("%d\n",__LINE__);
		header_ptr = &(loader->packet->header);
		memcpy( header_ptr, buffer, copy_size );
		
	printf("%d\n",__LINE__);
		if ( loader->bytes_loaded >= sizeof(zeo_header) )
		{
			/* Check the length value for corruption. */
			if ( header_ptr->message_length != ~header_ptr->message_length_inverse )
			{
	printf("%d\n",__LINE__);
				loader->err_str = "Packet corrupted: could not determine packet size.";
				loader->err_len = strlen(loader->err_str);
				return bytes_loaded_this_call;
			}
			
			/* Just loaded the message_length field. */
			/* This allows us to update the packet length. */
			loader->packet_len = header_ptr->message_length;
		}
		
	printf("%d\n",__LINE__);
		loader->bytes_loaded += copy_size;
		
		/* Update these before we possibly attempt to load more bytes into */
		/*   the data part of the packet. */
		buffer += copy_size;
		n_bytes -= copy_size;
		bytes_loaded_this_call += copy_size;
	}
	
	printf("%d\n",__LINE__);
	if ( loader->bytes_loaded >= sizeof(zeo_header) )
	{
		printf("buffer    == %ld\n", (size_t)buffer);
		printf("n_bytes   == %ld\n", n_bytes);
		printf("bytesLoad == %ld\n", loader->bytes_loaded);
		printf("headerSiz == %ld\n", sizeof(zeo_header));
		size_t data_len = loader->bytes_loaded - sizeof(zeo_header);
		size_t data_max = loader->packet_len - sizeof(zeo_header);
		size_t new_len = data_len + n_bytes;
		printf("new_len   == %ld\n", new_len);
		if ( new_len > data_max )
			new_len = data_max;
		printf("new_len   == %ld\n", new_len);
		
	printf("%d\n",__LINE__);
		size_t copy_size = new_len - data_len;
		
		void *new_data = NULL;
		if ( loader->packet->data == NULL )
			new_data = malloc(new_len);
		else
			new_data = realloc(loader->packet->data, new_len);

	printf("%d\n",__LINE__);
		if ( new_data == NULL )
		{
			loader->err_str = "Out of memory error.";
			loader->err_len = strlen(loader->err_str);
			return bytes_loaded_this_call;
		}
		
	printf("%d\n",__LINE__);
		printf("new_data  == %lu\n", (size_t)new_data);
		printf("data_len  == %ld\n", data_len);
		printf("copy_size == %ld\n", copy_size);
		memcpy(new_data + data_len, buffer, copy_size );
		
		loader->packet->data = new_data;
		loader->bytes_loaded += copy_size;
		
	printf("%d\n",__LINE__);
		/* Just incase we carelessly add more code below later that relies */
		/*   on these quantities being updated. */
		buffer += copy_size;
		n_bytes -= copy_size;
		bytes_loaded_this_call += copy_size;
	}
	
	printf("%d\n",__LINE__);
	/* TODO:
	if ( loader->bytes_loaded == loader->packet_len )
	{
		// check this.
		loader->packet.checksum
	}
	*/
	
	return bytes_loaded_this_call;
}

int zeo_packet_to_string( zeo_packet *packet, char *buf, int len )
{
	return snprintf(
		buf,
		len,
		"start_char : %X\n"
		"protocol_version : %c\n"
		"checksum : %d\n"
		"message_length : %u\n"
		"message_length         (hex) : %04X\n"
		"message_length_inverse (hex) : %04X\n"
		"unix_time : %d\n"
		"subsecond : %d\n"
		"sequence_number : %d\n"
		"data_type : %d\n"
		"data :\n%s\n",
		packet->header.start_char,
		packet->header.protocol_version,
		packet->header.checksum,
		packet->header.message_length,
		packet->header.message_length,
		packet->header.message_length_inverse,
		packet->header.unix_time,
		packet->header.subsecond,
		packet->header.sequence_number,
		packet->header.data_type,
		(char*)packet->data);
}
