#ifndef CHIPS_H
#define CHIPS_H

#include <stdio.h>
#include <stdbool.h>

// elementary chips
bool nand(bool a, bool b);
bool not(bool in);
bool or(bool a, bool b);
bool xor(bool a, bool b);
bool mux(bool a, bool b, bool sel);
void dmux(bool in, bool sel, bool *a, bool *b);

// 16-bit chips
void not16(bool in[16], bool out[16]);
void and16(bool a[16], bool b[16], bool out[16]);
void or16(bool a[16], bool b[16], bool out[16]);
void mux16(bool a[16], bool b[16], bool sel, bool out[16]);

// multiway chips
bool or8way(bool in[8]);
void mux4way16(bool a[16], bool b[16], bool c[16],
    		   bool d[16], bool sel[2], bool out[16]);
void mux8way16(bool a[16], bool b[16], bool c[16],
    		   bool d[16], bool e[16], bool f[16],
    		   bool g[16], bool h[16],bool sel[3], 
    		   bool out[16]);
void dmux4way(bool in, bool *a, bool *b, 
			  bool *c, bool *d, bool sel[2]);
void dmux8way(bool in, bool *a, bool *b, 
			  bool *c, bool *d, bool *e, 
			  bool *f, bool *g, bool *h, 
			  bool sel[3]);

// adder chips
void halfadder(bool a, bool b, bool *sum, bool *carry);
void fulladder(bool a, bool b, bool carry_in, bool *sum, bool *carry_out);
void add16(bool a[16], bool b[16], bool out[16]);
void inc16(bool in[16], bool out[16]);


#endif
