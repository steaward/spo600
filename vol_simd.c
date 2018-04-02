// vol_simd.c :: volume scaling in C using AArch64 SIMD
// Chris Tyler 2017.11.29-2018.02.20

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "vol.h"

int main() {

	int16_t*		in;		// input array
	int16_t*		limit;		// end of input array
	int16_t*		out;		// output array

	// these variables will be used in our assembler code, so we're going
	// to hand-allocate which register they are placed in
	// Q: what is an alternate approach?
	register int16_t*	in_cursor 	asm("r20");	// input cursor
	register int16_t*	out_cursor	asm("r21");	// output cursor
	register int16_t	vol_int		asm("r22");	// volume as int16_t

	int			x;		// array interator
	int			ttl;		// array total

	in=(int16_t*) calloc(SAMPLES, sizeof(int16_t));
	out=(int16_t*) calloc(SAMPLES, sizeof(int16_t));

	srand(-1);
	printf("Generating sample data.\n");
	for (x = 0; x < SAMPLES; x++) {
		in[x] = (rand()%65536)-32768;
	}

// --------------------------------------------------------------------

	in_cursor = in;
	out_cursor = out;
	limit = in + SAMPLES ;

	// set vol_int to fixed-point representation of 0.75
	// Q: should we use 32767 or 32768 in next line? why?
	vol_int = (int16_t) (0.75 * 32768.0);

	printf("Scaling samples.\n");

	// Q: what does it mean to "duplicate" values in the next line?
	__asm__ ("dup v1.8h,%w0"::"r"(vol_int)); // duplicate vol_int into v1.8h

	while ( in_cursor < limit ) {
		__asm__ (
			"ldr q0, [%[in]],#16		\n\t"
			// load eight samples into q0 (v0.8h)
			// from in_cursor, and post-increment
			// in_cursor by 16 bytes

			"sqdmulh v0.8h, v0.8h, v1.8h	\n\t"
			// multiply each lane in v0 by v1*2
			// saturate results
			// store upper 16 bits of results into v0
			
			"str q0, [%[out]],#16		\n\t"
			// store eight samples to out_cursor
			// post-increment out_cursor by 16 bytes

			// Q: what happens if we remove the following
			// two lines? Why?
			: [in]"+r"(in_cursor)
			: "0"(in_cursor),[out]"r"(out_cursor)
			);
	}

// --------------------------------------------------------------------

	printf("Summing samples.\n");
	for (x = 0; x < SAMPLES; x++) {
		ttl=(ttl+out[x])%1000;
	}

	// Q: are the results usable? are they correct?
	printf("Result: %d\n", ttl);

	return 0;

}

