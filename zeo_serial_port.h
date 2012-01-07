
#ifndef ZEO_SERIAL_PORT_INCLUDED
#define ZEO_SERIAL_PORT_INCLUDED

#include <zeo_packet.h>
/* TODO: does reading from a port deprive other listeners of that input? */
/*       If so, a zeo_broadcast_deamon should be designed that offers a set */
/*         of virtual serial ports that this library will recognize and */
/*         prefer over direct access. */

/* The type of a connection id. */
#define zeo_serial_conn_id uint32_t

/* Serial port /WILL NOT/ handle multithreading very well. */
/* Do not spread multiple connections to a single serial port accross multiple
 *   threads.  It is asking for trouble. */
/* Patches accepted. */
/* Multithreading for this actually has a couple of tricky design issues: */
/* - It must be portable. */
/* - It must be boost-compatible. */
/* - Will the threading API used here work consistently with whatever threading
 *     API a caller's other language might be using? (ex: D's threading API) */
/* Some cursory google searching suggests that OpenMP might work. */


/* The serial port serves as a layer between connections to the Zeo and the */
/* Zeo itself.  This allows for multiple connections to a single Zeo, as well */
/* as the ability to identify which serial ports actually have a Zeo behind */
/* them. */
/* Example:                                                                   */
/*   zeo_connection first_available_conn;                                     */
/*   zeo_serial_ports ports[];                                                 */
/*   size_t n_ports = zeo_list_serial_ports(&ports);                           */
/*   zeo_check_zeo_connectivity(ports, n_ports, 10, 1000);                     */
/*   size_t i;                                                                */
/*   for ( i = 0; i < n_ports; i++ )                                          */
/*   {                                                                        */
/*       if ( ports[i].is_zeo_port )                                          */
/*       {                                                                    */
/*           connect_zeo_port( &ports[i], first_available_conn );             */
/*           break;                                                           */
/*       }                                                                    */
/*   }                                                                        */
/*   ... etc ...                                                              */
/*   zeo_free_ports(ports, n_ports);                                           */

typedef struct S_ZEO_SERIAL_PORT
{
	/* Whether or not this port is known to have a zeo connection. */
	/* This is only set if zeo_check_port_zeo_connectivity(port) is called */
	/*   on the enclosing port AND there is a zeo device on the other end */
	/*   that is willing to talk. */
	uint32_t is_zeo_port;
	
	/* Device node path on Linux. */
	char *identifier;
	size_t identifier_len;
	
	/* The number of connection ids.  This is NOT the same as the number of
	 *   active connections. */
	size_t n_conn_ids;
	
	/* This is an array indexed by connection id. */
	/* If an element is 1, it indicates that the given connection id has a 
	 *   connection associated with it. */
	/* If an element is 0, it indicates that the given connection id is not
	 *   used.  This can happen because conn_id_used may be over-allocated or 
	 *   whenever connections are disconnected from the port. */
	uint8_t *conn_id_used;
	
	/* Packet queue.  A circular queue with multiple cursors. */
	zeo_packet *packet_queue;
	size_t queue_capacity;
	size_t queue_front_index;
	size_t queue_back_index;
	
	/* Packet cursors array; indexed by connection id. */
	size_t *packet_cursors;
	
} zeo_serial_port;

/* Causes the serial port to pull any Zeo packets that might be waiting. */
void zeo_poll_serial( zeo_serial_port *port );

/* Returns the next packet for the given connection id in the queue on the 
 *   given port */
/* This will increment the given connection id's cursor in the port's packet
 *   queue. */
/* If there are no more packets left in the queue, this returns NULL. */
/* The returned packet reference is a pointer into the port's queue.  This is
 *   done to avoid writing excess copying into the API.  When zeo_get_packet is
 *   called again, that packet may be deleted.  This is still true if the 
 *   second call to zeo_get_packet returns NULL because the queue was empty.
 *   Packet deletion happens if the given connection is the farthest behind in 
 *   the queue and removing that packet causes the queue to shrink.  If you 
 *   need to persist the packet longer than the next call to zeo_get_packet(), 
 *   then make a (deep) copy. */
const zeo_packet *zeo_get_packet( zeo_serial_port *port, zeo_serial_conn_id conn_id );

/* This will listen to the given port and look for Zeo packets. */
/* If it does not find n_packets within timeout milliseconds, then 0 is */
/*   returned and port's is_zeo_port member is set to 0. */
/* If it finds n_packets within timeout milliseconds, then 1 is returned */
/*   and port's is_zeo_port member is set to 1. */
uint32_t zeo_check_port_zeo_connectivity( zeo_serial_port *port, size_t n_packets, uint32_t timeout );

/* Similar to zeo_check_port_zeo_connectivity(), except that it is done on */
/*   all ports in the given array in parallel. */
/* This is the prefered way to detect which ports might have Zeos attached */
/*   if there are multiple serial ports. */
void zeo_check_zeo_connectivity(
	zeo_serial_port ports[],
	size_t ports_len,
	size_t n_packets,
	uint32_t timeout );

/* Lists all serial ports on the system. */
uint32_t zeo_list_serial_ports( zeo_serial_port *ports[] );

/* Frees all resources associated with the given port(s). */
void zeo_free_port( zeo_serial_port *port );
void zeo_free_ports( zeo_serial_port ports[], size_t ports_len );

/* TODO: where does this go? */
typedef struct S_ZEO_PACKET_CURSOR
{
	uint64_t packet_number;
	uint32_t last_timestamp;
	
} zeo_packet_cursor;

#endif