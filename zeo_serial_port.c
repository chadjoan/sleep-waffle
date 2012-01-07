
#include <zeo_malloc.h>
#include <zeo_serial_port.h>

#if 0
	/* Whether or not this port is known to have a zeo connection. */
	/* This is only set if zeo_check_port_zeo_connectivity(port) is called */
	/*   on the enclosing port AND there is a zeo device on the other end */
	/*   that is willing to talk. */
	uint32_t is_zeo_port;
	
	/* Device node path on Linux. */
	char *identifer;
	size_t identifier_len;
	
	/* This is an array indexed by connection id. */
	/* If an element is 1, it indicates that the given connection id has a 
	 *   connection associated with it. */
	/* If an element is 0, it indicates that the given connection id is not
	 *   used.  This can happen because conn_id_used may be over-allocated or 
	 *   whenever connections are disconnected from the port. */
	uint8_t *conn_id_used;
	
	/* The number of connection ids.  This is NOT the same as the number of
	 *   active connections. */
	size_t n_conn_ids;
	
	/* Packet queue.  A circular queue with multiple cursors. */
	zeo_packet *packet_queue;
	size_t queue_capacity;
	size_t queue_front_index;
	size_t queue_back_index;
	
	/* Packet cursors array; indexed by connection id. */
	size_t *packet_cursors;
#endif
	
#define INITIAL_N_CONN_IDS 1
#define INITIAL_QUEUE_CAPACITY 16
	
static void init_serial_port(zeo_serial_port *port)
{
	size_t i;
	port->is_zeo_port = 0;
	port->identifier = NULL;
	port->identifier_len = -1;
	
	port->conn_id_used = zeo_malloc( INITIAL_N_CONN_IDS );
	port->n_conn_ids = INITIAL_N_CONN_IDS;
	for( i = 0; i < port->n_conn_ids; i++ )
		port->conn_id_used[i] = 0;
	
	port->packet_queue = zeo_malloc( INITIAL_QUEUE_CAPACITY * sizeof(zeo_packet) );
	port->queue_capacity = INITIAL_QUEUE_CAPACITY;
	port->queue_front_index = 0;
	port->queue_back_index = 0;
	
	port->packet_cursors = zeo_malloc( INITIAL_N_CONN_IDS * sizeof(size_t) );
	for( i = 0; i < port->n_conn_ids; i++ )
		port->packet_cursors[i] = 0;
	
}


uint32_t z_list_serial_ports( zeo_serial_port *ports[] )
{
	/* This is where we create serial ports. */
	/* Thus, they should be initialized here. */
	/* init_serial_port(port); */
}