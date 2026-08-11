#include <cstdio>
#include "chips.h"

// DFF
namespace {
    bool state = false;
}

bool dff(bool in) {
    bool previous_out = state;
    state = in;
    return previous_out;
}

// 1-bit Register
namespace {
    bool bit_state = false;
}

bool bit(bool in, bool load) {
    bool next = mux(bit_state, in, load);
    bit_state = next;
    return bit_state;
}
