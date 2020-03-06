#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef int bool;

/*
ARGS: x, y: input signals;
RETURN VALUE: int[0] - sum signal, int[1] - carry signal
*/
int* half_adder(int x, int y);

/*
ARGS: x, y, c: input signals;
RETURN VALUE: int[0] - sum signal, int[1] - carry signal
*/
int* adder(int x, int y, int c); 

/*
ARGS: x, y: binary numbers; bits: bit representation; overflow_flag: flag to detect overflow
RETURN VALUE: binary sum
*/
int* sum(int* x, int* y, int bits, bool *overflow_flag); 

/*
ARGS: x: binary number; bits: bit representation
RETURN VALUE: two's complement form of binary number
*/
int* twoscomplement(int* x, int bits);

/*
ARGS: x: binary number; value: number of right arithmetic shifts; bits: bit representation
RETURN VALUE: binary number after right arithmetic shift
*/
int* shr(int* x, int value, int bits);

/*
ARGS: x, y: binary numbers; bits: bit representation
RETURN VALUE: binary product of x and y (Booth's algorithm)
*/
int* mul(int* x, int* y, int bits);

/*
ARGS: str: string to convert; bits: bit representation
RETURN VALUE: binary number converted from string
*/
int* strtobin(char* str, unsigned int bits);

int main(int argc, char **argv)
{
    int bits = atoi(argv[1]);
    int* x_bits = strtobin(argv[2], bits);
    int* y_bits = strtobin(argv[3], bits);
    char op = argv[4][0];
    int* result;
    bool overflow_flag = FALSE;
    switch (op)
    {
        case '+':
            x_bits = twoscomplement(x_bits, bits);
            y_bits = twoscomplement(y_bits, bits);
            result = sum(x_bits, y_bits, bits, &overflow_flag);
            break;
        case '-':
            y_bits[0] ^= 1;
            x_bits = twoscomplement(x_bits, bits);
            y_bits = twoscomplement(y_bits, bits);
            result = sum(x_bits, y_bits, bits, &overflow_flag);
            break;
        case 'x':
            x_bits = twoscomplement(x_bits, bits);
            y_bits = twoscomplement(y_bits, bits);
            result = mul(x_bits, y_bits, bits);
            bits *= 2;
            break;
    }

    result = twoscomplement(result, bits);
    printf("\nResult: ");
    if (!overflow_flag)
    {
        for (int i = 0; i < bits; i++)
            printf("%d", result[i]);
    }
    else
        printf("Overflow.");
    free(x_bits);
    free(y_bits);
    free(result);
    getch();
    return 0;
}

int* half_adder(int x, int y)
{
    int* output = malloc(sizeof(int) * 2);
    output[0] = x ^ y;
    output[1] = x & y;
    return output;
}

int* adder(int x, int y, int c)
{
    int* output = malloc(sizeof(int) * 2);
    int* HA1_output = half_adder(x, y);
    int* HA2_output = half_adder(HA1_output[0], c);
    output[0] = HA2_output[0];
    output[1] = HA1_output[1] | HA2_output[1];
    return output;
}

int* sum(int* x, int* y, int bits, bool* overflow_flag)
{
    int* result = malloc(sizeof(int) * bits);
    int c = 0;
    for (int i = bits - 1; i >= 0; i--)
    {
        int* temp_result = adder(x[i], y[i], c);
        result[i] = temp_result[0];
        c = temp_result[1];
        free(temp_result);
    }
    if (overflow_flag != NULL)
        if ((x[0] == 0 && y[0] == 0 && result[0] == 1) || (x[0] == 1 && y[0] == 1 && result[0] == 0))
            *overflow_flag = TRUE;
    return result;
}

int* mul(int* x, int* y, int bits)
{
    int* add_reg = calloc(2 * bits , sizeof(int));
    int* sub_reg = calloc(2 * bits , sizeof(int));
    int* result = calloc(2 * bits, sizeof(int));
    int bit_reg = 0;
    memcpy(add_reg, x, bits * sizeof(int));
    x = twoscomplement(x, bits);
    x[0] ^= 1;
    x = twoscomplement(x, bits);
    memcpy(sub_reg, x, bits * sizeof(int));
    memcpy(&result[bits], y, bits * sizeof(int));
    for (int i = 0; i < bits; i++)
    {
        if (result[2 * bits - 1] == 0 && bit_reg == 1)
            result = sum(result, add_reg, 2 * bits, NULL);
        else if (result[2 * bits - 1] == 1 && bit_reg == 0)
            result = sum(result, sub_reg, 2 * bits, NULL);
        bit_reg = result[2 * bits - 1];
        result = shr(result, 1, 2 * bits);
    }
    return result;
}

int* twoscomplement(int* x, int bits)
{
    if (x[0] == 0)
        return x;
    for (int i = 1; i < bits; i++)
        x[i] ^= 1;
    int* single = calloc(bits, sizeof(int));
    single[bits - 1] = 1;
    int* result = sum(x, single, bits, NULL);
    return result;
}

int* shr(int* x, int value, int bits)
{
    int* result = malloc(sizeof(int) * bits);
    for (int i = 0; i < value + 1; i++)
        result[i] = x[0];
    memcpy(&result[value + 1], &x[1], (bits - value - 1) * sizeof(int));
    return result;
}

int* strtobin(char* str, unsigned int bits)
{
    int* bin = malloc(sizeof(int) * bits);
    for (int i = 0; i < bits; i++)
        bin[i] = str[i] - 48;
    return bin;
}