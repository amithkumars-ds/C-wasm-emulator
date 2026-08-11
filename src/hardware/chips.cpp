#include "chips.h"
#include <cstdio>

// --------------------------- 0 Building brick: Nand Chip ---------------------------

bool nand(bool a, bool b){
	bool out = !(a && b);
	return out;
}

// --------------------------- 1a Elementary chips ---------------------------
bool not_gate(bool in){
	bool out = !in;
	return out;
}

bool and_gate(bool a, bool b){
	bool out = a && b;
	return out;	
}

bool or_gate(bool a, bool b){
	bool out = a || b;

	return out;
}

bool xor_gate(bool a, bool b){
	bool out = (!a && b) || (a && !b);	
	return out;
}

bool mux(bool a, bool b, bool sel){
	bool out = (a && !sel) || (b && sel);
	return out;
}

void dmux(bool in, bool sel, bool& a, bool& b){
	a = in && !sel;
	b = in && sel;
}


// --------------------------- 1b MultiBit Variant chips ---------------------------
// helper print function
void print_bits(bool bits[], int size){
	for (int i=0; i<size; i++){
		printf("%d",bits[i]);
	}
	printf("\n");
}

void not16(const Word& in,  Word& out){
	for (int i=0; i<16; i++){
		out[i] = not_gate(in[i]);
	}
}

void and16(const Word&a, const Word& b, Word& out){
	for (int i=0; i<16; i++){
		out[i] = and_gate(a[i],b[i]);
	}
}

void or16(const Word& a, const Word& b, Word& out){
	for (int i=0; i<16; i++){
		out[i] = or_gate(a[i],b[i]);
	}
}

void mux16(const Word& a, const Word& b, bool sel, Word& out ){
	for (int i=0; i<16; i++){
		out[i] = mux(a[i],b[i],sel);
	}
}


// --------------------------- 1c MultiWay Variant chips ---------------------------
bool or8way(const halfWord& in){
	bool or01 = or_gate(in[0],in[1]);
	bool or23 = or_gate(in[2],in[3]);
	bool or45 = or_gate(in[4],in[5]);
	bool or67 = or_gate(in[6],in[7]);

	bool or0123 = or_gate(or01,or23);
	bool or4567 = or_gate(or45,or67);

	bool out = or_gate(or0123,or4567);

	return out;
}

bool or16way(const Word& in){
	halfWord left{};
	halfWord right{};

	for (int i=0;i<8;i++){
		left[i] = in[i];
		right[i] = in[i+8];}

	bool leftOut = or8way(left);
	bool rightOut = or8way(right);
	bool out = or_gate(leftOut,rightOut);

	return out;
}

void mux4way16(
    const Word& a,
    const Word& b,
    const Word& c,
    const Word& d,
    const sel2& sel,
    Word& out
) {
    Word ab{};
    Word cd{};

    mux16(a,b,sel[1],ab);
    mux16(c,d,sel[1],cd);

    mux16(ab,cd,sel[0],out);
}

void mux8way16(
    const Word& a,
    const Word& b,
    const Word& c,
    const Word& d,
    const Word& e,
    const Word& f,
    const Word& g,
    const Word& h,
    const sel3& sel,
    Word& out
) {
    Word abcd{};
    Word efgh{};

    sel2 subset = {sel[1], sel[2]};

    mux4way16(a, b, c, d, subset, abcd);
    mux4way16(e, f, g, h, subset, efgh);

    mux16(abcd, efgh, sel[0], out);
}

void dmux4way(
	bool in, 
	bool& a, 
	bool& b, 
	bool& c, 
	bool& d, 
	const sel2& sel
){
	bool ab;
	bool cd;

	dmux(in,sel[0],ab,cd);
	dmux(ab,sel[1],a,b);
	dmux(cd,sel[1],c,d);
}

void dmux8way(
	bool in, 
	bool& a, 
	bool& b, 
	bool& c, 
	bool& d, 
	bool& e, 
	bool& f, 
	bool& g, 
	bool& h, 
	const sel3& sel
){
	bool abcd;
	bool efgh;
	sel2 subset = {sel[1], sel[2]};

	dmux(in, sel[0], abcd, efgh);
	dmux4way(abcd,a,b,c,d,subset);
	dmux4way(efgh,e,f,g,h,subset);
}


// --------------------------- 2 Adder Chips ---------------------------
void halfadder(bool a, bool b, bool *sum, bool *carry){
	*sum = xor_gate(a,b);
	*carry = and_gate(a,b);
}

void fulladder(bool a, bool b, bool carry_in, bool *sum, bool *carry_out){
	bool xor_a_b = xor_gate(a,b);
	*sum = xor_gate(xor_a_b,carry_in);

	bool ab = and_gate(a,b);
	bool cin_xor_ab = and_gate(xor_a_b,carry_in);
	*carry_out = or_gate(ab,cin_xor_ab);
}

void add16(const Word& a, const Word& b, Word& out){
	Word sum{};
	Word c_out{};

	halfadder(a[15], b[15], &sum[15], &c_out[15]);   // start at LSB (index 15)

	for (int i = 14; i >= 0; i--){                    // walk toward MSB (index 0)
		fulladder(a[i], b[i], c_out[i+1], &sum[i], &c_out[i]);
	}

	for (int i = 0; i < 16; i++){
		out[i] = sum[i];
	}
}

void inc16(const Word& in, Word& out){
	Word one{};
	one[15] = true;
	add16(in,one,out);
}
