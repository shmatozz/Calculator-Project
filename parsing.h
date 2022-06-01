#pragma once

#include <complex.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

void buildStack(char **tokens, int token_cnt, stack *st);
double complex evaluate(stack *input, char** variables, double complex *var_values, int *var_counter);


static char operands[] = {
        ')',
        '(',
        '^',
        '*',
        '/',
        '+',
        '-',
        '=',
        ','
};

static const char operands_count = 9;
