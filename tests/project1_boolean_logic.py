import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from booleanLogic.elementaryChips import Not, And, Or, Xor, Mux, DeMux
from booleanLogic.multibitVariantChips import Not16, And16, Or16, Mux16
from booleanLogic.multiWayVariantChips import Mux4Way16, Mux8Way16, DMux4Way, DMux8Way


# ============================================================
# 1a. Elementary Chips
# ============================================================

def test_Not():
    tests = [
        ("0", "1"),
        ("1", "0"),
    ]

    for i, (a, expected) in enumerate(tests, 1):
        result = Not(a)
        assert result == expected, (
            f"NOT Test {i} failed\n"
            f"in       = {a}\n"
            f"expected = {expected}\n"
            f"got      = {result}"
        )

    print("NOT: Passed")


def test_And():
    tests = [
        ("0", "0", "0"),
        ("0", "1", "0"),
        ("1", "0", "0"),
        ("1", "1", "1"),
    ]

    for i, (a, b, expected) in enumerate(tests, 1):
        result = And(a, b)
        assert result == expected, f"AND Test {i} failed"

    print("AND: Passed")


def test_Or():
    tests = [
        ("0", "0", "0"),
        ("0", "1", "1"),
        ("1", "0", "1"),
        ("1", "1", "1"),
    ]

    for i, (a, b, expected) in enumerate(tests, 1):
        result = Or(a, b)
        assert result == expected, f"OR Test {i} failed"

    print("OR: Passed")


def test_Xor():
    tests = [
        ("0", "0", "0"),
        ("0", "1", "1"),
        ("1", "0", "1"),
        ("1", "1", "0"),
    ]

    for i, (a, b, expected) in enumerate(tests, 1):
        result = Xor(a, b)
        assert result == expected, f"XOR Test {i} failed"

    print("XOR: Passed")


def test_Mux():
    tests = [
        ("0", "1", "0", "0"),
        ("0", "1", "1", "1"),
        ("1", "0", "0", "1"),
        ("1", "0", "1", "0"),
    ]

    for i, (a, b, sel, expected) in enumerate(tests, 1):
        result = Mux(a, b, sel)
        assert result == expected, f"MUX Test {i} failed"

    print("MUX: Passed")


def test_DeMux():
    tests = [
        ("0", "0", ("0", "0")),
        ("1", "0", ("1", "0")),
        ("0", "1", ("0", "0")),
        ("1", "1", ("0", "1")),
    ]

    for i, (D, sel, expected) in enumerate(tests, 1):
        result = DeMux(D, sel)
        assert result == expected, f"DEMUX Test {i} failed"

    print("DEMUX: Passed")


# ============================================================
# 1b. Multi-bit Chips
# ============================================================

A = "1010101010101010"
B = "1100110011001100"


def test_Not16():
    expected = "0101010101010101"
    result = Not16(A)
    assert result == expected, "NOT16 failed"
    print("NOT16: Passed")


def test_And16():
    expected = "1000100010001000"
    result = And16(A, B)
    assert result == expected, "AND16 failed"
    print("AND16: Passed")


def test_Or16():
    expected = "1110111011101110"
    result = Or16(A, B)
    assert result == expected, "OR16 failed"
    print("OR16: Passed")


def test_Mux16():
    tests = [
        (A, B, "0", A),
        (A, B, "1", B),
    ]

    for i, (a, b, sel, expected) in enumerate(tests, 1):
        result = Mux16(a, b, sel)
        assert result == expected, f"MUX16 Test {i} failed"

    print("MUX16: Passed")

# ============================================================
# 1c. Multi-Way Chips
# ============================================================

def test_Mux4Way16():
    tests = [
        ("1111111111111111", "1010101010101010", "1001100110011001", "1100110011001100", "00", "1111111111111111"),
        ("1111111111111111", "1010101010101010", "1001100110011001", "1100110011001100", "01", "1010101010101010"),
        ("1111111111111111", "1010101010101010", "1001100110011001", "1100110011001100", "10", "1001100110011001"),
        ("1111111111111111", "1010101010101010", "1001100110011001", "1100110011001100", "11", "1100110011001100"),
    ]

    for i, (a, b, c, d, sel, expected) in enumerate(tests, 1):
        result = Mux4Way16(a, b, c, d, sel)
        assert result == expected, (
            f"Test {i} failed\n"
            f"sel      = {sel}\n"
            f"expected = {expected}\n"
            f"got      = {result}"
        )

    print(f"Passed {len(tests)} tests.")


def test_Mux8Way16():
    tests = [
        (
            "0000000000000000",
            "0000000000000001",
            "0000000000000010",
            "0000000000000011",
            "0000000000000100",
            "0000000000000101",
            "0000000000000110",
            "0000000000000111",
            "000",
            "0000000000000000",
        ),
        (
            "0000000000000000",
            "0000000000000001",
            "0000000000000010",
            "0000000000000011",
            "0000000000000100",
            "0000000000000101",
            "0000000000000110",
            "0000000000000111",
            "001",
            "0000000000000001",
        ),
        (
            "0000000000000000",
            "0000000000000001",
            "0000000000000010",
            "0000000000000011",
            "0000000000000100",
            "0000000000000101",
            "0000000000000110",
            "0000000000000111",
            "010",
            "0000000000000010",
        ),
        (
            "0000000000000000",
            "0000000000000001",
            "0000000000000010",
            "0000000000000011",
            "0000000000000100",
            "0000000000000101",
            "0000000000000110",
            "0000000000000111",
            "011",
            "0000000000000011",
        ),
        (
            "0000000000000000",
            "0000000000000001",
            "0000000000000010",
            "0000000000000011",
            "0000000000000100",
            "0000000000000101",
            "0000000000000110",
            "0000000000000111",
            "100",
            "0000000000000100",
        ),
        (
            "0000000000000000",
            "0000000000000001",
            "0000000000000010",
            "0000000000000011",
            "0000000000000100",
            "0000000000000101",
            "0000000000000110",
            "0000000000000111",
            "101",
            "0000000000000101",
        ),
        (
            "0000000000000000",
            "0000000000000001",
            "0000000000000010",
            "0000000000000011",
            "0000000000000100",
            "0000000000000101",
            "0000000000000110",
            "0000000000000111",
            "110",
            "0000000000000110",
        ),
        (
            "0000000000000000",
            "0000000000000001",
            "0000000000000010",
            "0000000000000011",
            "0000000000000100",
            "0000000000000101",
            "0000000000000110",
            "0000000000000111",
            "111",
            "0000000000000111",
        ),
    ]

    for i, (
        a, b, c, d, e, f, g, h,
        sel, expected
    ) in enumerate(tests, 1):
        result = Mux8Way16(a, b, c, d, e, f, g, h, sel)

        assert result == expected, (
            f"MUX8WAY16 Test {i} failed\n"
            f"sel      = {sel}\n"
            f"expected = {expected}\n"
            f"got      = {result}"
        )

    print("MUX8WAY16: Passed")

def test_DMux4Way():
    tests = [
        ("0", "00", ("0", "0", "0", "0")),
        ("1", "00", ("1", "0", "0", "0")),

        ("0", "01", ("0", "0", "0", "0")),
        ("1", "01", ("0", "1", "0", "0")),

        ("0", "10", ("0", "0", "0", "0")),
        ("1", "10", ("0", "0", "1", "0")),

        ("0", "11", ("0", "0", "0", "0")),
        ("1", "11", ("0", "0", "0", "1")),
    ]

    for i, (D, sel, expected) in enumerate(tests, 1):
        result = DMux4Way(D, sel)

        assert result == expected, (
            f"DMUX4WAY Test {i} failed\n"
            f"D        = {D}\n"
            f"sel      = {sel}\n"
            f"expected = {expected}\n"
            f"got      = {result}"
        )

    print("DMUX4WAY: Passed")

def test_DMux8Way():
    tests = [
        ("0", "000", ("0", "0", "0", "0", "0", "0", "0", "0")),
        ("1", "000", ("1", "0", "0", "0", "0", "0", "0", "0")),

        ("0", "001", ("0", "0", "0", "0", "0", "0", "0", "0")),
        ("1", "001", ("0", "1", "0", "0", "0", "0", "0", "0")),

        ("0", "010", ("0", "0", "0", "0", "0", "0", "0", "0")),
        ("1", "010", ("0", "0", "1", "0", "0", "0", "0", "0")),

        ("0", "011", ("0", "0", "0", "0", "0", "0", "0", "0")),
        ("1", "011", ("0", "0", "0", "1", "0", "0", "0", "0")),

        ("0", "100", ("0", "0", "0", "0", "0", "0", "0", "0")),
        ("1", "100", ("0", "0", "0", "0", "1", "0", "0", "0")),

        ("0", "101", ("0", "0", "0", "0", "0", "0", "0", "0")),
        ("1", "101", ("0", "0", "0", "0", "0", "1", "0", "0")),

        ("0", "110", ("0", "0", "0", "0", "0", "0", "0", "0")),
        ("1", "110", ("0", "0", "0", "0", "0", "0", "1", "0")),

        ("0", "111", ("0", "0", "0", "0", "0", "0", "0", "0")),
        ("1", "111", ("0", "0", "0", "0", "0", "0", "0", "1")),
    ]

    for i, (D, sel, expected) in enumerate(tests, 1):
        result = DMux8Way(D, sel)

        assert result == expected, (
            f"DMUX8WAY Test {i} failed\n"
            f"D        = {D}\n"
            f"sel      = {sel}\n"
            f"expected = {expected}\n"
            f"got      = {result}"
        )

    print("DMUX8WAY: Passed")
    
# ============================================================
# Main
# ============================================================

if __name__ == "__main__":
    print("========== Elementary Chips ==========")
    test_Not()
    test_And()
    test_Or()
    test_Xor()
    test_Mux()
    test_DeMux()

    print("\n========== Multi-bit Chips ==========")
    test_Not16()
    test_And16()
    test_Or16()
    test_Mux16()

    print("\nAll tests passed.")

    print("========== Multi-Way Chips ==========")
    test_Mux4Way16()
    test_Mux8Way16()
    test_DMux4Way()
    test_DMux8Way()