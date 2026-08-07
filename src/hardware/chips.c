#include <stdio.h>
#include <stdbool.h>	

// --------------------------- Building brick: Nand Chip ---------------------------

bool nand(bool a, bool b){
	bool and_ab = a && b;
	bool out = !(and_ab);
	return out;
}

// --------------------------- Elementary chips ---------------------------
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


// --------------------------- MultiBit Variant chips ---------------------------
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


// --------------------------- MultiWay Variant chips ---------------------------
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

// --------------------------- Chips Testing ---------------------------

int main() {
    int passed = 0;
    int total = 0;
    for (int in = 0; in < 2; in++) {
        for (int sel = 0; sel < 8; sel++) {
            bool sel_bits[3] = {
                (sel >> 2) & 1,
                (sel >> 1) & 1,
                sel & 1
            };
            bool a, b, c, d, e, f, g, h;
            dmux8way(
                in,
                &a, &b, &c, &d, &e, &f, &g, &h,
                sel_bits
            );
            bool expected[8] = {0, 0, 0, 0, 0, 0, 0, 0};
            if (in)
                expected[sel] = true;
            bool correct =
                a == expected[0] &&
                b == expected[1] &&
                c == expected[2] &&
                d == expected[3] &&
                e == expected[4] &&
                f == expected[5] &&
                g == expected[6] &&
                h == expected[7];
            total++;
            if (correct)
                passed++;
        }
    }
    printf("Tests passed: %d/%d\n", passed, total);
    return 0;
}
