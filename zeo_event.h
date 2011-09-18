/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */


#ifndef ZEO_EVENT_INCLUDED
#define ZEO_EVENT_INCLUDED

#include <stdint.h>
#include <stdlib.h>

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


/* Event enumeration utilities. */
#define ZEO_ENUM_VAR_ID(ID, VAR, NAME, DESC) VAR = ID,
enum zeo_event_type { ZEO_EVENT_TYPES(ZEO_ENUM_VAR_ID) };
#undef ZEO_ENUM_VAR_ID

char *zeo_event_type_to_string(int packet_id, size_t *len);
char *zeo_event_type_to_desc  (int packet_id, size_t *len);

typedef struct S_ZEO_EVENT
{
	int32_t time;
	int32_t version;
	int32_t type;
	
} zeo_event;


/* Initializes a zeo_event.  Always call this after allocating a new event. */
void init_zeo_event( zeo_event *event );

/* Frees any memory held by the given event, then reinitializes the event. */
/* This does not free the event itself.  To do that, use free_zeo_event. */
/* NOTE: zeo_events current do not allocate any memory, so this is equivalent */
/*         to calling init_zeo_event.  It is provided as a future-proofing */
/*         measure and is consistent with zeo_event's interface. */
void clear_zeo_event( zeo_event *event );

/* Frees the given zeo_event and any memory it indirectly holds. */
void free_zeo_event( zeo_event *event );

#endif
