#include <cstdio>

// External function declarations
extern bool dff(bool in);
extern bool bit(bool in, bool load);

struct TestResult {
    const char *name;
    bool got;
    bool expected;
};

int failures = 0;
int total = 0;

void check(const char *name, bool got, bool expected) {
    total++;
    printf("[%s] in=%-5s -> got=%-5s expected=%-5s  %s\n",
           name,
           "",
           got ? "true" : "false",
           expected ? "true" : "false",
           got == expected ? "PASS" : "FAIL");
    if (got != expected) failures++;
}

void test_dff() {
    printf("\n--- DFF tests ---\n");
    // DFF output should lag input by one cycle (returns previous state)
    check("dff cycle1", dff(true),  false); // initial state false, now set true
    check("dff cycle2", dff(true),  true);  // previous was true
    check("dff cycle3", dff(false), true);  // previous was true
    check("dff cycle4", dff(false), false); // previous was false
    check("dff cycle5", dff(true),  false); // previous was false
}

void test_bit() {
    printf("\n--- Bit (1-bit register) tests ---\n");
    // load=false should preserve previous state regardless of in
    check("bit no-load keeps false", bit(true, false), false);
    // load=true should latch in the new value
    check("bit load true", bit(true, true), true);
    // load=false again should hold the latched value
    check("bit hold after load", bit(false, false), true);
    // load=true with false should latch false
    check("bit load false", bit(false, true), false);
    // load=false should hold false
    check("bit hold false", bit(true, false), false);
}

int main() {
    test_dff();
    test_bit();

    printf("\n=== Results: %d/%d passed ===\n", total - failures, total);
    return failures == 0 ? 0 : 1;
}
