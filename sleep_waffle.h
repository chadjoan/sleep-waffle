/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */

/* This is THE public API header.  #include this to gain access to the */
/*   functions and types that should be called by code using the Sleep Waffle */
/*   library. */


#ifndef SLEEP_WAFFLE_INCLUDED
#define SLEEP_WAFFLE_INCLUDED

#include <stdint.h>
#include <stdlib.h>

#include <zeo_event.h>
#include <zeo_splice.h>

/*******************************************************************************
 * General configuration stuff.
 ******************************************************************************/
enum zeo_language_option
{
	zeo_lang_c, /* C semantics.  Manual memory management. */
	zeo_lang_d, /* D semantics.  Garbage collected; D's gc is called. */
	/* zeo_lang_python, ... it could happen if someone cares to do it. */
	/* etc... */
};

/* If using Sleep Waffle from a language other than C, this should be set to */
/*   a zeo_language_option matching that language. */
/* TODO: this should probably be made thread-local whenever possible. */
/* (This is declared as uint32_t instead of zeo_language_option for the sake */
/*   of providing a consistant, platform-independent, declaration for */
/*   other languages that may bind to Sleep Waffle.) */
extern uint32_t zeo_language_compat;

/*******************************************************************************
 * Connectivity.
 ******************************************************************************/

/* TODO: does reading from a port deprive other listeners of that input? */
/*       If so, a zeo_broadcast_deamon should be designed that offers a set */
/*         of virtual serial ports that this library will recognize and */
/*         prefer over direct access. */

typedef struct S_SW_SERIAL_PORT
{
	/* Whether or not this port is known to have a zeo connection. */
	/* This is only set if sw_check_port_zeo_connectivity(port) is called */
	/*   on the enclosing port AND there is a zeo device on the other end */
	/*   that is willing to talk. */
	uint32_t is_zeo_port;
	
	/* Device node path on Linux. */
	char *identifer;
	
} sw_serial_port;

/* This will listen to the given port and look for Zeo packets. */
/* If it does not find n_packets within timeout milliseconds, then 0 is */
/*   returned and port's is_zeo_port member is set to 0. */
/* If it finds n_packets within timeout milliseconds, then 1 is returned */
/*   and port's is_zeo_port member is set to 1. */
uint32_t sw_check_port_zeo_connectivity( sw_serial_port *port, size_t n_packets, uint32_t timeout );

/* Similar to sw_check_port_zeo_connectivity(), except that it is done on */
/*   all ports in the given array in parallel. */
/* This is the prefered way to detect which ports might have Zeos attached */
/*   if there are multiple serial ports. */
void sw_check_zeo_connectivity(
	sw_serial_port ports[],
	size_t ports_len,
	size_t n_packets,
	uint32_t timeout );

/* Lists all serial ports on the system. */
uint32_t sw_list_serial_ports( sw_serial_port *ports[] );

void sw_free_port( sw_serial_port *port );
void sw_free_ports( sw_serial_port ports[], size_t ports_len );

typedef struct S_ZEO_CONNETION
{
	sw_serial_port *port;
	
	
} zeo_connection;


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