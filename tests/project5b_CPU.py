import os, sys
import pytest

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from computer.cpu import CPU
from booleanArithmetic.PC import PC
from utilities.utils import BitType, Word16

def reset_cpu():
    """Clear all persistent register state between tests."""
    if hasattr(CPU, "_A"):
        del CPU._A
    if hasattr(CPU, "_D"):
        del CPU._D
    if hasattr(PC, "_reg"):
        del PC._reg


@pytest.fixture(autouse=True)
def _isolate_each_test():
    """Ensure every test starts from a clean CPU/PC state."""
    reset_cpu()
    yield
    reset_cpu()


def a_instruction(value: int) -> Word16:
    """Build an A-instruction: opcode bit 0 = 0, rest = value (15 bits)."""
    assert 0 <= value < 2**15
    return Word16(value)  # MSB (index 0) is 0 automatically since value < 2^15


def c_instruction(comp: str, dest: str = "", jump: str = "") -> Word16:
    """
    Build a C-instruction from mnemonic comp/dest/jump strings, e.g.
    c_instruction("D+1", "D", "JGT")
    """
    comp_table = {
        "0":   "0101010", "1":   "0111111", "-1":  "0111010",
        "D":   "0001100", "A":   "0110000", "M":   "1110000",
        "!D":  "0001101", "!A":  "0110001", "!M":  "1110001",
        "-D":  "0001111", "-A":  "0110011", "-M":  "1110011",
        "D+1": "0011111", "A+1": "0110111", "M+1": "1110111",
        "D-1": "0001110", "A-1": "0110010", "M-1": "1110010",
        "D+A": "0000010", "D+M": "1000010",
        "D-A": "0010011", "D-M": "1010011",
        "A-D": "0000111", "M-D": "1000111",
        "D&A": "0000000", "D&M": "1000000",
        "D|A": "0010101", "D|M": "1010101",
    }
    dest_table = {"": "000", "M": "001", "D": "010", "MD": "011",
                  "A": "100", "AM": "101", "AD": "110", "AMD": "111"}
    jump_table = {"": "000", "JGT": "001", "JEQ": "010", "JGE": "011",
                  "JLT": "100", "JNE": "101", "JLE": "110", "JMP": "111"}

    bits = "111" + comp_table[comp] + dest_table[dest] + jump_table[jump]
    return Word16(int(bits, 2))


# ---------------------------------------------------------------------
# A-instruction tests
# ---------------------------------------------------------------------

def test_a_instruction_loads_value_into_A():
    instr = a_instruction(21)
    outM, writeM, addressM, pc = CPU(inM=Word16(0), instruction=instr, reset=BitType(0))
    assert int(addressM) == 21
    assert int(writeM) == 0

def test_a_instruction_does_not_write_memory():
    instr = a_instruction(100)
    _, writeM, _, _ = CPU(inM=Word16(0), instruction=instr, reset=BitType(0))
    assert int(writeM) == 0


# ---------------------------------------------------------------------
# C-instruction tests
# ---------------------------------------------------------------------

def test_c_instruction_compute_D_plus_1_store_D():
    # @5  -> A=5
    CPU(inM=Word16(0), instruction=a_instruction(5), reset=BitType(0))
    # D=0 initially; D=D+1 -> D=1
    CPU(inM=Word16(0), instruction=c_instruction("D+1", dest="D"), reset=BitType(0))
    # D=D+1 again -> D=2, and confirm via outM through D+A referencing A=5
    outM, _, _, _ = CPU(inM=Word16(0), instruction=c_instruction("D+A", dest="D"), reset=BitType(0))
    assert int(outM) == 6  # D(1) + A(5)

def test_c_instruction_writeM_flag():
    CPU(inM=Word16(0), instruction=a_instruction(3), reset=BitType(0))
    _, writeM, addressM, _ = CPU(
        inM=Word16(0), instruction=c_instruction("D", dest="M"), reset=BitType(0)
    )
    assert int(writeM) == 1
    assert int(addressM) == 3

def test_c_instruction_outM_matches_alu():
    CPU(inM=Word16(0), instruction=a_instruction(10), reset=BitType(0))  # A=10
    outM, _, _, _ = CPU(
        inM=Word16(7), instruction=c_instruction("D+M", dest="D"), reset=BitType(0)
    )
    assert int(outM) == 7  # D was 0, +M(7) = 7


# ---------------------------------------------------------------------
# Jump / PC tests
# ---------------------------------------------------------------------

def test_pc_increments_by_default():
    CPU(inM=Word16(0), instruction=a_instruction(0), reset=BitType(0))
    _, _, _, pc1 = CPU(inM=Word16(0), instruction=a_instruction(0), reset=BitType(0))
    _, _, _, pc2 = CPU(inM=Word16(0), instruction=a_instruction(0), reset=BitType(0))
    assert int(pc2) == int(pc1) + 1

def test_jmp_sets_pc_to_A():
    CPU(inM=Word16(0), instruction=a_instruction(50), reset=BitType(0))  # A=50
    _, _, _, pc = CPU(
        inM=Word16(0), instruction=c_instruction("0", jump="JMP"), reset=BitType(0)
    )
    assert int(pc) == 50

def test_jgt_taken_when_positive():
    CPU(inM=Word16(0), instruction=a_instruction(50), reset=BitType(0))  # A=50
    _, _, _, pc = CPU(
        inM=Word16(0), instruction=c_instruction("1", jump="JGT"), reset=BitType(0)
    )
    assert int(pc) == 50

def test_jgt_not_taken_when_zero():
    CPU(inM=Word16(0), instruction=a_instruction(50), reset=BitType(0))  # A=50
    _, _, _, pc_before = CPU(inM=Word16(0), instruction=a_instruction(0), reset=BitType(0))
    _, _, _, pc = CPU(
        inM=Word16(0), instruction=c_instruction("0", jump="JGT"), reset=BitType(0)
    )
    assert int(pc) == int(pc_before) + 1  # falls through, doesn't jump to 50

def test_jeq_taken_when_zero():
    CPU(inM=Word16(0), instruction=a_instruction(77), reset=BitType(0))
    _, _, _, pc = CPU(
        inM=Word16(0), instruction=c_instruction("0", jump="JEQ"), reset=BitType(0)
    )
    assert int(pc) == 77

def test_jlt_taken_when_negative():
    CPU(inM=Word16(0), instruction=a_instruction(88), reset=BitType(0))
    _, _, _, pc = CPU(
        inM=Word16(0), instruction=c_instruction("-1", jump="JLT"), reset=BitType(0)
    )
    assert int(pc) == 88


# ---------------------------------------------------------------------
# Reset tests
# ---------------------------------------------------------------------

def test_reset_sets_pc_to_zero():
    CPU(inM=Word16(0), instruction=a_instruction(100), reset=BitType(0))
    CPU(inM=Word16(0), instruction=c_instruction("0", jump="JMP"), reset=BitType(0))  # pc=100
    _, _, _, pc = CPU(inM=Word16(0), instruction=a_instruction(0), reset=BitType(1))
    assert int(pc) == 0


if __name__ == "__main__":
    pytest.main([__file__, "-v"])