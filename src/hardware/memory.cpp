#include "chips.h"

// ============================================================
// 1-bit Register
// ============================================================

bool bit(bool in, bool load, bool& state) {
    state = mux(state, in, load);
    return state;
}


// ============================================================
// 16-bit Register
// ============================================================

Word register16(
    const Word& in,
    bool load,
    Word& state
) {
    for (int i = 0; i < 16; i++) {
        state[i] = bit(in[i], load, state[i]);
    }

    return state;
}


// ============================================================
// RAM8
// 8 × 16-bit words
// ============================================================

Word ram8(
    const Word& in,
    bool load,
    const sel3& address,
    std::array<Word, 8>& state
) {
    Word out{};

    bool la, lb, lc, ld;
    bool le, lf, lg, lh;

    dmux8way(
        load,
        la, lb, lc, ld,
        le, lf, lg, lh,
        address
    );

    register16(in, la, state[0]);
    register16(in, lb, state[1]);
    register16(in, lc, state[2]);
    register16(in, ld, state[3]);
    register16(in, le, state[4]);
    register16(in, lf, state[5]);
    register16(in, lg, state[6]);
    register16(in, lh, state[7]);

    mux8way16(
        state[0], state[1],
        state[2], state[3],
        state[4], state[5],
        state[6], state[7],
        address,
        out
    );

    return out;
}


// ============================================================
// RAM64
// 8 × RAM8
// 64 × 16-bit words
// ============================================================

Word ram64(
    const Word& in,
    bool load,
    const sel6& address,
    std::array<
        std::array<Word, 8>,
        8
    >& state
) {
    Word out{};

    bool la, lb, lc, ld;
    bool le, lf, lg, lh;

    sel3 address_ram8{};
    sel3 address_select{};

    // [5:3] -> RAM8 selector
    // [2:0] -> RAM8 address
    for (int i = 0; i < 3; i++) {
        address_ram8[i] = address[i];
        address_select[i] = address[i + 3];
    }

    dmux8way(
        load,
        la, lb, lc, ld,
        le, lf, lg, lh,
        address_select
    );

    Word ra = ram8(in, la, address_ram8, state[0]);
    Word rb = ram8(in, lb, address_ram8, state[1]);
    Word rc = ram8(in, lc, address_ram8, state[2]);
    Word rd = ram8(in, ld, address_ram8, state[3]);
    Word re = ram8(in, le, address_ram8, state[4]);
    Word rf = ram8(in, lf, address_ram8, state[5]);
    Word rg = ram8(in, lg, address_ram8, state[6]);
    Word rh = ram8(in, lh, address_ram8, state[7]);

    mux8way16(
        ra, rb, rc, rd,
        re, rf, rg, rh,
        address_select,
        out
    );

    return out;
}


// ============================================================
// RAM512
// 8 × RAM64
// 512 × 16-bit words
// ============================================================

Word ram512(
    const Word& in,
    bool load,
    const sel9& address,
    std::array<
        std::array<
            std::array<Word, 8>,
            8
        >,
        8
    >& state
) {
    Word out{};

    bool la, lb, lc, ld;
    bool le, lf, lg, lh;

    sel6 address_ram64{};
    sel3 address_select{};

    // First 6 bits -> RAM64 address
    for (int i = 0; i < 6; i++) {
        address_ram64[i] = address[i];
    }

    // Last 3 bits -> RAM64 selector
    for (int i = 0; i < 3; i++) {
        address_select[i] = address[i + 6];
    }

    dmux8way(
        load,
        la, lb, lc, ld,
        le, lf, lg, lh,
        address_select
    );

    Word ra = ram64(in, la, address_ram64, state[0]);
    Word rb = ram64(in, lb, address_ram64, state[1]);
    Word rc = ram64(in, lc, address_ram64, state[2]);
    Word rd = ram64(in, ld, address_ram64, state[3]);
    Word re = ram64(in, le, address_ram64, state[4]);
    Word rf = ram64(in, lf, address_ram64, state[5]);
    Word rg = ram64(in, lg, address_ram64, state[6]);
    Word rh = ram64(in, lh, address_ram64, state[7]);

    mux8way16(
        ra, rb, rc, rd,
        re, rf, rg, rh,
        address_select,
        out
    );

    return out;
}


// ============================================================
// RAM4K
// 8 × RAM512
// 4096 × 16-bit words
// ============================================================

Word ram4k(
    const Word& in,
    bool load,
    const sel12& address,
    std::array<
        std::array<
            std::array<
                std::array<Word, 8>,
                8
            >,
            8
        >,
        8
    >& state
) {
    Word out{};

    bool la, lb, lc, ld;
    bool le, lf, lg, lh;

    sel9 address_ram512{};
    sel3 address_select{};

    // First 9 bits -> RAM512 address
    for (int i = 0; i < 9; i++) {
        address_ram512[i] = address[i];
    }

    // Last 3 bits -> RAM512 selector
    for (int i = 0; i < 3; i++) {
        address_select[i] = address[i + 9];
    }

    dmux8way(
        load,
        la, lb, lc, ld,
        le, lf, lg, lh,
        address_select
    );

    Word ra = ram512(in, la, address_ram512, state[0]);
    Word rb = ram512(in, lb, address_ram512, state[1]);
    Word rc = ram512(in, lc, address_ram512, state[2]);
    Word rd = ram512(in, ld, address_ram512, state[3]);
    Word re = ram512(in, le, address_ram512, state[4]);
    Word rf = ram512(in, lf, address_ram512, state[5]);
    Word rg = ram512(in, lg, address_ram512, state[6]);
    Word rh = ram512(in, lh, address_ram512, state[7]);

    mux8way16(
        ra, rb, rc, rd,
        re, rf, rg, rh,
        address_select,
        out
    );

    return out;
}


// ============================================================
// RAM16K
// 4 × RAM4K
// 16384 × 16-bit words
// ============================================================

Word ram16k(
    const Word& in,
    bool load,
    const sel14& address,
    std::array<
        std::array<
            std::array<
                std::array<
                    std::array<Word, 8>,
                    8
                >,
                8
            >,
            8
        >,
        4
    >& state
) {
    Word out{};

    bool la, lb, lc, ld;

    sel12 address_ram4k{};
    sel2 address_select{};

    // First 12 bits -> RAM4K address
    for (int i = 0; i < 12; i++) {
        address_ram4k[i] = address[i];
    }

    // Last 2 bits -> RAM4K selector
    for (int i = 0; i < 2; i++) {
        address_select[i] = address[i + 12];
    }

    // 2-to-4 demultiplexer
    dmux4way(
        load,
        la, lb, lc, ld,
        address_select
    );

    Word ra = ram4k(
        in,
        la,
        address_ram4k,
        state[0]
    );

    Word rb = ram4k(
        in,
        lb,
        address_ram4k,
        state[1]
    );

    Word rc = ram4k(
        in,
        lc,
        address_ram4k,
        state[2]
    );

    Word rd = ram4k(
        in,
        ld,
        address_ram4k,
        state[3]
    );


    mux4way16(
        ra, rb, rc, rd,
        address_select,
        out
    );

    return out;
}


// ============================================================
// PC: Program Counter
// ============================================================
Word programCounter(Word in, bool load, int inc, bool reset, Word& state){
	Word incOut{}, incOrHold, loadOrInc, nextPC;
	Word zero{};
	bool loadOrReset, shouldLoad;
	 
	// calculate next value with priority: reset > load > inc
	inc16(state, incOut);
	mux16(state, incOut, inc, incOrHold);
	mux16(incOrHold, in, load, loadOrInc);
	mux16(loadOrInc, zero, reset, nextPC);

	// register load signal: load if reset
	loadOrReset = or_gate(reset, load);
	shouldLoad = or_gate(loadOrReset, inc);
	register16(nextPC,shouldLoad,state);

	return state;
}
