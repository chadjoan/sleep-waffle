
#include <zeo_connection.h>
#include <zeo_malloc.h>

static void grow_conn_ids(zeo_serial_port *port, size_t amount)
{
	port->n_conn_ids += amount;
	port->conn_id_used = zeo_realloc( port->conn_id_used, port->n_conn_ids);
	port->packet_cursors = zeo_realloc( port->packet_cursors, port->n_conn_ids);
}

static zeo_serial_conn_id alloc_conn_id(zeo_serial_port *port)
{
	size_t i;
	for ( i = 0; i < port->n_conn_ids; i++ )
	{
		if ( !port->conn_id_used[i] )
			return i;
	}
	
	/* If we're here, then we couldn't just reuse one. */
	/* So we'll have to make more connection IDs. */
	grow_conn_ids(port,1);
	return port->n_conn_ids - 1; /* Return the newly allocated conn_id. */
}

void connect_zeo_port( zeo_serial_port *port, zeo_connection *connection )
{
	/* Allocate a connection ID */
	connection->conn_id = alloc_conn_id(port);
	
}
