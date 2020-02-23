#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

int* half_adder(int x, int y); // x, y: input signals; int*: int[0] - sum signal, int[1] - carry signal
int* adder(int x, int y, int c); // x, y, c: input signals; int[0] - sum signal, int[1] - carry signal
int* sum(int* x, int* y, int bits);
int* twoscomplement(int* x, int bits);
int* strtobin(char* str, unsigned int bits);

int main()
{
    unsigned int bits;
    printf("ALU emulator.\n");
    printf("Using ...-bit representation: ");
    scanf("%d", &bits);
    char* x_operand = malloc(sizeof(char) * bits);
    char* y_operand = malloc(sizeof(char) * bits);
    printf("\nFirst operand: ");
    fflush(stdin);
    fgets(x_operand, bits + 1, stdin);
    printf("\nSecond operand: ");
    fflush(stdin);
    fgets(y_operand, bits + 1, stdin);
    int* x_bits = strtobin(strtok(x_operand, "\n"), bits);
    int* y_bits = strtobin(strtok(y_operand, "\n"), bits);

    int* result = sum(x_bits, y_bits, bits);
    if (result[0] == 1)
        result = twoscomplement(result, bits);
    printf("\nResult: ");
    for (int i = 0; i < bits; i++)
    {
        printf("%d", result[i]);
    }
    free(x_operand);
    free(y_operand);
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

int* sum(int* x, int* y, int bits)
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
    if (c != 0)
        printf("\nOverflow!");
    return result;
}

int* twoscomplement(int* x, int bits)
{
    for (int i = 1; i < bits; i++)
        x[i] ^= 1;
    int* single = calloc(bits, sizeof(int));
    single[bits - 1] = 1;
    int* result = sum(x, single, bits);
    free(x);
    return result;
}


int* strtobin(char* str, unsigned int bits)
{
    int* bin = (int*)malloc(sizeof(int) * bits);
    for (int i = 0; i < bits; i++)
        bin[i] = str[i] - 48;
    if (bin[0] == 1)
        bin = twoscomplement(bin, bits);
    return bin;
}

