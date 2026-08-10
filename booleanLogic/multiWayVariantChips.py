from booleanLogic.elementaryChips import Or, DeMux
from booleanLogic.multibitVariantChips import Mux16
from utilities.utils import BitType, _check_length, _check_bits, bits_to_binary, binary_to_bits, sel2, sel3, Word8, Word16

def Or8Way(a:Word8):
    _check_length(a,8)

    a_bits = bits_to_binary(a)
    _check_bits(*a_bits)

    or01 = Or(a=a_bits[0],b=a_bits[1])
    or23 = Or(a=a_bits[2],b=a_bits[3])
    or45 = Or(a=a_bits[4],b=a_bits[5])
    or67 = Or(a=a_bits[6],b=a_bits[7])

    or1234 = Or(a=or01,b=or23)
    or4567 = Or(a=or45,b=or67)

    out = Or(a=or1234,b=or4567)

    return out

def Mux4Way16(a:Word16, b:Word16, c:Word16, d:Word16, sel: sel2):
    _check_length(sel,2)

    sel_bits = binary_to_bits(sel)
    _check_bits(*sel_bits)

    ab = Mux16(a=a,b=b,sel=sel_bits[1])
    cd = Mux16(a=c,b=d,sel=sel_bits[1])
    out = Mux16(a=ab,b=cd,sel=sel_bits[0])

    return out


def Mux8Way16(a:Word16, b:Word16, c:Word16, d:Word16, 
              e:Word16, f:Word16, g:Word16, h:Word16, sel: sel3):
    _check_length(sel, 3)

    sel_bits = binary_to_bits(sel)
    _check_bits(*sel_bits)

    abcd = Mux4Way16(a=a,b=b,c=c,d=d,sel=sel_bits[1:])
    efgh = Mux4Way16(a=e,b=f,c=g,d=h,sel=sel_bits[1:])
    out = Mux16(a=abcd,b=efgh,sel=sel_bits[0])

    return out


def DMux4Way(D:BitType, sel:sel2) -> tuple[BitType, BitType, BitType, BitType]:
    _check_length(sel, 2)
    _check_bits(*sel)
    sel_bits = binary_to_bits(sel)

    ab, cd = DeMux(D=D, sel=sel_bits[0])
    a, b = DeMux(D=ab, sel=sel_bits[1])
    c, d = DeMux(D=cd, sel=sel_bits[1])

    return a, b, c ,d


def DMux8Way(D:BitType, sel:sel3) -> tuple[BitType, BitType, BitType, BitType,BitType, BitType, BitType, BitType]:
    _check_length(sel, 3)
    _check_bits(*sel)
    sel_bits = binary_to_bits(sel)

    abcd, efgh = DeMux(D=D, sel=sel_bits[0])
    a, b, c, d = DMux4Way(D=abcd, sel=sel_bits[1:])
    e, f, g, h = DMux4Way(D=efgh, sel=sel_bits[1:])

    return a, b, c, d, e, f, g, h