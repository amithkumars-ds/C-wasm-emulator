#include <cstdio>
#include <cstdint>
#include <chrono>
#include <array>

#include "chips.h"
#include "memory.h"

using Clock = std::chrono::high_resolution_clock;

static uint64_t passed = 0;
static uint64_t failed = 0;

static sel14 make_address(uint16_t value) {
    sel14 address{};

    // address[0] = least significant address bit
    for (int i = 0; i < 14; i++) {
        address[i] = (value >> i) & 1;
    }

    return address;
}

static Word make_word(uint16_t value) {
    Word word{};

    // word[15] = LSB
    for (int i = 0; i < 16; i++) {
        word[15 - i] = (value >> i) & 1;
    }

    return word;
}

static uint16_t word_to_uint16(const Word& word) {
    uint16_t value = 0;

    for (int i = 0; i < 16; i++) {
        value <<= 1;
        value |= word[i];
    }

    return value;
}

static void print_word(const Word& word) {
    for (bool bit : word) {
        std::printf("%d", bit);
    }
}

static void check_word(
    const Word& actual,
    const Word& expected,
    uint16_t address,
    const char* test_name
) {
    if (actual == expected) {
        passed++;
        return;
    }

    failed++;

    std::printf("\nFAIL: %s\n", test_name);
    std::printf("Address: 0x%04X (%u)\n", address, address);

    std::printf("Expected: ");
    print_word(expected);
    std::printf("\n");

    std::printf("Actual:   ");
    print_word(actual);
    std::printf("\n");
}


// ============================================================
// TEST 1
// Initial state
// ============================================================

static void test_initial_state() {
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
    > state{};

    Word zero{};
    sel14 address{};

    bool ok = true;

    for (uint32_t i = 0; i < 16384; i++) {
        address = make_address(i);

        Word actual = ram16k(
            zero,
            false,
            address,
            state
        );

        if (actual != zero) {
            ok = false;

            std::printf(
                "FAIL: Initial state at address %u\n",
                i
            );

            break;
        }
    }

    if (ok) {
        passed++;
        std::printf("Test 1 - Initial state: PASS\n");
    } else {
        failed++;
        std::printf("Test 1 - Initial state: FAIL\n");
    }
}


// ============================================================
// TEST 2
// Exhaustive address write/read
// Every one of 16,384 addresses
// ============================================================

static void test_exhaustive_write_read() {
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
    > state{};

    const uint16_t patterns[] = {
        0x0000,
        0x0001,
        0xFFFF,
        0xAAAA,
        0x5555,
        0x1234,
        0xABCD,
        0x8000,
        0x0002,
        0x7FFF
    };

    uint64_t writes = 0;
    uint64_t reads = 0;

    auto start = Clock::now();

    // Write every pattern to every address.
    for (uint16_t pattern : patterns) {
        Word input = make_word(pattern);

        for (uint32_t address_value = 0;
             address_value < 16384;
             address_value++) {

            sel14 address = make_address(address_value);

            ram16k(
                input,
                true,
                address,
                state
            );

            writes++;
        }
    }

    // Read every address after each complete pattern pass.
    // Only the final pattern should remain.
    Word expected = make_word(patterns[9]);

    for (uint32_t address_value = 0;
         address_value < 16384;
         address_value++) {

        sel14 address = make_address(address_value);

        Word actual = ram16k(
            make_word(0),
            false,
            address,
            state
        );

        reads++;

        if (actual != expected) {
            failed++;

            std::printf(
                "\nFAIL: Exhaustive write/read\n"
                "Address: 0x%04X (%u)\n"
                "Expected: 0x%04X\n"
                "Actual:   0x%04X\n",
                address_value,
                address_value,
                patterns[9],
                word_to_uint16(actual)
            );

            auto end = Clock::now();

            double seconds =
                std::chrono::duration<double>(end - start).count();

            std::printf(
                "Writes: %llu\n"
                "Reads:  %llu\n"
                "Time: %.3f s\n",
                (unsigned long long)writes,
                (unsigned long long)reads,
                seconds
            );

            return;
        }
    }

    passed++;

    auto end = Clock::now();

    double seconds =
        std::chrono::duration<double>(end - start).count();

    std::printf("\nTest 2 - Exhaustive write/read: PASS\n");
    std::printf("Addresses: 16,384\n");
    std::printf("Patterns:  %zu\n",
                sizeof(patterns) / sizeof(patterns[0]));
    std::printf("Writes:    %llu\n",
                (unsigned long long)writes);
    std::printf("Reads:     %llu\n",
                (unsigned long long)reads);
    std::printf("Time:      %.3f s\n", seconds);
}


// ============================================================
// TEST 3
// Address isolation
// ============================================================

static void test_isolation() {
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
    > state{};

    auto start = Clock::now();

    // Give every address a unique value.
    for (uint32_t i = 0; i < 16384; i++) {
        sel14 address = make_address(i);

        uint16_t value =
            static_cast<uint16_t>(
                ((i * 251) + 12345) & 0xFFFF
            );

        ram16k(
            make_word(value),
            true,
            address,
            state
        );
    }

    // Verify every address independently.
    for (uint32_t i = 0; i < 16384; i++) {
        sel14 address = make_address(i);

        uint16_t expected_value =
            static_cast<uint16_t>(
                ((i * 251) + 12345) & 0xFFFF
            );

        Word actual = ram16k(
            make_word(0),
            false,
            address,
            state
        );

        Word expected = make_word(expected_value);

        if (actual != expected) {
            failed++;

            std::printf(
                "\nFAIL: RAM16K isolation\n"
                "Address: 0x%04X (%u)\n"
                "Expected: 0x%04X\n"
                "Actual:   0x%04X\n",
                i,
                i,
                expected_value,
                word_to_uint16(actual)
            );

            return;
        }
    }

    passed++;

    auto end = Clock::now();

    double seconds =
        std::chrono::duration<double>(end - start).count();

    std::printf("\nTest 3 - RAM16K isolation: PASS\n");
    std::printf("Addresses: 16,384\n");
    std::printf("Time: %.3f s\n", seconds);
}


// ============================================================
// TEST 4
// load=false must not modify memory
// ============================================================

static void test_load_false() {
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
    > state{};

    auto start = Clock::now();

    // Initialize every address.
    for (uint32_t i = 0; i < 16384; i++) {
        uint16_t value =
            static_cast<uint16_t>(
                (i * 37 + 0x1234) & 0xFFFF
            );

        ram16k(
            make_word(value),
            true,
            make_address(i),
            state
        );
    }

    // Attempt to overwrite using load=false.
    for (uint32_t i = 0; i < 16384; i++) {
        ram16k(
            make_word(0xFFFF),
            false,
            make_address(i),
            state
        );
    }

    // Verify nothing changed.
    for (uint32_t i = 0; i < 16384; i++) {
        uint16_t expected_value =
            static_cast<uint16_t>(
                (i * 37 + 0x1234) & 0xFFFF
            );

        Word actual = ram16k(
            make_word(0),
            false,
            make_address(i),
            state
        );

        if (word_to_uint16(actual) != expected_value) {
            failed++;

            std::printf(
                "\nFAIL: load=false protection\n"
                "Address: 0x%04X (%u)\n"
                "Expected: 0x%04X\n"
                "Actual:   0x%04X\n",
                i,
                i,
                expected_value,
                word_to_uint16(actual)
            );

            return;
        }
    }

    passed++;

    auto end = Clock::now();

    double seconds =
        std::chrono::duration<double>(end - start).count();

    std::printf("\nTest 4 - load=false protection: PASS\n");
    std::printf("Addresses: 16,384\n");
    std::printf("Time: %.3f s\n", seconds);
}


// ============================================================
// MAIN
// ============================================================

int main() {
    std::printf("========================================\n");
    std::printf("        RAM16K EXHAUSTIVE TEST\n");
    std::printf("========================================\n");
    std::printf("Capacity: 16,384 x 16-bit words\n");
    std::printf("Total bits: 262,144\n\n");

    auto total_start = Clock::now();

    test_initial_state();
    test_exhaustive_write_read();
    test_isolation();
    test_load_false();

    auto total_end = Clock::now();

    double total_seconds =
        std::chrono::duration<double>(
            total_end - total_start
        ).count();

    std::printf("\n========================================\n");
    std::printf("                 REPORT\n");
    std::printf("========================================\n");

    std::printf(
        "Tests passed: %llu\n",
        (unsigned long long)passed
    );

    std::printf(
        "Tests failed: %llu\n",
        (unsigned long long)failed
    );

    std::printf(
        "Total time:   %.3f s\n",
        total_seconds
    );

    if (failed == 0) {
        std::printf("RESULT: PASS\n");
        return 0;
    }

    std::printf("RESULT: FAIL\n");
    return 1;
}
