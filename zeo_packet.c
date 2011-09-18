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
#include <zeo_enum.h>
		
#define GENERATE_ZEO_ENUM_CASE(ID, VAR, NAME, DESC) \
	ZEO_ENUM_STRINGIZER_CASE(VAR,NAME)
ZEO_ENUM_STRINGIZER_START(zeo_packet_type_to_string)
	ZEO_PACKET_TYPES(GENERATE_ZEO_ENUM_CASE)
ZEO_ENUM_STRINGIZER_END("Can not look up packet type name: invalid packet type.")
#undef GENERATE_ZEO_ENUM_CASE


#define GENERATE_ZEO_ENUM_CASE(ID, VAR, NAME, DESC) \
	ZEO_ENUM_STRINGIZER_CASE(VAR,DESC)
ZEO_ENUM_STRINGIZER_START(zeo_packet_type_to_desc)
	ZEO_PACKET_TYPES(GENERATE_ZEO_ENUM_CASE)
ZEO_ENUM_STRINGIZER_END("Can not look up packet type description: invalid packet type.")
#undef GENERATE_ZEO_ENUM_CASE

void init_zeo_packet( zeo_packet *packet )
{
	// etc
	memset(&(packet->header), '\0', sizeof(zeo_header));
	packet->data = NULL;
}

int zeo_packet_to_string( zeo_packet *packet, char *buf, int len )
{
	return snprintf(
		buf,
		len,
		"%02ld : start_char : %X\n"
		"%02ld : protocol_version : %c\n"
		"%02ld : checksum : %d\n"
		"%02ld : message_length : %u\n"
		"%02ld : message_length         (hex) : %04X\n"
		"%02ld : message_length_inverse (hex) : %04X\n"
		"%02ld : unix_time : %d\n"
		"%02ld : subsecond : %d\n"
		"%02ld : sequence_number : %d\n"
		"%02ld : data_type : %d = %s\n"
		"%02ld : data :\n%s\n",
		offsetof(zeo_header, start_char),             packet->header.start_char,
		offsetof(zeo_header, protocol_version),       packet->header.protocol_version,
		offsetof(zeo_header, checksum),               packet->header.checksum,
		offsetof(zeo_header, message_length),         packet->header.message_length,
		offsetof(zeo_header, message_length),         packet->header.message_length,
		offsetof(zeo_header, message_length_inverse), packet->header.message_length_inverse,
		offsetof(zeo_header, unix_time),              packet->header.unix_time,
		offsetof(zeo_header, subsecond),              packet->header.subsecond,
		offsetof(zeo_header, sequence_number),        packet->header.sequence_number,
		offsetof(zeo_header, data_type),              packet->header.data_type,
			zeo_packet_type_to_string(packet->header.data_type, NULL),
		offsetof(zeo_packet, data),                   (char*)packet->data);
}
