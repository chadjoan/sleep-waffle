

#ifndef ZEO_CONNETION_INCLUDED
#define ZEO_CONNETION_INCLUDED

#include <zeo_serial_port.h>
#include <zeo_event.h>
#include <zeo_splice.h>


typedef struct S_ZEO_CONNETION
{
	zeo_serial_port *port;
	zeo_serial_conn_id conn_id;
	
	
} zeo_connection;

void connect_zeo_port( zeo_serial_port *port, zeo_connection *connection );


/*******************************************************************************
 * Message and polling related definitions.
 ******************************************************************************/
enum zeo_message_type
{
	zeo_msg_type_splice,
	zeo_msg_type_event,
};

#pragma pack(push,1)
typedef struct S_ZEO_MESSAGE
{
	union
	{
		zeo_splice *splice;
		zeo_event  *event;
	};
	
	/* The type of contents in this message. */
	/* See zeo_message_type for possible values. */
	/* (This is declared as uint32_t instead of zeo_message_type for the sake */
	/*   of providing a consistent, platform-independent, declaration for */
	/*   other languages that may bind to Sleep Waffle.) */
	uint32_t type;

} zeo_message;
#pragma pack(pop)

/* Polls the given connection for zeo messages. */
/* If there are any messages pending, the oldest one will be placed into the */
/*   given zeo_message struct. */
/* The caller is responsible for free'ing the splice/event object that */
/*   populates the message struct. */
/* Returns 1 if there was a message pending, or 0 if the call found nothing. */
uint32_t zeo_poll( zeo_connection *conn, zeo_message *message );

#endif
