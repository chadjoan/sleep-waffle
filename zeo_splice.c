/*          Copyright Chad Joan 2011.                         */
/* Distributed under the Boost Software License, Version 1.0. */
/*    (See accompanying file LICENSE_1_0.txt or copy at       */
/*          http://www.boost.org/LICENSE_1_0.txt)             */

#include <zeo_malloc.h>
#include <zeo_splice.h>

void init_zeo_splice( zeo_splice *splice )
{
	splice->n_packets = -1;
	splice->sqi = -1;
	splice->time = -1;
	splice->impedance = -1;
	splice->signal = -1;
	splice->stage = -1;
	
	splice->freq_bins = NULL;
	splice->n_freq_bins = 0;
	
	splice->raw_samples = NULL;
	splice->n_raw_samples = 0;
	splice->sample_span = 1000000UL;
	
}

void clear_zeo_splice( zeo_splice *splice )
{
	free(splice->freq_bins);
	free(splice->raw_samples);
	init_zeo_splice(splice);
}

void free_zeo_splice( zeo_splice *splice )
{
	clear_zeo_splice(splice);
	zeo_free(splice);
}
