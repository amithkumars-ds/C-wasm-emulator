from booleanLogic.elementaryChips import Not, And, Or
from memory.memoryChips import Register16
from utilities.utils import BitType, Word16, Word15

def PC(inp: Word16, load: BitType, inc: BitType, reset: BitType) -> Word16:
    """
    Simple behavioral PC (not gate-level incrementer).
    If you need a pure gate-level Inc16, swap the '+1' below for your Inc16 chip.
    """
    reg = PC._reg if hasattr(PC, "_reg") else None
    if reg is None:
        reg = Register16()
        PC._reg = reg

    current = reg.output()

    if reset:
        next_val = Word16(0)
    elif load:
        next_val = inp
    elif inc:
        next_val = Word16((int(current) + 1) & 0xFFFF)
    else:
        next_val = current

    reg.input(inp=next_val, load=True)
    return reg.output()