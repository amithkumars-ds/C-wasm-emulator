from booleanLogic.elementaryChips import Mux
from booleanLogic.multiWayVariantChips import DMux4Way, DMux8Way, Mux4Way16, Mux8Way16
from utilities.utils import BitType, Word16, _check_bits, _check_length, sel3, sel6, sel9, sel12, sel14


class DFF:
    def __init__(self):
        self.current = "0"
        self.next = "0"

    def input(self, D: BitType):
        _check_bits(D)
        self.next = D

    def output(self) -> BitType:
        return self.current

    def tick(self):
        self.current = self.next


class Bit:
    def __init__(self):
        self.dff = DFF()

    def input(self, inp: BitType, load: BitType):
        _check_bits(inp, load)

        d = Mux(
            a=self.dff.output(),
            b=inp,
            sel=load
        )

        self.dff.input(d)

    def output(self) -> BitType:
        return self.dff.output()

    def tick(self):
        self.dff.tick()


class Register16:
    def __init__(self):
        self.bits = [Bit() for _ in range(16)]

    def input(self, inp: Word16, load: BitType):
        _check_length(inp, 16)
        _check_bits(*inp)
        _check_bits(load)

        for i in range(16):
            self.bits[i].input(inp[i], load)

    def output(self) -> Word16:
        return "".join(bit.output() for bit in self.bits)

    def tick(self):
        for bit in self.bits:
            bit.tick()


class RAM8:
    def __init__(self):
        self.registers = [Register16() for _ in range(8)]

class RAM8:
    def __init__(self):
        self.registers = [Register16() for _ in range(8)]

    def input(self, inp: Word16, load: BitType, address: sel3):
        la, lb, lc, ld, le, lf, lg, lh = DMux8Way(load, address)

        self.registers[0].input(inp, la)
        self.registers[1].input(inp, lb)
        self.registers[2].input(inp, lc)
        self.registers[3].input(inp, ld)
        self.registers[4].input(inp, le)
        self.registers[5].input(inp, lf)
        self.registers[6].input(inp, lg)
        self.registers[7].input(inp, lh)

    def output(self, address: sel3) -> Word16:
        return Mux8Way16(
            self.registers[0].output(),
            self.registers[1].output(),
            self.registers[2].output(),
            self.registers[3].output(),
            self.registers[4].output(),
            self.registers[5].output(),
            self.registers[6].output(),
            self.registers[7].output(),
            address,
        )

    def tick(self):
        for reg in self.registers:
            reg.tick()

class RAM64:
    def __init__(self):
        self.rams = [RAM8() for _ in range(8)]

    def input(self, inp: Word16, load: BitType, address: sel6):
        upper = address[:3]
        lower = address[3:]

        la, lb, lc, ld, le, lf, lg, lh = DMux8Way(load, upper)

        self.rams[0].input(inp, la, lower)
        self.rams[1].input(inp, lb, lower)
        self.rams[2].input(inp, lc, lower)
        self.rams[3].input(inp, ld, lower)
        self.rams[4].input(inp, le, lower)
        self.rams[5].input(inp, lf, lower)
        self.rams[6].input(inp, lg, lower)
        self.rams[7].input(inp, lh, lower)

    def output(self, address: sel6) -> Word16:
        upper = address[:3]
        lower = address[3:]

        return Mux8Way16(
            self.rams[0].output(lower),
            self.rams[1].output(lower),
            self.rams[2].output(lower),
            self.rams[3].output(lower),
            self.rams[4].output(lower),
            self.rams[5].output(lower),
            self.rams[6].output(lower),
            self.rams[7].output(lower),
            upper,
        )

    def tick(self):
        for ram in self.rams:
            ram.tick()


class RAM512:
    def __init__(self):
        self.rams = [RAM64() for _ in range(8)]

    def input(self, inp: Word16, load: BitType, address: sel9):
        upper = address[:3]
        lower = address[3:]

        la, lb, lc, ld, le, lf, lg, lh = DMux8Way(load, upper)

        self.rams[0].input(inp, la, lower)
        self.rams[1].input(inp, lb, lower)
        self.rams[2].input(inp, lc, lower)
        self.rams[3].input(inp, ld, lower)
        self.rams[4].input(inp, le, lower)
        self.rams[5].input(inp, lf, lower)
        self.rams[6].input(inp, lg, lower)
        self.rams[7].input(inp, lh, lower)

    def output(self, address: sel9) -> Word16:
        upper = address[:3]
        lower = address[3:]

        return Mux8Way16(
            self.rams[0].output(lower),
            self.rams[1].output(lower),
            self.rams[2].output(lower),
            self.rams[3].output(lower),
            self.rams[4].output(lower),
            self.rams[5].output(lower),
            self.rams[6].output(lower),
            self.rams[7].output(lower),
            upper,
        )

    def tick(self):
        for ram in self.rams:
            ram.tick()


class RAM4K:
    def __init__(self):
        self.rams = [RAM512() for _ in range(8)]

    def input(self, inp: Word16, load: BitType, address: sel12):
        upper = address[:3]
        lower = address[3:]

        la, lb, lc, ld, le, lf, lg, lh = DMux8Way(load, upper)

        self.rams[0].input(inp, la, lower)
        self.rams[1].input(inp, lb, lower)
        self.rams[2].input(inp, lc, lower)
        self.rams[3].input(inp, ld, lower)
        self.rams[4].input(inp, le, lower)
        self.rams[5].input(inp, lf, lower)
        self.rams[6].input(inp, lg, lower)
        self.rams[7].input(inp, lh, lower)

    def output(self, address: sel12) -> Word16:
        upper = address[:3]
        lower = address[3:]

        return Mux8Way16(
            self.rams[0].output(lower),
            self.rams[1].output(lower),
            self.rams[2].output(lower),
            self.rams[3].output(lower),
            self.rams[4].output(lower),
            self.rams[5].output(lower),
            self.rams[6].output(lower),
            self.rams[7].output(lower),
            upper,
        )

    def tick(self):
        for ram in self.rams:
            ram.tick()


class RAM16K:
    def __init__(self):
        self.rams = [RAM4K() for _ in range(4)]

    def input(self, inp: Word16, load: BitType, address: sel14):
        upper = address[:2]
        lower = address[2:]

        la, lb, lc, ld = DMux4Way(load, upper)

        self.rams[0].input(inp, la, lower)
        self.rams[1].input(inp, lb, lower)
        self.rams[2].input(inp, lc, lower)
        self.rams[3].input(inp, ld, lower)

    def output(self, address: sel14) -> Word16:
        upper = address[:2]
        lower = address[2:]

        return Mux4Way16(
            self.rams[0].output(lower),
            self.rams[1].output(lower),
            self.rams[2].output(lower),
            self.rams[3].output(lower),
            upper,
        )

    def tick(self):
        for ram in self.rams:
            ram.tick()