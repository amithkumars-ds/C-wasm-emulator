#include <stdio.h>
#include <stdbool.h>
#include "chips.h"

// ----------------------- ALU CHIP -----------------------

void alu(
	const Word& x, const Word& y,
	bool zx, bool nx,
	bool zy, bool ny,
	bool f, bool no,
	bool *zr, bool *ng,
	Word& out
){
	Word zeroed_x;
	Word negated_x;
	Word processed_x;
	
	Word zeroed_y;
	Word negated_y;
	Word processed_y;

	Word and_xy;
	Word add_xy;
	Word f_out;
	Word negated_out;
	
	Word zero_16 = {};

	// preprocess x
	mux16(x,zero_16,zx,zeroed_x);

	not16(zeroed_x,negated_x);
	mux16(zeroed_x,negated_x,nx,processed_x);

	// preprocess y
	mux16(y,zero_16,zy,zeroed_y);

	not16(zeroed_y,negated_y);
	mux16(zeroed_y,negated_y,ny,processed_y);

	// compute function
	and16(processed_x,processed_y,and_xy);
	add16(processed_x,processed_y,add_xy);
	mux16(and_xy,add_xy,f,f_out);

	// output postprocess
	not16(f_out,negated_out);
	mux16(f_out,negated_out,no,out);

	// output control bits
	*zr = not_gate(or16way(out));
	*ng = out[0];
}
