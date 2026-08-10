from typing import Literal
from utilities.utils import Nand, _check_bits, BitType

def Not(a:BitType) -> BitType:
    '''
    performs not operation on 1 bit

    Args:
        a - 1 bit value

    Returns:
        output is negation of a i.e., a'
    '''

    _check_bits(a)

    out = Nand(a=a,b=a)
    return out

def And(a:BitType, b:BitType) -> BitType:
    '''
    performs and operation on 2 bits

    Args:
        a - 1 bit value
        b - 1 bit value
    
    Returns:
        output is AND operation between a and b ie., a.b
    '''

    _check_bits(a,b)
    
    not_ab = Nand(a=a,b=b)
    out = Nand(a=not_ab,b=not_ab)

    return out

def Or(a:BitType, b:BitType) -> BitType:
    '''
    performs or operation on 2 bits

    Args:
        a - 1 bit value
        b - 1 bit value

    Returns:
        output is OR operation between a and b ie., a + b
    '''

    _check_bits(a,b)

    not_a = Nand(a=a,b=a)
    not_b = Nand(a=b,b=b)
    out = Nand(a=not_a,b=not_b)

    return out

def Xor(a:BitType, b:BitType) -> BitType:
    '''
    performs Xor operation on 2 bits

    Args:
        a - 1 bit value
        b - 1 bit value

    Returns:
        output is XOR operation between a and b ie., a'b + ab'
    '''

    _check_bits(a,b)

    Nand_ab = Nand(a=a,b=b)
    not_a_Nand_ab = Nand(a=a,b=Nand_ab)
    not_b_Nand_ab = Nand(a=Nand_ab,b=b)

    out = Nand(a=not_a_Nand_ab,b=not_b_Nand_ab)

    return out

def Mux(a:BitType, b:BitType, sel:BitType) -> BitType:
    '''
    2:1 Multiplexer - selects one of the inputs

    if,
        sel=0, out=a
        sel=1, out=b

    Logic is: as' + bs

    Args:
        a - 1 bit value
        b - 1 bit value
        sel - 1 bit selector value

    Returns:
        output is selected from a and b depending on sel signal
    '''

    _check_bits(a,b,sel)

    not_sel = Nand(a=sel,b=sel)
    nand_a_not_sel = Nand(a=a,b=not_sel)
    nand_b_sel = Nand(a=b,b=sel)

    out = Nand(a=nand_a_not_sel,b=nand_b_sel)

    return out

def DeMux(D: BitType, sel: BitType) -> tuple[BitType, BitType]:    
    '''
    1:2  DeMultiplexer - splits input into 2 
    
    if,
        sel=0, a=D, b=0
        sel=1, a=0, b=D

    Logic is: a=D.s' , b=D.s

    Args:
        D - 1 bit input value
        sel - 1 bit selector value

    Returns:
        output is split into a and b using sel signal
    '''

    _check_bits(D,sel)

    not_sel = Nand(a=sel,b=sel)
    nand_D_not_sel = Nand(a=D,b=not_sel)
    a = Nand(a=nand_D_not_sel,b=nand_D_not_sel)

    nand_D_sel = Nand(a=D,b=sel)
    b = Nand(a=nand_D_sel,b=nand_D_sel)

    return a, b