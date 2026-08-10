import os
import sys
import random

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from memory.memoryChips import RAM16K
from utilities.utils import Word16_zero


def rand_bit() -> str:
    return random.choice("01")


def rand_word16() -> str:
    return "".join(rand_bit() for _ in range(16))


def rand_address14() -> str:
    return "".join(rand_bit() for _ in range(14))


def test_RAM16K():
    ram = RAM16K()

    # Sparse reference memory
    reference = {}

    print("========== RAM16K ==========\n")

    # -------------------------------------------------
    # Initial memory should be zero
    # -------------------------------------------------
    for _ in range(20):
        address = rand_address14()

        expected = Word16_zero
        got = ram.output(address)

        assert got == expected, (
            f"\nInitial Read Failed\n"
            f"Address  : {address}\n"
            f"Expected : {expected}\n"
            f"Got      : {got}"
        )

    print("Initial memory: Passed")

    # -------------------------------------------------
    # Random writes
    # -------------------------------------------------
    for _ in range(200):
        address = rand_address14()
        value = rand_word16()

        ram.input(
            inp=value,
            load="1",
            address=address
        )
        ram.tick()

        reference[address] = value

    print("Random writes: Passed")

    # -------------------------------------------------
    # Random reads
    # -------------------------------------------------
    for _ in range(200):
        address = rand_address14()

        expected = reference.get(address, Word16_zero)
        got = ram.output(address)

        assert got == expected, (
            f"\nRead Failed\n"
            f"Address  : {address}\n"
            f"Expected : {expected}\n"
            f"Got      : {got}"
        )

    print("Random reads: Passed")

    # -------------------------------------------------
    # load = 0 should not overwrite memory
    # -------------------------------------------------
    for _ in range(50):
        address = rand_address14()

        original = reference.get(address, Word16_zero)
        new_value = rand_word16()

        ram.input(
            inp=new_value,
            load="0",
            address=address
        )
        ram.tick()

        got = ram.output(address)

        assert got == original, (
            f"\nLoad=0 Failed\n"
            f"Address  : {address}\n"
            f"Expected : {original}\n"
            f"Got      : {got}"
        )

    print("Load=0: Passed")

    # -------------------------------------------------
    # Overwrite same address
    # -------------------------------------------------
    for _ in range(50):
        address = rand_address14()

        first = rand_word16()
        second = rand_word16()

        ram.input(first, "1", address)
        ram.tick()

        ram.input(second, "1", address)
        ram.tick()

        reference[address] = second

        got = ram.output(address)

        assert got == second, (
            f"\nOverwrite Failed\n"
            f"Address  : {address}\n"
            f"Expected : {second}\n"
            f"Got      : {got}"
        )

    print("Overwrite: Passed")

    print("\nRAM16K PASSED ALL TESTS")


if __name__ == "__main__":
    test_RAM16K()