#include <array>
#include <chrono>
#include <cstdio>

#include "chips.h"

using Clock = std::chrono::high_resolution_clock;

bool words_equal(const Word& a, const Word& b) {
    for (int i = 0; i < 16; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

Word int_to_word(unsigned int value) {
    Word word{};

    // index 0 = MSB, index 15 = LSB
    for (int i = 0; i < 16; i++) {
        word[i] = (value >> (15 - i)) & 1;
    }

    return word;
}

sel3 int_to_sel3(int address) {
    sel3 sel{};

    // sel[0] = MSB, sel[2] = LSB
    sel[0] = (address >> 2) & 1;
    sel[1] = (address >> 1) & 1;
    sel[2] = address & 1;

    return sel;
}

void print_word(const Word& word) {
    for (bool bit : word) {
        printf("%d", bit);
    }
}

int main() {
    unsigned long long tests = 0;
    unsigned long long passed = 0;

    auto total_start = Clock::now();

    std::array<Word, 8> expected{};

    // ================================================================
    // TEST 1: Initial state
    // ================================================================

    auto start = Clock::now();

    Word zero{};

    for (int address = 0; address < 8; address++) {
        sel3 sel = int_to_sel3(address);

        Word actual = ram8(zero, false, sel);

        tests++;

        if (words_equal(actual, zero)) {
            passed++;
        } else {
            printf("FAIL: Initial state, address %d\n", address);
            printf("Expected: ");
            print_word(zero);
            printf("\nActual:   ");
            print_word(actual);
            printf("\n");
            return 1;
        }
    }

    auto end = Clock::now();

    printf("Test 1 - Initial state: PASS\n");
    printf("Time: %.3f ms\n\n",
           std::chrono::duration<double, std::milli>(end - start).count());

    // ================================================================
    // TEST 2: Exhaustive writes + isolation
    //
    // 8 addresses × 65536 values
    // ================================================================

    start = Clock::now();

    for (int address = 0; address < 8; address++) {
        sel3 sel = int_to_sel3(address);

        for (unsigned int value = 0; value <= 0xFFFF; value++) {
            Word input = int_to_word(value);

            // Write value.
            Word actual = ram8(input, true, sel);

            expected[address] = input;

            tests++;

            if (words_equal(actual, expected[address])) {
                passed++;
            } else {
                printf("FAIL: Write\n");
                printf("Address: %d\n", address);
                printf("Value: 0x%04X\n", value);
                printf("Expected: ");
                print_word(expected[address]);
                printf("\nActual:   ");
                print_word(actual);
                printf("\n");
                return 1;
            }

            // Verify all 8 addresses.
            for (int other = 0; other < 8; other++) {
                sel3 other_sel = int_to_sel3(other);

                Word read = ram8(zero, false, other_sel);

                tests++;

                if (words_equal(read, expected[other])) {
                    passed++;
                } else {
                    printf("FAIL: Memory isolation\n");
                    printf("Written address: %d\n", address);
                    printf("Read address:    %d\n", other);
                    printf("Written value:   0x%04X\n", value);

                    printf("Expected: ");
                    print_word(expected[other]);
                    printf("\nActual:   ");
                    print_word(read);
                    printf("\n");

                    return 1;
                }
            }
        }
    }

    end = Clock::now();

    printf("Test 2 - Exhaustive writes + isolation: PASS\n");
    printf("Time: %.3f ms\n\n",
           std::chrono::duration<double, std::milli>(end - start).count());

    // ================================================================
    // TEST 3: Exhaustive load=false
    //
    // Input changes through all 65536 values, but RAM must not change.
    // ================================================================

    start = Clock::now();

    for (int address = 0; address < 8; address++) {
        sel3 sel = int_to_sel3(address);

        for (unsigned int value = 0; value <= 0xFFFF; value++) {
            Word input = int_to_word(value);

            Word actual = ram8(input, false, sel);

            tests++;

            if (words_equal(actual, expected[address])) {
                passed++;
            } else {
                printf("FAIL: load=false modified RAM\n");
                printf("Address: %d\n", address);
                printf("Input: 0x%04X\n", value);

                printf("Expected: ");
                print_word(expected[address]);
                printf("\nActual:   ");
                print_word(actual);
                printf("\n");

                return 1;
            }
        }
    }

    end = Clock::now();

    printf("Test 3 - load=false: PASS\n");
    printf("Time: %.3f ms\n\n",
           std::chrono::duration<double, std::milli>(end - start).count());

    // ================================================================
    // FINAL RESULT
    // ================================================================

    auto total_end = Clock::now();

    double total_ms =
        std::chrono::duration<double, std::milli>(
            total_end - total_start
        ).count();

    printf("========================================\n");
    printf("RAM8 EXHAUSTIVE TEST PASSED\n");
    printf("Tests passed: %llu/%llu\n", passed, tests);
    printf("Total time: %.3f ms\n", total_ms);
    printf("========================================\n");

    return 0;
}
