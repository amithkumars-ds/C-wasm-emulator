from typing import Literal

# preset
BitType = Literal['0','1']
Word16 = str
Word15 = str
Word8 = str

Word16_zero = '0000000000000000'

sel2 = Literal["00", "01", "10", "11"]
sel3 = Literal["000", "001", "010", "011",
               "100", "101", "110", "111"]
sel4 = Literal[
    "0000", "0001", "0010", "0011",
    "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011",
    "1100", "1101", "1110", "1111"
]

sel6 = Literal[
    "000000", "000001", "000010", "000011",
    "000100", "000101", "000110", "000111",
    "001000", "001001", "001010", "001011",
    "001100", "001101", "001110", "001111",
    "010000", "010001", "010010", "010011",
    "010100", "010101", "010110", "010111",
    "011000", "011001", "011010", "011011",
    "011100", "011101", "011110", "011111",
    "100000", "100001", "100010", "100011",
    "100100", "100101", "100110", "100111",
    "101000", "101001", "101010", "101011",
    "101100", "101101", "101110", "101111",
    "110000", "110001", "110010", "110011",
    "110100", "110101", "110110", "110111",
    "111000", "111001", "111010", "111011",
    "111100", "111101", "111110", "111111"
]

sel9 = str
sel12 = str
sel14 = str

def Nand(a:BitType, b:BitType) -> BitType:  # the building block of this project
    '''
    implementing a basic Nand gate

    Args:
        a - 1 bit value
        b - 1 bit value
    
    Returns:
        output - 1 bit value of (ab)'
    '''
    
    _check_bits(a,b)

    out = "0" if a == "1" and b == "1" else "1"
    
    return out

def _check_bits(*bits: int) -> None:
    if any(bit not in ('0', '1') for bit in bits):
        raise ValueError("Values must be 0 or 1")

def _check_length(bits:str, size: int) -> None:
    if len(bits) != size:
        raise ValueError(f"Given input is not of length: {size}")

def bits_to_binary(binary: str) -> list[str]:
    _check_bits(*binary)
    return list(binary)

def binary_to_bits(bits: list[BitType]) -> Word16:
    _check_bits(*bits)
    return ''.join(bits)