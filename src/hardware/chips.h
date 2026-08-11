#pragma once

#include <array>

// typedef for custom data types
using Word = std::array<bool, 16>;
using halfWord = std::array<bool, 8>;

using sel2 = std::array<bool, 2>;
using sel3 = std::array<bool, 3>;

namespace {
    std::array<Word, 8> ram_state{};
}


// Elementary chips
bool nand(bool a, bool b);
bool not_gate(bool in);
bool and_gate(bool a, bool b);
bool or_gate(bool a, bool b);
bool xor_gate(bool a, bool b);
bool mux(bool a, bool b, bool sel);
void dmux(bool in, bool sel, bool& a, bool& b);

// 16-bit chips
void not16(const Word& in, Word& out);
void and16(const Word& a, const Word& b, Word& out);
void or16(const Word& a, const Word& b, Word& out);
void mux16(const Word& a, const Word& b, bool sel, Word& out);

// Multiway chips
bool or8way(const halfWord& in);
bool or16way(const Word& in);

void mux4way16(
    const Word& a,
    const Word& b,
    const Word& c,
    const Word& d,
    const sel2& sel,
    Word& out
);

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
);

void dmux4way(
    bool in,
    bool& a,
    bool& b,
    bool& c,
    bool& d,
    const sel2& sel
);

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
);

// Adders
void halfadder(bool a, bool b, bool* sum, bool* carry);

void fulladder(
    bool a,
    bool b,
    bool carry_in,
    bool* sum,
    bool* carry_out
);

void add16(const Word& a, const Word& b, Word& out);
void inc16(const Word& in, Word& out);


// memory chips
bool bit(bool in, bool load);

Word register16(Word& state, const Word& in, bool load);

Word ram8(const Word& in, bool load, const sel3& address);
