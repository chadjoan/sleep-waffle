/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */

#ifndef ZEO_PACKET_HEADER_INCLUDED
#define ZEO_PACKET_HEADER_INCLUDED

#include <stdint.h>

#include <zeo_packet.h>

typedef struct S_ZEO_PACKET_LOADER
{
	zeo_packet *packet;

	/***********************************************************************
	* How many bytes must be loaded before the packet
	*   is done being loaded.
	* Starts at 12, grows when length is known.
	***********************************************************************/
	size_t packet_len;   

	/***********************************************************************
	* Number of bytes that have been loaded.
	* We're done when this equals packet_len.
	***********************************************************************/
	size_t bytes_loaded;
	
	/***********************************************************************
	* String of the last error, if any. NULL if there
	*   have been no unhandled errors.
	***********************************************************************/
	char *err_str;

	/* The length of err_str. */
	size_t err_len;
	
} zeo_packet_loader;

/*******************************************************************************
* Call this on any 'zeo_packet_loader' instance before using it.
* This function will not alter the 'packet' member of the loader instance.
*******************************************************************************/
void init_zeo_packet_loader( zeo_packet_loader *loader );

/*******************************************************************************
* Loads 'n_bytes' bytes from 'buffer' and into 'loader'.  
* Returns the number of bytes loaded.
* The number of bytes loaded can be less than the number of bytes input when
*   the packet finishes loading, or when there is an error.
* If 0 bytes are loaded, it means an error has occured.
* Instead of looking for the the return value to be less than n_bytes, 
*   the recommended method for checking completion is to look for when
*   loader->bytes_loaded == loader->packet_len.
*******************************************************************************/
int accumulate_zeo_packet( zeo_packet_loader *loader, void *buffer, size_t n_bytes );

#endif