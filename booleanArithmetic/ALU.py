from booleanLogic.elementaryChips import Or, Not
from booleanLogic.multibitVariantChips import Not16, And16 , Mux16
from booleanLogic.multiWayVariantChips import Or8Way
from booleanArithmetic.arithmeticChips import Add16
from utilities.utils import BitType, Word16, Word16_zero

def ALU(x:Word16, y:Word16, 
        zx:BitType, nx:BitType, 
        zy:BitType, ny:BitType, 
        f:BitType, no:BitType) -> tuple[Word16, BitType, BitType]:

    # preprocess x
    zero_x = Word16_zero
    x = Mux16(a=x,b=zero_x,sel=zx)

    negate_x = Not16(a=x)
    preprocessed_x = Mux16(a=x,b=negate_x,sel=nx)


    # preprocess y
    zero_y = Word16_zero
    y = Mux16(a=y,b=zero_y,sel=zy)

    negate_y = Not16(a=y)
    preprocessed_y = Mux16(a=y,b=negate_y,sel=ny)

    # function f
    and_ab = And16(a=preprocessed_x,b=preprocessed_y)
    add_ab = Add16(a=preprocessed_x,b=preprocessed_y)

    f_out = Mux16(a=and_ab,b=add_ab,sel=f)

    # process output
    negate_f_out = Not16(f_out)
    out = Mux16(a=f_out,b=negate_f_out,sel=no)

    # output control bits
    zr_left = Or8Way(a=out[:8])
    zr_right = Or8Way(a=out[8:])
    zr = Not(Or(zr_left,zr_right))

    ng = out[0]

    return out, zr, ng