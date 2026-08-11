#include <cstdio>
#include "chips.h"

// 1-bit Register
bool bit(bool& state, bool in, bool load) {
    state = mux(state, in, load);
    return state;
}

// 16-bit register
Word register16(Word& state, const Word& in, bool load) {
    for (int i = 0; i < 16; i++) {
        state[i] = bit(state[i], in[i], load);
    }

    return state;
}

// RAM8 Chip
Word ram8(const Word& in, bool load, const sel3& address) {
    Word out{};

    bool la, lb, lc, ld, le, lf, lg, lh;

    dmux8way(load, la, lb, lc, ld, le, lf, lg, lh, address);

    register16(ram_state[0], in, la);
    register16(ram_state[1], in, lb);
    register16(ram_state[2], in, lc);
    register16(ram_state[3], in, ld);
    register16(ram_state[4], in, le);
    register16(ram_state[5], in, lf);
    register16(ram_state[6], in, lg);
    register16(ram_state[7], in, lh);

    mux8way16(
        ram_state[0], ram_state[1],
        ram_state[2], ram_state[3],
        ram_state[4], ram_state[5],
        ram_state[6], ram_state[7],
        address, out
    );

    return out;
}
