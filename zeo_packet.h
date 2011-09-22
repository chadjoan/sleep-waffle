/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */

#ifndef ZEO_PACKET_INCLUDED
#define ZEO_PACKET_INCLUDED

#include <stdint.h>
#include <stdlib.h>

#define ZEO_PACKET_TYPES(apply) \
    apply(0x00, zeo_event,          "Event",                "An event has occured.") \
    apply(0x02, zeo_slice_end,      "Slice End",            "Marks the end of a slice of data.") \
    apply(0x03, zeo_version,        "Version",              "Version of the raw data output") \
	apply(0x80, zeo_waveform,       "Waveform",             "Raw time domain brainwave") \
	apply(0x83, zeo_frequency_bins, "Frequency Bins",       "Frequency bins derived from waveform") \
	apply(0x84, zeo_sqi,            "Signal Quality Index", "Signal Quality Index of waveform (0-30)") \
	apply(0x8A, zeo_timestamp,      "Timestamp",            "Timestamp from Zeo’s RTC") \
	apply(0x97, zeo_impedance,      "Impedance",            "Impedance across the headband") \
	apply(0x9C, zeo_bad_signal,     "Bad Signal",           "Signal contains artifacts") \
	apply(0x9D, zeo_sleep_stage,    "Sleep Stage",          "Current 30sec sleep stage")

/* Packet enumeration utilities. */
#define ZEO_ENUM_VAR_ID(ID, VAR, NAME, DESC) VAR = ID,
enum zeo_packet_type { ZEO_PACKET_TYPES(ZEO_ENUM_VAR_ID) };
#undef ZEO_ENUM_VAR_ID

char *zeo_packet_type_to_string(int packet_id, size_t *len);
char *zeo_packet_type_to_desc  (int packet_id, size_t *len);

#pragma pack(push,1)
typedef struct S_ZEO_HEADER
{
	/* Zeo packet header. */
	char start_char;       /* Always 'A' (for now). */
	char protocol_version; /* '0' through '9' (or higher?) */
	uint8_t  checksum;
	uint16_t message_length; /* Includes this header info. */
	uint16_t message_length_inverse; /* Used as a redundancy check */
	//uint8_t  _padding_1; /* strange... I seem to get better results with this here. */
	uint8_t  unix_time; /* The lower 8 bits of Zeo's Unix time. */
	uint16_t subsecond; /* Runs from 0x0000 through 0xFFFF in 1 second. */
	uint8_t  sequence_number;
	uint8_t  data_type;
	
} zeo_header;
#pragma pack(pop)

typedef struct S_ZEO_PACKET
{
	/* Zeo packet header. */
	zeo_header header;
	
	/***********************************************************************
	* The data itself.  The length is message_length - 1.
	* This is dynamically allocated.
	* The caller is responsible for free'ing it.
	***********************************************************************/
	char *data;
	
} zeo_packet;

/*******************************************************************************
* Data block types.
*******************************************************************************/
/* These might be pointless, depending on how the splice/event loading code works. */
#if 0
typedef struct S_ZEO_EVENT
{
	int32_t type;
} zeo_event;

typedef struct S_ZEO_SPLICE_END
{
	/* The number of packets in the splice. */
	int32_t n_packets;
} zeo_splice_end;

typedef struct S_ZEO_PROTOCOL
{
	int32_t protocol_version;
} zeo_protocol;

typedef struct S_ZEO_WAVEFORM
{
	int16_t samples[128];
} zeo_waveform;

typedef struct S_ZEO_FREQUENCY_BINS
{
	int16_t bins[7];
} zeo_frequency_bins;

typedef struct S_ZEO_SQI
{
	int32_t sqi;
} zeo_sqi;
#endif


/*******************************************************************************
* Call this on any 'zeo_packet' instance before using it.
*******************************************************************************/
void init_zeo_packet( zeo_packet *packet );

/*******************************************************************************
* Convenience function for dumping data present in packets.
*******************************************************************************/
char *bytes_to_hex( const char *bytes, size_t n_bytes, char *out_str, size_t len );

/*******************************************************************************
* Creates a string representation of the given Zeo packet.
*******************************************************************************/
int zeo_packet_to_string( zeo_packet *packet, char *buf, size_t len );

#endif
