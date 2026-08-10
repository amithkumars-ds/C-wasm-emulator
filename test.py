import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from booleanArithmetic.arithmeticChips import HalfAdder, FullAdder, Add16, Inc16


# ============================================================
# Half Adder
# ============================================================

def test_HalfAdder():
    tests = [
        ("0", "0", ("0", "0")),
        ("0", "1", ("1", "0")),
        ("1", "0", ("1", "0")),
        ("1", "1", ("0", "1")),
    ]

    for i, (a, b, expected) in enumerate(tests, 1):
        result = HalfAdder(a, b)

        assert result == expected, (
            f"HALFADDER Test {i} failed\n"
            f"a        = {a}\n"
            f"b        = {b}\n"
            f"expected = {expected}\n"
            f"got      = {result}"
        )

    print("HALFADDER: Passed")


# ============================================================
# Full Adder
# ============================================================

def test_FullAdder():
    tests = [
        ("0", "0", "0", ("0", "0")),
        ("0", "0", "1", ("1", "0")),
        ("0", "1", "0", ("1", "0")),
        ("0", "1", "1", ("0", "1")),
        ("1", "0", "0", ("1", "0")),
        ("1", "0", "1", ("0", "1")),
        ("1", "1", "0", ("0", "1")),
        ("1", "1", "1", ("1", "1")),
    ]

    for i, (a, b, c, expected) in enumerate(tests, 1):
        result = FullAdder(a, b, c)

        assert result == expected, (
            f"FULLADDER Test {i} failed\n"
            f"a        = {a}\n"
            f"b        = {b}\n"
            f"c        = {c}\n"
            f"expected = {expected}\n"
            f"got      = {result}"
        )

    print("FULLADDER: Passed")


# ============================================================
# Add16
# ============================================================

def test_Add16():
    tests = [
        (
            "0000000000000000",
            "0000000000000000",
            "0000000000000000",
        ),
        (
            "0000000000000001",
            "0000000000000001",
            "0000000000000010",
        ),
        (
            "0000000000001010",
            "0000000000000101",
            "0000000000001111",
        ),
        (
            "1111111111111111",
            "0000000000000001",
            "0000000000000000",  # overflow discarded
        ),
        (
            "1010101010101010",
            "0101010101010101",
            "1111111111111111",
        ),
    ]

    for i, (a, b, expected) in enumerate(tests, 1):
        result = Add16(a, b)

        assert result == expected, (
            f"ADD16 Test {i} failed\n"
            f"a        = {a}\n"
            f"b        = {b}\n"
            f"expected = {expected}\n"
            f"got      = {result}"
        )

    print("ADD16: Passed")


# ============================================================
# Inc16
# ============================================================

def test_Inc16():
    tests = [
        (
            "0000000000000000",
            "0000000000000001",
        ),
        (
            "0000000000000001",
            "0000000000000010",
        ),
        (
            "0000000000001111",
            "0000000000010000",
        ),
        (
            "0111111111111111",
            "1000000000000000",
        ),
        (
            "1111111111111111",
            "0000000000000000",  # overflow discarded
        ),
    ]

    for i, (inp, expected) in enumerate(tests, 1):
        result = Inc16(inp)

        assert result == expected, (
            f"INC16 Test {i} failed\n"
            f"in       = {inp}\n"
            f"expected = {expected}\n"
            f"got      = {result}"
        )

    print("INC16: Passed")


# ============================================================
# Main
# ============================================================

if __name__ == "__main__":
    print("========== Project 2 ==========\n")

    test_HalfAdder()
    test_FullAdder()
    test_Add16()
    test_Inc16()

    print("\nAll Project 2 tests passed.")