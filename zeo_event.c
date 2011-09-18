/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */


#include <zeo_malloc.h>
#include <zeo_event.h>
#include <zeo_enum.h>


#define GENERATE_ZEO_ENUM_CASE(ID, VAR, NAME, DESC) \
	ZEO_ENUM_STRINGIZER_CASE(VAR,NAME)
ZEO_ENUM_STRINGIZER_START(zeo_event_type_to_string)
	ZEO_EVENT_TYPES(GENERATE_ZEO_ENUM_CASE)
ZEO_ENUM_STRINGIZER_END("Can not look up event type name: invalid event type.")
#undef GENERATE_ZEO_ENUM_CASE


#define GENERATE_ZEO_ENUM_CASE(ID, VAR, NAME, DESC) \
	ZEO_ENUM_STRINGIZER_CASE(VAR,DESC)
ZEO_ENUM_STRINGIZER_START(zeo_event_type_to_desc)
	ZEO_EVENT_TYPES(GENERATE_ZEO_ENUM_CASE)
ZEO_ENUM_STRINGIZER_END("Can not look up event type description: invalid event type.")
#undef GENERATE_ZEO_ENUM_CASE

void init_zeo_event( zeo_event *event )
{
	event->time = -1;
	event->version = -1;
	event->type = -1;
}

void clear_zeo_event( zeo_event *event )
{
	init_zeo_event(event);
}

void free_zeo_event( zeo_event *event )
{
	zeo_free(event);
}