#include <stdio.h>
#include <stdbool.h>
#include "chips.h"

// ----------------------- ALU CHIP -----------------------

void alu(
	bool x[16], bool y[16],
	bool zx, bool nx,
	bool zy, bool ny,
	bool f, bool no,
	bool *zr, bool *ng,
	bool out[16] 
){
	bool zeroed_x[16];
	bool negated_x[16];
	bool processed_x[16];
	
	bool zeroed_y[16];
	bool negated_y[16];
	bool processed_y[16];

	bool and_xy[16];
	bool add_xy[16];
	bool f_out[16];
	bool negated_out[16];
	
	bool zero_16[16] = {false};

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
	bool zr_a[8] = {
	    out[0], out[1], out[2], out[3],
	    out[4], out[5], out[6], out[7]
	};

	bool zr_b[8] = {
	    out[8], out[9], out[10], out[11],
	    out[12], out[13], out[14], out[15]
	};

	*zr = not(or(or8way(zr_a), or8way(zr_b)));
	*ng = out[15];
}
