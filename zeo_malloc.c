/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */

#include <stdio.h>
#include <stdlib.h>

#include <sleep_waffle.h>
#include <zeo_malloc.h>

static zeo_invalid_language_error( uint32_t language )
{
	/* TODO: print a more useful description. */
	fprintf(stderr, "%d is not a valid language type.\n", language);
}

void *zeo_malloc( size_t size )
{
	void *result;
	switch(zeo_language_compat)
	{
		case zeo_lang_c: result = malloc(size); break;
		case zeo_lang_d: result = zeo_d_lang_gc_malloc(size); break;
		default:
			zeo_invalid_language_error(zeo_language_compat);
			break;
	}
	return result;
}

void *zeo_realloc( void *ptr, size_t size )
{
	switch(zeo_language_compat)
	{
		case zeo_lang_c: ptr = realloc(ptr,size); break;
		case zeo_lang_d: ptr = zeo_d_lang_gc_realloc(ptr,size); break;
		default:
			zeo_invalid_language_error(zeo_language_compat);
			break;
	}
	return ptr;
}

void zeo_free( void *ptr )
{
	switch(zeo_language_compat)
	{
		case zeo_lang_c: free(ptr); break;
		case zeo_lang_d: zeo_d_lang_gc_free(ptr); break;
		default:
			zeo_invalid_language_error(zeo_language_compat);
			break;
	}
}

#ifndef DLANG_COMPAT
/* Define stubs so that the linker won't complain. */
void *zeo_d_lang_gc_malloc( size_t size ) {}
void *zeo_d_lang_gc_realloc( void *ptr, size_t size ) {}
void zeo_d_lang_gc_free( void *ptr ) {}
#endif