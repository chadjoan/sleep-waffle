/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */


#ifndef ZEO_SPLICE_INCLUDED
#define ZEO_SPLICE_INCLUDED

#include <stdint.h>

#pragma pack(push,1)
typedef struct S_ZEO_FREQ_BIN
{
	uint16_t amplitute;
	uint16_t lo_cutoff;
	uint16_t hi_cutoff;
} zeo_freq_bin;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct S_ZEO_SPLICE
{
	/* Integer members will be -1 when not sent. */
	int32_t n_packets;
	int32_t sqi;
	int32_t time;
	int32_t version;
	int32_t impedance;
	int32_t signal;
	int32_t stage;

	/* Array members will be NULL when not sent. */
	/* Length members will be 0 when not sent. */
	/* In the interest of future-proofing, all arrays are dynamically. */
	/* The caller is responsible for free'ing them. */
	/* Use the clear_zeo_splice function to do this. */
	zeo_freq_bin *freq_bins;
	size_t       n_freq_bins;
	
	int16_t  *raw_samples;
	size_t   n_raw_samples;
	
	/* How much time elapsed during sampling, in nanoseconds. */
	/* Currently this is always 1000000 ns (1 second). */
	uint32_t sample_span;

} zeo_splice;
#pragma pack(pop)

/* Initializes a zeo_splice.  Always call this after allocating a new splice. */
void init_zeo_splice( zeo_splice *splice );

/* Frees any memory held by the given splice, then reinitializes the splice. */
/* This does not free the splice itself.  To do that, use free_zeo_splice. */
void clear_zeo_splice( zeo_splice *splice );

/* Frees the given zeo_splice and any memory it indirectly holds. */
void free_zeo_splice( zeo_splice *splice );

#endif
