/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */

#ifndef ZEO_PACKET_INCLUDED
#define ZEO_PACKET_INCLUDED

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

#define ZEO_EVENT_TYPES(apply) \
    apply(0x05, zeo_night_start,     "Night Start",          "User’s night has begun.") \
    apply(0x07, zeo_sleep_onset,     "Sleep Onset",          "User is asleep.") \
    apply(0x0E, zeo_headband_dock,   "Headband Docked",      "Headband returned to dock.") \
	apply(0x0F, zeo_headband_undock, "Headband Undocked",    "Headband removed from dock.") \
	apply(0x10, zeo_alarm_off,       "Alarm Off",            "User turned off the alarm.") \
	apply(0x11, zeo_alarm_snooze,    "Alarm Snooze",         "User hit snooze.") \
	apply(0x13, zeo_alarm_play,      "Alarm Play",           "Alarm is firing.") \
	apply(0x15, zeo_night_end,       "Night End",            "User’s night has ended.") \
	apply(0x24, zeo_new_headband,    "New Headband",         "A new headband ID has been read.")

/* Packet enumeration utilities. */
#define X(ID, VAR, NAME, DESC) VAR = ID,
enum zeo_packet_type { ZEO_PACKET_TYPES(X) };
#undef X

char *zeo_packet_type_to_string(int packet_id, size_t *len);
char *zeo_packet_type_to_desc  (int packet_id, size_t *len);

/* Event enumeration utilities. */
#define X(ID, VAR, NAME, DESC) VAR = ID,
enum zeo_event_type { ZEO_EVENT_TYPES(X) };
#undef X

char *zeo_event_type_to_string(int packet_id, size_t *len);
char *zeo_event_type_to_desc  (int packet_id, size_t *len);

#include <stdint.h>

#pragma pack(push,1)
typedef struct S_ZEO_HEADER
{
	/* Zeo packet header. */
	char start_char;       /* Always 'A' (for now). */
	char protocol_version; /* '0' through '9' (or higher?) */
	uint8_t  checksum;
	uint16_t message_length; /* Includes this header info. */
	uint16_t message_length_inverse; /* Used as a redundancy check */
	uint8_t  _padding_1;
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
	* The data itself.  The length is message_length - sizeof(header)
	* This is dynamically allocated.
	* The caller is responsible for free'ing it.
	***********************************************************************/
	void *data;
	
} zeo_packet;


/*******************************************************************************
* Call this on any 'zeo_packet' instance before using it.
*******************************************************************************/
void init_zeo_packet( zeo_packet *packet );

/*******************************************************************************
* Creates a string representation of the given Zeo packet.
*******************************************************************************/
int zeo_packet_to_string( zeo_packet *packet, char *buf, int len );

#endif
