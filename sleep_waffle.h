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

#endif