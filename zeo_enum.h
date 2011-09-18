/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */


#ifndef ZEO_ENUM_INCLUDED
#define ZEO_ENUM_INCLUDED

#include <string.h>

/* All of the enum printing functions follow this pattern: */
#define ZEO_ENUM_STRINGIZER_START(FUNC_NAME) \
char *FUNC_NAME(int packet_id, size_t *len) \
{ \
	char *result; \
	size_t dummy_len; \
	if ( len == NULL ) \
		len = &dummy_len; \
	\
	switch(packet_id) \
	{
#define ZEO_ENUM_STRINGIZER_CASE(KEY,VALUE) \
		case KEY: { \
			result = VALUE; \
			*len = strlen(result); \
			return result; \
		}
		/* ZEO_PACKET_TYPES(GENERATE_ZEO_ENUM_CASE) */
#define ZEO_ENUM_STRINGIZER_END(ERR_MSG) \
	} \
	 \
	result = ERR_MSG; \
	*len = strlen(result); \
	return result; \
}

#endif