from booleanLogic.elementaryChips import Not, And
from booleanLogic.multibitVariantChips import Mux16
from memory.memoryChips import RAM16K
from utilities.utils import (
    BitType,
    Word16,
    Word16_zero,
    _check_bits,
    _check_length,
)


class Screen:
    """
    Screen memory.
    8192 x 16-bit words.
    Address = 13 bits.
    """

    def __init__(self):
        self.memory = [Word16_zero] * 8192

    def input(self, inp: Word16, load: BitType, address: str):
        _check_length(inp, 16)
        _check_length(load, 1)
        _check_length(address, 13)

        _check_bits(*inp)
        _check_bits(load)
        _check_bits(*address)

        if load == "1":
            index = int(address, 2)
            self.memory[index] = inp

    def output(self, address: str) -> Word16:
        _check_length(address, 13)
        _check_bits(*address)

        index = int(address, 2)
        return self.memory[index]

    def tick(self):
        pass


class Keyboard:
    """
    Keyboard register.
    """

    def __init__(self):
        self.value = Word16_zero

    def set_key(self, value: Word16):
        _check_length(value, 16)
        _check_bits(*value)

        self.value = value

    def output(self) -> Word16:
        return self.value


class Memory:
    """
    Hack Memory chip.

    Address map:
        0x0000-0x3FFF : RAM16K
        0x4000-0x5FFF : Screen
        0x6000        : Keyboard
    """

    def __init__(self):
        self.ram = RAM16K()
        self.screen = Screen()
        self.keyboard = Keyboard()

    def input(self, inp: Word16, load: BitType, address: str):
        _check_length(inp, 16)
        _check_length(load, 1)
        _check_length(address, 15)

        _check_bits(*inp)
        _check_bits(load)
        _check_bits(*address)

        # -----------------------
        # RAM
        # -----------------------

        ramSel = Not(address[0])
        loadRam = And(ramSel, load)

        self.ram.input(
            inp=inp,
            load=loadRam,
            address=address[1:]
        )

        # -----------------------
        # Screen
        # -----------------------

        notA13 = Not(address[1])
        screenSel = And(address[0], notA13)
        loadScreen = And(screenSel, load)

        self.screen.input(
            inp=inp,
            load=loadScreen,
            address=address[2:]
        )

    def output(self, address: str) -> Word16:
        _check_length(address, 15)
        _check_bits(*address)

        ramOut = self.ram.output(address[1:])
        screenOut = self.screen.output(address[2:])
        keyOut = self.keyboard.output()

        ramOrScreen = Mux16(
            a=ramOut,
            b=screenOut,
            sel=address[0]
        )

        keySel = And(address[0], address[1])

        out = Mux16(
            a=ramOrScreen,
            b=keyOut,
            sel=keySel
        )

        return out

    def tick(self):
        self.ram.tick()
        self.screen.tick()