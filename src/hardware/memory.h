#pragma once

#include "chips.h"

bool bit(bool in, bool load, bool& state);

Word register16(
    const Word& in,
    bool load,
    Word& state
);

Word ram8(
    const Word& in,
    bool load,
    const sel3& address,
    std::array<Word, 8>& state
);

Word ram64(
    const Word& in,
    bool load,
    const sel6& address,
    std::array<
        std::array<Word, 8>,
        8
    >& state
);

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
);

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
);

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
);

Word programCounter(
	Word in, 
	bool load, 
	int inc, 
	bool reset, 
	Word& state
);
