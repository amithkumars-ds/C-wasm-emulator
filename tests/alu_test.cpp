#include <iostream>
#include <cstdint>
#include <chrono>
#include "../src/hardware/chips.h"
#include "../src/hardware/alu.h"

bool equalWord(const Word& a, const Word& b) {
    return a == b;
}

Word uint16ToWord(uint16_t value) {
    Word out{};

    for (int i = 0; i < 16; i++) {
        out[i] = (value >> (15 - i)) & 1;
    }

    return out;
}

uint16_t wordToUint16(const Word& word) {
    uint16_t value = 0;

    for (int i = 0; i < 16; i++) {
        value |= static_cast<uint16_t>(word[i]) << (15 - i);
    }

    return value;
}

void printWord(uint16_t value) {
    for (int i = 15; i >= 0; i--) {
        std::cout << ((value >> i) & 1);
    }
}

struct TestStats {
    uint64_t passed = 0;
    uint64_t failed = 0;
};

void test(
    bool condition,
    const char* name,
    TestStats& stats
) {
    if (condition) {
        stats.passed++;
    } else {
        stats.failed++;
        std::cout << "FAILED: " << name << '\n';
    }
}


// ------------------------------------------------------------
// Reference implementation of the Hack ALU
// ------------------------------------------------------------

uint16_t referenceALU(
    uint16_t x,
    uint16_t y,
    bool zx,
    bool nx,
    bool zy,
    bool ny,
    bool f,
    bool no
) {
    if (zx)
        x = 0;

    if (nx)
        x = static_cast<uint16_t>(~x);

    if (zy)
        y = 0;

    if (ny)
        y = static_cast<uint16_t>(~y);

    uint16_t out;

    if (f)
        out = static_cast<uint16_t>(x + y);
    else
        out = x & y;

    if (no)
        out = static_cast<uint16_t>(~out);

    return out;
}


// ------------------------------------------------------------
// ALU TEST
// ------------------------------------------------------------

int main() {
	auto start = std::chrono::high_resolution_clock::now();

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "        NAND2TETRIS ALU TESTS\n";
    std::cout << "========================================\n\n";

    TestStats stats;

    /*
        We test:

        - All 64 combinations of ALU control bits
        - Multiple strategically chosen x/y values
        - zr
        - ng
        - 16-bit output
    */

    uint16_t testValues[] = {
        0x0000,
        0x0001,
        0x0002,
        0x0003,
        0x000F,
        0x00FF,
        0x0100,
        0x7FFF,
        0x8000,
        0x8001,
        0xFFFF,
        0xAAAA,
        0x5555
    };

    const int numValues =
        sizeof(testValues) / sizeof(testValues[0]);

    int controlCases = 0;


    // --------------------------------------------------------
    // Exhaustive control-bit testing
    // --------------------------------------------------------

    for (int zx = 0; zx <= 1; zx++) {
        for (int nx = 0; nx <= 1; nx++) {
            for (int zy = 0; zy <= 1; zy++) {
                for (int ny = 0; ny <= 1; ny++) {
                    for (int f = 0; f <= 1; f++) {
                        for (int no = 0; no <= 1; no++) {

                            controlCases++;

                            for (int xi = 0; xi < numValues; xi++) {
                                for (int yi = 0; yi < numValues; yi++) {

                                    uint16_t xValue =
                                        testValues[xi];

                                    uint16_t yValue =
                                        testValues[yi];

                                    Word x =
                                        uint16ToWord(xValue);

                                    Word y =
                                        uint16ToWord(yValue);

                                    Word out{};

                                    bool zr = false;
                                    bool ng = false;


                                    // ------------------------
                                    // Run your ALU
                                    // ------------------------

                                    alu(
                                        x,
                                        y,
                                        zx,
                                        nx,
                                        zy,
                                        ny,
                                        f,
                                        no,
                                        &zr,
                                        &ng,
                                        out
                                    );


                                    // ------------------------
                                    // Reference result
                                    // ------------------------

                                    uint16_t expected =
                                        referenceALU(
                                            xValue,
                                            yValue,
                                            zx,
                                            nx,
                                            zy,
                                            ny,
                                            f,
                                            no
                                        );


                                    uint16_t actual =
                                        wordToUint16(out);


                                    // ------------------------
                                    // Output
                                    // ------------------------

                                    test(
                                        actual == expected,
                                        "ALU OUTPUT",
                                        stats
                                    );


                                    // ------------------------
                                    // zr
                                    // ------------------------

                                    bool expectedZr =
                                        (expected == 0);

                                    test(
                                        zr == expectedZr,
                                        "ALU ZR",
                                        stats
                                    );


                                    // ------------------------
                                    // ng
                                    // ------------------------

                                    bool expectedNg =
                                        (expected & 0x8000) != 0;

                                    test(
                                        ng == expectedNg,
                                        "ALU NG",
                                        stats
                                    );
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    // --------------------------------------------------------
    // Results
    // --------------------------------------------------------
	auto end = std::chrono::high_resolution_clock::now();
	
	double elapsed =
	    std::chrono::duration<double, std::milli>(end - start).count();
	    
    std::cout << "\n";
    std::cout << "Control combinations : "
              << controlCases << '\n';

    std::cout << "Tests passed         : "
              << stats.passed << '\n';

    std::cout << "Tests failed         : "
              << stats.failed << '\n';

    std::cout << "Total tests          : "
              << stats.passed + stats.failed << '\n';

	std::cout << "TIME         : "
	          << elapsed << " ms\n";
	          
    if (stats.failed == 0) {
        std::cout << "\nALL ALU TESTS PASSED\n";
        return 0;
    }

    std::cout << "\nALU TESTS FAILED\n";
    return 1;
}
