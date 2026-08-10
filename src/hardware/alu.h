#pragma once

#include "chips.h"

void alu(
    const Word& x,
    const Word& y,
    bool zx,
    bool nx,
    bool zy,
    bool ny,
    bool f,
    bool no,
    bool* zr,
    bool* ng,
    Word& out
);
