from booleanLogic.elementaryChips import And, Or, Xor
from utilities.utils import BitType, _check_length, _check_bits, bits_to_binary, binary_to_bits, Word8, Word16

def HalfAdder(a: BitType, b: BitType) -> tuple[BitType, BitType]:
    sum = Xor(a=a,b=b)
    carry = And(a=a,b=b)

    return sum, carry

def FullAdder(a: BitType, b: BitType, c_in: BitType):
    ab = And(a=a,b=b)
    bc_in = And(a=b,b=c_in)
    ac_in = And(a=a,b=c_in)

    or_ab_bcin = Or(a=ab,b=bc_in)
    c_out = Or(a=or_ab_bcin,b=ac_in)

    xor_a_b = Xor(a=a,b=b)
    sum = Xor(a=xor_a_b,b=c_in)

    return sum, c_out

def Add16(a: Word16, b: Word16) -> Word16:
    _check_length(a,16)
    _check_length(b,16)

    a_bits = binary_to_bits(a)
    b_bits = binary_to_bits(b)

    result = ["0"] * 16
    carry = "0"

    for i in range(15, -1, -1):
        if i == 15:
            result[i], carry = HalfAdder(a_bits[i], b_bits[i])
        else:
            result[i], carry = FullAdder(a_bits[i], b_bits[i], carry)

    return binary_to_bits(result)

def Inc16(a: Word16) -> Word16:
    increment_value = '0000000000000001'
    out = Add16(a=a,b=increment_value)

    return out