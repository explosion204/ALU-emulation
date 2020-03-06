# ALU_emulation
ALU emulation on C (signed fixed-point arithemtics; Booth's algorithm for multiplication)

COMPILE: gcc main.c -o {filename}
USAGE: {filename} {bit_representation} {first binary operand} {second binary operand} {operation (+ - x)}

EXAMPLE:
gcc main.c -o alu
./alu 4 1001 0101 +