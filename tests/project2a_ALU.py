import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from booleanArithmetic.ALU import ALU


def test_ALU():
    x = "0000000000000101"  # 5
    y = "0000000000000011"  # 3

    tests = [
        # zx nx zy ny f no expected
        ("1","0","1","0","1","0","0000000000000000"),  # 0
        ("1","1","1","1","1","1","0000000000000001"),  # 1
        ("1","1","1","0","1","0","1111111111111111"),  # -1

        ("0","0","1","1","0","0", x),                  # x
        ("1","1","0","0","0","0", y),                  # y

        ("0","0","1","1","0","1","1111111111111010"),  # !x
        ("1","1","0","0","0","1","1111111111111100"),  # !y

        ("0","0","1","1","1","1","1111111111111011"),  # -x
        ("1","1","0","0","1","1","1111111111111101"),  # -y

        ("0","1","1","1","1","1","0000000000000110"),  # x+1
        ("1","1","0","1","1","1","0000000000000100"),  # y+1

        ("0","0","1","1","1","0","0000000000000100"),  # x-1
        ("1","1","0","0","1","0","0000000000000010"),  # y-1

        ("0","0","0","0","1","0","0000000000001000"),  # x+y
        ("0","1","0","0","1","1","0000000000000010"),  # x-y
        ("0","0","0","1","1","1","1111111111111110"),  # y-x

        ("0","0","0","0","0","0","0000000000000001"),  # x&y
        ("0","1","0","1","0","1","0000000000000111"),  # x|y
    ]

    for i, (zx, nx, zy, ny, f, no, expected) in enumerate(tests, 1):
        out, zr, ng = ALU(
            x=x,
            y=y,
            zx=zx,
            nx=nx,
            zy=zy,
            ny=ny,
            f=f,
            no=no,
        )

        expected_zr = "1" if expected == "0000000000000000" else "0"
        expected_ng = expected[0]

        assert out == expected, (
            f"Test {i} failed (out)\n"
            f"expected = {expected}\n"
            f"got      = {out}"
        )

        assert zr == expected_zr, (
            f"Test {i} failed (zr)\n"
            f"expected = {expected_zr}\n"
            f"got      = {zr}"
        )

        assert ng == expected_ng, (
            f"Test {i} failed (ng)\n"
            f"expected = {expected_ng}\n"
            f"got      = {ng}"
        )

    print(f"Passed {len(tests)} ALU tests.")


if __name__ == "__main__":
    test_ALU()