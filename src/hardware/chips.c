#include <stdio.h>
#include <stdbool.h>	

// --------------------------- 0 Building brick: Nand Chip ---------------------------

bool nand(bool a, bool b){
	bool and_ab = a && b;
	bool out = !(and_ab);
	return out;
}

// --------------------------- 1a Elementary chips ---------------------------
bool not(bool in){
	bool out = nand(in,in);
	return out;
}

bool and(bool a, bool b){
	bool nand_a_b = nand(a,b);
	bool out = nand(nand_a_b,nand_a_b);
	return out;	
}

bool or(bool a, bool b){
	bool not_a = nand(a,a);
	bool not_b = nand(b,b);
	bool out = nand(not_a,not_b);

	return out;
}

bool xor(bool a, bool b){
	bool nand_a_b = nand(a,b);
	bool nand_a_nand_a_b = nand(a,nand_a_b);
	bool nand_b_nand_a_b = nand(b,nand_a_b);
	bool out = nand(nand_a_nand_a_b,nand_b_nand_a_b);	
	return out;
}

bool mux(bool a, bool b, bool sel){
	bool not_s = nand(sel,sel);
	bool nand_a_not_s = nand(a,not_s);
	bool nand_b_s = nand(b,sel);
	bool out = nand(nand_a_not_s, nand_b_s);
	
	return out;
}

void dmux(bool in, bool sel, bool *a, bool *b){
	bool not_s = nand(sel,sel);
	bool nand_in_not_s = nand(in,not_s);
	*a = nand(nand_in_not_s,nand_in_not_s);

	bool nand_in_s = nand(in,sel);
	*b = nand(nand_in_s,nand_in_s);
}


// --------------------------- 1b MultiBit Variant chips ---------------------------
// helper print function
void print_bits(bool bits[], int size){
	for (int i=0; i<size; i++){
		printf("%d",bits[i]);
	}
	printf("\n");
}

void not16(bool in[16], bool out[16]){
	for (int i=0; i<16; i++){
		out[i] = not(in[i]);
	}
}

void and16(bool a[16], bool b[16], bool out[16]){
	for (int i=0; i<16; i++){
		out[i] = and(a[i],b[i]);
	}
}

void or16(bool a[16], bool b[16], bool out[16]){
	for (int i=0; i<16; i++){
		out[i] = or(a[i],b[i]);
	}
}

void mux16(bool a[16], bool b[16], bool sel, bool out[16]){
	for (int i=0; i<16; i++){
		out[i] = mux(a[i],b[i],sel);
	}
}


// --------------------------- 1c MultiWay Variant chips ---------------------------
bool or8way(bool in[8]){
	bool or01 = or(in[0],in[1]);
	bool or23 = or(in[2],in[3]);
	bool or45 = or(in[4],in[5]);
	bool or67 = or(in[6],in[7]);

	bool or0123 = or(or01,or23);
	bool or4567 = or(or45,or67);

	bool out = or(or0123,or4567);

	return out;
}

void mux4way16(
    bool a[16],
    bool b[16],
    bool c[16],
    bool d[16],
    bool sel[2],
    bool out[16]
) {
    bool ab[16];
    bool cd[16];

    mux16(a,b,sel[1],ab);
    mux16(c,d,sel[1],cd);

    mux16(ab,cd,sel[0],out);
}

void mux8way16(
    bool a[16],
    bool b[16],
    bool c[16],
    bool d[16],
    bool e[16],
    bool f[16],
    bool g[16],
    bool h[16],
    bool sel[3],
    bool out[16]
) {
    bool abcd[16];
    bool efgh[16];
    bool subset[2] = {sel[1], sel[2]};

    mux4way16(a,b,c,d,subset,abcd);
    mux4way16(e,f,g,h,subset,efgh);

    mux16(abcd,efgh,sel[0],out);
}

void dmux4way(
	bool in, 
	bool *a, 
	bool *b, 
	bool *c, 
	bool *d, 
	bool sel[2]
){
	bool ab;
	bool cd;

	dmux(in,sel[0],&ab,&cd);
	dmux(ab,sel[1],a,b);
	dmux(cd,sel[1],c,d);
}

void dmux8way(
	bool in, 
	bool *a, 
	bool *b, 
	bool *c, 
	bool *d, 
	bool *e, 
	bool *f, 
	bool *g, 
	bool *h, 
	bool sel[3]
){
	bool abcd;
	bool efgh;
	bool subset[2] = {sel[1], sel[2]};

	dmux(in, sel[0], &abcd, &efgh);
	dmux4way(abcd,a,b,c,d,subset);
	dmux4way(efgh,e,f,g,h,subset);
}


// --------------------------- 2 Adder Chips ---------------------------
void halfadder(bool a, bool b, bool *sum, bool *carry){
	*sum = xor(a,b);
	*carry = and(a,b);
}

void fulladder(bool a, bool b, bool carry_in, bool *sum, bool *carry_out){
	bool xor_a_b = xor(a,b);
	*sum = xor(xor_a_b,carry_in);

	bool ab = and(a,b);
	bool cin_xor_ab = and(xor_a_b,carry_in);
	*carry_out = or(ab,cin_xor_ab);
}

void add16(bool a[16], bool b[16], bool out[16]){
	bool sum[16];
	bool c_out[16];

	halfadder(a[15], b[15], &sum[15], &c_out[15]);   // start at LSB (index 15)

	for (int i = 14; i >= 0; i--){                    // walk toward MSB (index 0)
		fulladder(a[i], b[i], c_out[i+1], &sum[i], &c_out[i]);
	}

	for (int i = 0; i < 16; i++){
		out[i] = sum[i];
	}
}

void inc16(bool in[16], bool out[16]){
	bool one[16] = {false};
	one[15] = true;
	add16(in,one,out);
}
