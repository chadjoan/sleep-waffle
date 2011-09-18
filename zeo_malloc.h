/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */

/* This module contains definitions for the malloc/realloc/free functions */
/*   used by Sleep Waffle.  */
/* The intent is to allow these to be easily redefined if a new platform has */
/*   different memory management requirements than the ones already supported.*/

#ifndef ZEO_MALLOC_INCLUDED
#define ZEO_MALLOC_INCLUDED

#include <stdlib.h>

void *zeo_malloc( size_t size );
void *zeo_realloc( void *ptr, size_t size );
void zeo_free( void *ptr );

/* Special functions for calling D's garbage collector's allocator functions. */
/* These are used when zeo_language_compat is set to zeo_lang_d. */
void *zeo_d_lang_gc_malloc( size_t size );
void *zeo_d_lang_gc_realloc( void *ptr, size_t size );
void zeo_d_lang_gc_free( void *ptr );

#endif
