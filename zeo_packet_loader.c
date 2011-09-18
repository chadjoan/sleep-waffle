/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

#include <zeo_packet.h>
#include <zeo_packet_loader.h>


#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 0
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN    1
#endif

int machine_endianness()
{
   int i = 1;
   char *p = (char *) &i;
   if (p[0] == 1) // Lowest address contains the least significant byte
      return LITTLE_ENDIAN;
   else
      return BIG_ENDIAN;
}

void swap_endian( uint16_t *var )
{
	*var = (*var << 8) | ((*var >> 8) & 0xFF);
}

void init_zeo_packet_loader( zeo_packet_loader *loader )
{
	/* loader->packet       = NULL; // Don't do this. The caller may not expect it. */
	loader->packet_len   = sizeof(zeo_header);
	loader->bytes_loaded = 0;
	loader->err_str      = NULL;
	loader->err_len      = 0;
}

static void parse_header( zeo_packet_loader *loader )
{
	zeo_header *header_ptr = &(loader->packet->header);
	uint8_t *header_buf = (uint8_t*)header_ptr;
	
	/* Make sure the byte-order is correct. */
	if ( machine_endianness() == BIG_ENDIAN )
	{
		swap_endian(&header_ptr->message_length);
		swap_endian(&header_ptr->message_length_inverse);
		//swap_endian(&header_ptr->subsecond);
	}
	
	/* Valid Zeo packets start with 'A'. */
	if ( header_ptr->start_char != 'A' )
	{
		loader->err_str = "Not a Zeo packet: does not start with 'A' character.";
		loader->err_len = strlen(loader->err_str);
		return;
	}
	
	/* Message length and its invserse must match. */
	if ( (0xFFFF & header_ptr->message_length) != 
		 (0xFFFF & ~header_ptr->message_length_inverse ) )
	{
		loader->err_str = "Packet corrupted: could not determine packet size.";
		loader->err_len = strlen(loader->err_str);
		return;
	}
	
	/* For some reason Zeo packets have a max length. */
	if ( header_ptr->message_length > 257 )
	{
		loader->err_str = "Packet too big: data block is large than 256 bytes.";
		loader->err_len = strlen(loader->err_str);
		return;
	}

	/* Just loaded the message_length field. */
	/* This allows us to update the packet length. */
	/* The message_length is defined as the size of the "data block */
	/*   plus the identifier." */
	/* packet_len is here defined as the data block plus the header. */
	/* Since both the message_length and the header account for the */
	/*   identifier, we have to subtract 1 to dedup its contribution. */
	loader->packet_len = header_ptr->message_length + sizeof(zeo_header) - 1;
}

int accumulate_zeo_packet( zeo_packet_loader *loader, void *buffer, size_t n_bytes )
{
	size_t bytes_loaded_this_call = 0;
	
	if ( n_bytes == 0 )
		return 0;
	
	loader->err_str = NULL;
	loader->err_len = 0;
	
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
		void *header_buf;
		//size_t copy_size = sizeof(zeo_header) - loader->bytes_loaded;
		size_t copy_max  = sizeof(zeo_header) - loader->bytes_loaded;
		size_t copy_size = n_bytes;
		if ( copy_size > copy_max )
			copy_size = copy_max;
		
	printf("%d\n",__LINE__);
		header_ptr = &(loader->packet->header);
		header_buf = (void*)header_ptr + loader->bytes_loaded;
		memcpy( header_buf, buffer, copy_size );
		
	printf("%d\n",__LINE__);
	printf("copy_size == %ld\n",copy_size);
	printf("copy_max  == %ld\n",copy_max);
		loader->bytes_loaded += copy_size;
		
		/* Update these before we possibly attempt to load more bytes into */
		/*   the data part of the packet. */
		buffer += copy_size;
		n_bytes -= copy_size;
		bytes_loaded_this_call += copy_size;
		
	printf("%d\n",__LINE__);
		if ( loader->bytes_loaded >= sizeof(zeo_header) )
		{
			parse_header(loader);
			if ( loader->err_len > 0 )
				return bytes_loaded_this_call;
		}
	}
	
	printf("%d\n",__LINE__);
	if ( loader->bytes_loaded >= sizeof(zeo_header) )
	{
		printf("buffer    == %ld\n", (size_t)buffer);
		printf("n_bytes   == %ld\n", n_bytes);
		printf("bytesLoad == %ld\n", loader->bytes_loaded);
		printf("headerSiz == %ld\n", sizeof(zeo_header));
		size_t data_len = loader->bytes_loaded - sizeof(zeo_header);
		size_t data_max = loader->packet->header.message_length - 1;
		size_t new_len = data_len + n_bytes;
		printf("new_len   == %ld\n", new_len);
		if ( new_len > data_max )
			new_len = data_max;
		printf("new_len   == %ld\n", new_len);
		printf("data_len  == %ld\n", data_len);
		printf("data_max  == %ld\n", data_max);
		printf("pckt_len  == %ld\n", loader->packet_len );
		printf("msg_len   == %d\n",  loader->packet->header.message_length );
		
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
				char packet_str[65536];
				zeo_packet_to_string(loader->packet, packet_str, 65536);
	printf("Packet dump:\n%s\n",packet_str);
	/* TODO:
	if ( loader->bytes_loaded == loader->packet_len )
	{
		// check this.
		loader->packet.checksum
	}
	*/
	
	return bytes_loaded_this_call;
}