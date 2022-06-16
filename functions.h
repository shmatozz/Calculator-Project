#pragma once

#include <complex.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


typedef struct {
    char str[10];
    double complex num;
    char type;
} token;

typedef struct {
    token *buf;
    int ptr;
} stack;


double complex my_abs(double complex a);
double complex my_cabs(double complex a);
double complex my_carg(double complex a);
double complex my_creal(double complex a);
double complex my_cimag(double complex a);
double complex my_clog(double complex base, double complex num);
void eraseReductant(char *str);
stack *initStack();
void freeStack(stack *st);
char **splitTokens(char *infix, size_t *token_cnt);

static double complex (*simple_function_ptrs[])(double complex) = {
        ccos,
        csin,
        ctan,
        clog,
        csqrt,
        my_abs,
        cexp,
        my_creal,
        my_cimag,
        my_cabs,
        my_carg
};

static double complex (*complex_function_ptrs[])(double complex, double complex) = {
        cpow,
        my_clog
};
static const int STACK_BUFFER_SIZE = 100;
static const int  INPUT_BUFFER_SIZE = 250;

static char *simple_functions[] = {
        "cos",
        "sin",
        "tg",
        "ln",
        "sqrt",
        "abs",
        "exp",
        "real",
        "imag",
        "mag",
        "phase"
};

static int simple_function_cnt = 11;

static char *complex_functions[] = {
        "pow",
        "log"
};

static int complex_function_cnt = 2;

