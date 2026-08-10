from typing import Literal
from booleanLogic.elementaryChips import Not, And, Or, Mux
from utilities.utils import _check_length, _check_bits, bits_to_binary, binary_to_bits, Word8, Word16

def Not16(a:Word16):
    '''
    performs not operation on 16 bits

    Args:
        a - 16 bit value

    Returns:
        output is negation of a
        output[i] = Not(a[i])
    '''
    a = str(a)
    _check_length(list(a),16)

    a_bits = bits_to_binary(a)
    _check_bits(*a_bits)

    not_a_bits = []
    for bit in a_bits:
        not_a_i = Not(bit)
        not_a_bits.append(not_a_i)

    out = binary_to_bits(not_a_bits)
    return out

def And16(a:Word16, b:Word16):
    '''
    performs and operation on 16 bits

    Args:
        a - 16 bit value
        b - 16 bit value

    Returns:
        output is AND operation between a and b
        output[i] = a[i] and b[i]
    '''
    a = str(a)
    b = str(b)

    _check_length(list(a),16)
    _check_length(list(b),16)

    a_bits = bits_to_binary(a)
    b_bits = bits_to_binary(b)
    _check_bits(*a_bits)
    _check_bits(*b_bits)

    and_a_b_bits = []
    for a_i, b_i in zip(a_bits,b_bits):
        and_a_b_i = And(a_i,b_i)
        and_a_b_bits.append(and_a_b_i)

    out = binary_to_bits(and_a_b_bits)
    return out

def Or16(a:Word16, b:Word16):
    '''
    performs or operation on 16 bits

    Args:
        a - 16 bit value
        b - 16 bit value

    Returns:
        output is OR operation between a and b
        output[i] = a[i] + b[i]
    '''
    a = str(a)
    b = str(b)

    _check_length(list(a),16)
    _check_length(list(b),16)

    a_bits = bits_to_binary(a)
    b_bits = bits_to_binary(b)
    _check_bits(*a_bits)
    _check_bits(*b_bits)

    or_a_b_bits = []
    for a_i, b_i in zip(a_bits,b_bits):
        or_a_b_i = Or(a_i,b_i)
        or_a_b_bits.append(or_a_b_i)

    out = binary_to_bits(or_a_b_bits)
    return out

def Mux16(a:Word16, b:Word16, sel:Literal['0','1']):
    '''
    16-bit 2:1 multiplexer - selects one of the inputs

    Args:
        a - 16 bit value
        b - 16 bit value
        sel - 1 bit selector value

    Returns:
        output is selected from a and b depending on sel signal
    '''
    a = str(a)
    b = str(b)

    _check_length(list(a),16)
    _check_length(list(b),16)

    a_bits = bits_to_binary(a)
    b_bits = bits_to_binary(b)
    _check_bits(*a_bits)
    _check_bits(*b_bits)
    _check_bits(sel)

    mux_a_b_bits = []
    for a_i, b_i in zip(a_bits,b_bits):
        mux_a_b_i = Mux(sel=sel,a=a_i,b=b_i)
        mux_a_b_bits.append(mux_a_b_i)

    out = binary_to_bits(mux_a_b_bits)
    return out