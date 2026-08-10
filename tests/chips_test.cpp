#include "../src/hardware/chips.h"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

struct TestStats {
    std::string name;
    uint64_t passed = 0;
    uint64_t failed = 0;

    uint64_t total() const {
        return passed + failed;
    }
};

void test(
    bool condition,
    TestStats& stats
) {
    if (condition) {
        stats.passed++;
    } else {
        stats.failed++;
    }
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

void printStats(const TestStats& stats) {
    std::cout
        << stats.name
        << " : "
        << stats.passed
        << "/"
        << stats.total()
        << " passed";

    if (stats.failed > 0) {
        std::cout
            << "  ["
            << stats.failed
            << " FAILED]";
    } else {
        std::cout << "  [PASS]";
    }

    std::cout << '\n';
}

int main() {

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "========================================\n";
    std::cout << "        NAND2TETRIS CHIP TESTS\n";
    std::cout << "========================================\n\n";

    std::vector<TestStats> stats = {

        {"NAND"},
        {"NOT"},
        {"AND"},
        {"OR"},
        {"XOR"},
        {"MUX"},
        {"DMUX"},
        {"NOT16"},
        {"AND16"},
        {"OR16"},
        {"MUX16"},
        {"OR8WAY"},
        {"OR16WAY"},
        {"MUX4WAY16"},
        {"MUX8WAY16"},
        {"DMUX4WAY"},
        {"DMUX8WAY"},
        {"HALFADDER"},
        {"FULLADDER"},
        {"ADD16"},
        {"INC16"}
    };

    // ========================================================
    // 1. SINGLE-BIT CHIPS
    // ========================================================

    std::cout << "[1] SINGLE-BIT CHIPS\n";

    for (int a = 0; a <= 1; a++) {
        for (int b = 0; b <= 1; b++) {

            bool A = a;
            bool B = b;

            test(
                nand(A, B) == !(A && B),
                stats[0]
            );

            test(
                and_gate(A, B) == (A && B),
                stats[2]
            );

            test(
                or_gate(A, B) == (A || B),
                stats[3]
            );

            test(
                xor_gate(A, B) == (A != B),
                stats[4]
            );

            for (int sel = 0; sel <= 1; sel++) {

                bool expected = sel ? B : A;

                test(
                    mux(A, B, sel) == expected,
                    stats[5]
                );
            }
        }
    }

    for (int in = 0; in <= 1; in++) {

        test(
            not_gate(in) == !in,
            stats[1]
        );

        for (int sel = 0; sel <= 1; sel++) {

            bool a;
            bool b;

            dmux(in, sel, &a, &b);

            test(
                a == (in && !sel),
                stats[6]
            );

            test(
                b == (in && sel),
                stats[6]
            );
        }
    }

    // ========================================================
    // 2. 16-BIT CHIPS
    // ========================================================

    std::cout << "\n[2] 16-BIT CHIPS\n";

    // NOT16 / OR8WAY / OR16WAY
    for (uint32_t x = 0; x <= 0xFFFF; x++) {

        Word in = uint16ToWord(static_cast<uint16_t>(x));
        Word out{};

        not16(in, out);

        test(
            wordToUint16(out) ==
            static_cast<uint16_t>(~x),
            stats[7]
        );

        halfWord left{};
        halfWord right{};

        for (int i = 0; i < 8; i++) {
            left[i] = in[i];
            right[i] = in[i + 8];
        }

        bool expectedLeft = false;
        bool expectedRight = false;
        bool expected16 = false;

        for (bool bit : left)
            expectedLeft |= bit;

        for (bool bit : right)
            expectedRight |= bit;

        for (bool bit : in)
            expected16 |= bit;

        test(
            or8way(left) == expectedLeft,
            stats[11]
        );

        test(
            or8way(right) == expectedRight,
            stats[11]
        );

        test(
            or16way(in) == expected16,
            stats[12]
        );
    }

    // AND16 / OR16
    for (uint32_t x = 0; x <= 0xFFFF; x++) {

        Word A = uint16ToWord(static_cast<uint16_t>(x));
        Word B = uint16ToWord(static_cast<uint16_t>(~x));

        Word out{};

        and16(A, B, out);

        test(
            wordToUint16(out) == 0,
            stats[8]
        );

        or16(A, B, out);

        test(
            wordToUint16(out) == 0xFFFF,
            stats[9]
        );
    }

    // ========================================================
    // 3. MUX16
    // ========================================================

    std::cout << "\n[3] MULTIPLEXERS\n";

    for (int a = 0; a <= 1; a++) {
        for (int b = 0; b <= 1; b++) {
            for (int sel = 0; sel <= 1; sel++) {

                Word A{};
                Word B{};
                Word out{};

                A.fill(a);
                B.fill(b);

                mux16(A, B, sel, out);

                bool expected = sel ? b : a;

                for (int i = 0; i < 16; i++) {
                    test(
                        out[i] == expected,
                        stats[10]
                    );
                }
            }
        }
    }

    // ========================================================
    // 4. MUX4WAY16
    // ========================================================

    for (int value = 0; value < 16; value++) {

        Word inputs[4];

        for (int i = 0; i < 4; i++) {
            inputs[i].fill((value >> i) & 1);
        }

        for (int s = 0; s < 4; s++) {

            sel2 sel = {
                static_cast<bool>((s >> 1) & 1),
                static_cast<bool>(s & 1)
            };

            Word out{};

            mux4way16(
                inputs[0],
                inputs[1],
                inputs[2],
                inputs[3],
                sel,
                out
            );

            bool expected = inputs[s][0];

            for (int i = 0; i < 16; i++) {
                test(
                    out[i] == expected,
                    stats[13]
                );
            }
        }
    }

    // ========================================================
    // 5. MUX8WAY16
    // ========================================================

    for (int value = 0; value < 256; value++) {

        Word inputs[8];

        for (int i = 0; i < 8; i++) {
            inputs[i].fill((value >> i) & 1);
        }

        for (int s = 0; s < 8; s++) {

            sel3 sel = {
                static_cast<bool>((s >> 2) & 1),
                static_cast<bool>((s >> 1) & 1),
                static_cast<bool>(s & 1)
            };

            Word out{};

            mux8way16(
                inputs[0],
                inputs[1],
                inputs[2],
                inputs[3],
                inputs[4],
                inputs[5],
                inputs[6],
                inputs[7],
                sel,
                out
            );

            bool expected = inputs[s][0];

            for (int i = 0; i < 16; i++) {
                test(
                    out[i] == expected,
                    stats[14]
                );
            }
        }
    }

    // ========================================================
    // 6. DMUX4WAY / DMUX8WAY
    // ========================================================

    std::cout << "\n[4] DEMULTIPLEXERS\n";

    for (int in = 0; in <= 1; in++) {

        for (int s = 0; s < 4; s++) {

            sel2 sel = {
                static_cast<bool>((s >> 1) & 1),
                static_cast<bool>(s & 1)
            };

            bool a, b, c, d;

            dmux4way(
                in,
                &a, &b, &c, &d,
                sel
            );

            bool expected[4] = {false, false, false, false};

            expected[s] = in;

            test(a == expected[0], stats[15]);
            test(b == expected[1], stats[15]);
            test(c == expected[2], stats[15]);
            test(d == expected[3], stats[15]);
        }

        for (int s = 0; s < 8; s++) {

            sel3 sel = {
                static_cast<bool>((s >> 2) & 1),
                static_cast<bool>((s >> 1) & 1),
                static_cast<bool>(s & 1)
            };

            bool a, b, c, d;
            bool e, f, g, h;

            dmux8way(
                in,
                &a, &b, &c, &d,
                &e, &f, &g, &h,
                sel
            );

            bool expected[8] =
                {false, false, false, false,
                 false, false, false, false};

            expected[s] = in;

            test(a == expected[0], stats[16]);
            test(b == expected[1], stats[16]);
            test(c == expected[2], stats[16]);
            test(d == expected[3], stats[16]);
            test(e == expected[4], stats[16]);
            test(f == expected[5], stats[16]);
            test(g == expected[6], stats[17]);
            test(h == expected[7], stats[17]);
        }
    }

    // ========================================================
    // 7. ADDERS
    // ========================================================

    std::cout << "\n[5] ADDERS\n";

    // HALF ADDER
    for (int a = 0; a <= 1; a++) {
        for (int b = 0; b <= 1; b++) {

            bool sum;
            bool carry;

            halfadder(
                a,
                b,
                &sum,
                &carry
            );

            int expected = a + b;

            test(
                sum == (expected & 1),
                stats[17]
            );

            test(
                carry == ((expected >> 1) & 1),
                stats[17]
            );
        }
    }

    // FULL ADDER
    for (int a = 0; a <= 1; a++) {
        for (int b = 0; b <= 1; b++) {
            for (int carry = 0; carry <= 1; carry++) {

                bool sum;
                bool carryOut;

                fulladder(
                    a,
                    b,
                    carry,
                    &sum,
                    &carryOut
                );

                int expected =
                    a + b + carry;

                test(
                    sum == (expected & 1),
                    stats[18]
                );

                test(
                    carryOut == ((expected >> 1) & 1),
                    stats[18]
                );
            }
        }
    }

    // INC16
    std::cout << "\n[6] INC16\n";

    for (uint32_t x = 0; x <= 0xFFFF; x++) {

        Word X = uint16ToWord(
            static_cast<uint16_t>(x)
        );

        Word out{};

        inc16(X, out);

        test(
            wordToUint16(out) ==
            static_cast<uint16_t>(x + 1),
            stats[20]
        );
    }

    // ========================================================
    // RESULTS
    // ========================================================

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start);

    std::cout << "\n========================================\n";
    std::cout << "             TEST RESULTS\n";
    std::cout << "========================================\n";

    uint64_t totalPassed = 0;
    uint64_t totalFailed = 0;

    for (const auto& s : stats) {

        if (s.total() == 0)
            continue;

        printStats(s);

        totalPassed += s.passed;
        totalFailed += s.failed;
    }

    std::cout << "\n----------------------------------------\n";

    std::cout
        << "TOTAL PASSED : "
        << totalPassed
        << '\n';

    std::cout
        << "TOTAL FAILED : "
        << totalFailed
        << '\n';

    std::cout
        << "TOTAL TESTS  : "
        << totalPassed + totalFailed
        << '\n';

    std::cout
        << "TIME         : "
        << duration.count() / 1000.0
        << " ms\n";

    std::cout << "========================================\n";

    return totalFailed == 0 ? 0 : 1;
}
