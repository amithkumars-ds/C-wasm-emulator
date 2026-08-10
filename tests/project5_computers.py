import os
import sys
import random

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from computer.parts import Memory
from utilities.utils import Word16_zero


def rand_bit():
    return random.choice("01")


def rand_word16():
    return "".join(rand_bit() for _ in range(16))


def rand_ram_address():
    # A14 = 0
    return "0" + "".join(rand_bit() for _ in range(14))


def rand_screen_address():
    # A14 = 1, A13 = 0
    return "10" + "".join(rand_bit() for _ in range(13))


def keyboard_address():
    # 0x6000 = 110000000000000
    return "110000000000000"


def test_memory():

    mem = Memory()

    print("========== Memory Chip ==========\n")

    # ---------------------------------------------------
    # Initial RAM
    # ---------------------------------------------------

    for _ in range(20):
        addr = rand_ram_address()

        got = mem.output(addr)

        assert got == Word16_zero, (
            f"RAM Initial Read Failed\n"
            f"Address  : {addr}\n"
            f"Expected : {Word16_zero}\n"
            f"Got      : {got}"
        )

    print("Initial RAM: Passed")

    # ---------------------------------------------------
    # RAM write/read
    # ---------------------------------------------------

    for _ in range(100):
        addr = rand_ram_address()
        value = rand_word16()

        mem.input(value, "1", addr)
        mem.tick()

        got = mem.output(addr)

        assert got == value, (
            f"RAM Write Failed\n"
            f"Address  : {addr}\n"
            f"Expected : {value}\n"
            f"Got      : {got}"
        )

    print("RAM write/read: Passed")

    # ---------------------------------------------------
    # RAM load = 0
    # ---------------------------------------------------

    for _ in range(50):
        addr = rand_ram_address()

        original = mem.output(addr)
        new_value = rand_word16()

        mem.input(new_value, "0", addr)
        mem.tick()

        got = mem.output(addr)

        assert got == original, (
            f"RAM load=0 Failed\n"
            f"Address  : {addr}\n"
            f"Expected : {original}\n"
            f"Got      : {got}"
        )

    print("RAM load=0: Passed")

    # ---------------------------------------------------
    # Screen write/read
    # ---------------------------------------------------

    for _ in range(100):
        addr = rand_screen_address()
        value = rand_word16()

        mem.input(value, "1", addr)
        mem.tick()

        got = mem.output(addr)

        assert got == value, (
            f"Screen Write Failed\n"
            f"Address  : {addr}\n"
            f"Expected : {value}\n"
            f"Got      : {got}"
        )

    print("Screen write/read: Passed")

    # ---------------------------------------------------
    # Keyboard
    # ---------------------------------------------------

    key_value = rand_word16()

    mem.keyboard.set_key(key_value)

    got = mem.output(keyboard_address())

    assert got == key_value, (
        f"Keyboard Failed\n"
        f"Expected : {key_value}\n"
        f"Got      : {got}"
    )

    print("Keyboard: Passed")

    # ---------------------------------------------------
    # Isolation test
    # ---------------------------------------------------

    ram_addr = rand_ram_address()
    screen_addr = rand_screen_address()

    ram_value = rand_word16()
    screen_value = rand_word16()

    mem.input(ram_value, "1", ram_addr)
    mem.tick()

    mem.input(screen_value, "1", screen_addr)
    mem.tick()

    assert mem.output(ram_addr) == ram_value
    assert mem.output(screen_addr) == screen_value

    print("RAM/Screen isolation: Passed")

    print("\nAll Memory tests passed.")


if __name__ == "__main__":
    test_memory()