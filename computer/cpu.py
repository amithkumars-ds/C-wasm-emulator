from booleanLogic.elementaryChips import Not, Or, And
from booleanLogic.multibitVariantChips import Mux16
from booleanArithmetic.ALU import ALU
from memory.memoryChips import Register16
from utilities.utils import BitType, Word16, Word15
from booleanArithmetic.PC import PC

def CPU(inM: Word16, instruction: Word16, reset: BitType) -> tuple[Word16, BitType, Word15, Word15]:
    isAInstruction = Not(a=instruction[0])  # 0=A-instr, 1=C-instr

    # Registers persist across calls
    if not hasattr(CPU, "_A"):
        CPU._A = Register16()
        CPU._D = Register16()

    aOut = CPU._A.output()
    dOut = CPU._D.output()

    aluY = Mux16(a=aOut, b=inM, sel=instruction[3])

    aluOut, zr, ng = ALU(
        x=dOut, y=aluY,
        zx=instruction[4], nx=instruction[5],
        zy=instruction[6], ny=instruction[7],
        f=instruction[8],  no=instruction[9],
    )

    # A register
    aIn = Mux16(a=instruction, b=aluOut, sel=instruction[0])
    loadA  = Or(a=isAInstruction, b=instruction[10])
    CPU._A.input(inp=aIn, load=loadA)

    # D register
    loadD  = And(a=instruction[0], b=instruction[11])
    CPU._D.input(inp=aluOut, load=loadD)

    # Memory interface
    outM = aluOut
    writeM = And(a=instruction[0], b=instruction[12])
    addressM = Word15(aOut)  # truncate/convert to 15 bits

    # Jump logic
    notZr = Not(a=zr)
    notNg = Not(a=ng)
    pos = And(a=notNg, b=notZr)
    zero = zr
    neg = ng

    jgt = And(a=instruction[13], b=pos)
    jeq = And(a=instruction[14], b=zero)
    jlt = And(a=instruction[15], b=neg)
    jump = Or(a=Or(a=jgt, b=jeq), b=jlt)
    pcLoad = And(a=instruction[0], b=jump)

    pc = PC(inp=aOut, load=pcLoad, inc=BitType(1), reset=reset)

    return outM, writeM, addressM, Word15(pc)
