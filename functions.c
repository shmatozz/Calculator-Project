#include "functions.h"

const double complex PI = M_PI;
const double complex E = M_E;
const double complex J = I;

double complex my_clog(double complex base, double complex num) {
    return clog(num) / clog(base);
}

double complex my_abs(double complex a) {
    return fabs(creal(a));
}

double complex my_cabs(double complex a) {
    return cabs(a);
}

double complex my_carg(double complex a) {
    return carg(a);
}

double complex my_creal(double complex a) {
    return creal(a);
}

double complex my_cimag(double complex a) {
    return cimag(a);
}

stack *initStack() {
    stack *result = malloc(sizeof(stack));
    result->ptr = 0;
    result->buf = malloc(sizeof(token) * STACK_BUFFER_SIZE);
    memset(result->buf, 0, sizeof(token) * STACK_BUFFER_SIZE);
    return result;
}

void freeStack(stack *st) {
    free(st->buf);
    free(st);
}

void eraseReductant(char *str) {
    int size = strlen(str);
    int j = 0;
    for (int i = 0; i < size; ++i) {
        if (str[i] == ' ' || str[i] == '\n') continue;
        str[j] = str[i];
        ++j;
    }
    for (; j < size; ++j) {
        str[j] = 0;
    }
}

void buildStack(char **tokens, int token_cnt, stack *st) {
    int priorities[token_cnt];
    char unary[token_cnt];
    char token_type[token_cnt];//0 - brackets, 1 - func, 2 - number, 3 - variable, 4 - operand, 5 - =, 6 - ','
    memset(unary, 0, sizeof(unary));
    memset(token_type, 0, sizeof(token_type));
    memset(priorities, 0, sizeof(priorities));
    int bracket_balance = 0;
    char is_unary = 1;
    for (int i = 0; i < token_cnt; ++i) {
        char type = 0;
        if (strcmp(tokens[i], "(") == 0) {
            type = 0;
            bracket_balance++;
            priorities[i] = 0;
            is_unary = 1;
        } else if (strcmp(tokens[i], ")") == 0) {
            type = 0;
            bracket_balance--;
            priorities[i] = 0;
            is_unary = 0;
        } else if (strcmp(tokens[i], ",") == 0) {
            type = 0;
            bracket_balance += 100;
            priorities[i] = 0;
            is_unary = 1;
        } else if (strcmp(tokens[i], "=") == 0) {
            type = 5;
            priorities[i] = -token_cnt * 6;
            is_unary = 1;
        } else if (strcmp(tokens[i], "*") == 0 || strcmp(tokens[i], "/") == 0) {
            type = 4;
            priorities[i] = 1;
            is_unary = 0;
        } else if (strcmp(tokens[i], "^") == 0) {
            type = 4;
            priorities[i] = 2;
            is_unary = 0;
        } else if (strcmp(tokens[i], "-") == 0 || strcmp(tokens[i], "+") == 0) {
            type = 4;
            if (is_unary) {
                unary[i] = 1;
                priorities[i] = 4;
            } else {
                priorities[i] = 0;
            }
            is_unary = 0;
        } else {
            char func = 0;
            for (int j = 0; j < simple_function_cnt; ++j) {
                if (strcmp(simple_functions[j], tokens[i]) == 0) {
                    func = 1;
                    type = 1;
                    priorities[i] = 3;
                }
            }
            for (int j = 0; j < complex_function_cnt; ++j) {
                if (strcmp(complex_functions[j], tokens[i]) == 0) {
                    func = 1;
                    type = 1;
                    priorities[i] = 3;
                }
            }
            if (!func) {
                int len = strlen(tokens[i]);
                char var = 0;
                for (int j = 0; j < len; j++) {
                    if (!((tokens[i][j] >= '0' && tokens[i][j] <= '9') || tokens[i][j] == '.') &&
                        (tokens[i][j] != 'j' || j != len - 1)) {
                        var = 1;
                        break;
                    }
                }
                type = (var ? 3 : 2);
                priorities[i] = 5;
                is_unary = 0;
            } else {
                is_unary = 0;
            }
        }
        token_type[i] = type;
        priorities[i] += bracket_balance * 6;
    }
    int minimal_priority = token_cnt * 100;
    int minimal_position = 0;
    for (int i = 0; i < token_cnt; ++i) {
        if (priorities[i] < minimal_priority && token_type[i] != 0) {
            minimal_priority = priorities[i];
            minimal_position = i;
        }
    }
    if (minimal_position > 0 && !unary[minimal_position]) {
        buildStack(tokens, minimal_position, st);
    }
    if (minimal_position < token_cnt - 1) {
        buildStack(tokens + minimal_position + 1, token_cnt - minimal_position - 1, st);
    }
    switch (token_type[minimal_position]) {
        case 0:
            return;
        case 1:
            st->buf[st->ptr].type = 0;
            memcpy(st->buf[st->ptr].str, tokens[minimal_position], strlen(tokens[minimal_position]) + 1);
            st->ptr++;
            break;
        case 2:
            st->buf[st->ptr].type = 1;
            int len = strlen(tokens[minimal_position]);
            double complex num = 0;
            if (tokens[minimal_position][len - 1] == 'j') {
                tokens[minimal_position][len - 1] = 0;
                if (len == 1) {
                    num = J;
                } else {
                    num = strtod(tokens[minimal_position], NULL) * J;
                }
            } else {
                num = strtod(tokens[minimal_position], NULL);
            }
            st->buf[st->ptr].num = num;
            st->ptr++;
            break;
        case 3:
            if (strcmp(tokens[minimal_position], "PI") == 0) {
                st->buf[st->ptr].type = 1;
                st->buf[st->ptr].num = PI;
            } else if (strcmp(tokens[minimal_position], "e") == 0) {
                st->buf[st->ptr].type = 1;
                st->buf[st->ptr].num = E;
            } else {
                st->buf[st->ptr].type = 2;
                memcpy(st->buf[st->ptr].str, tokens[minimal_position], strlen(tokens[minimal_position]) + 1);
            }
            st->ptr++;
            break;
        case 4:
        case 5:
            if(unary[minimal_position]) {
                st->buf[st->ptr].type = 4;
            } else {
                st->buf[st->ptr].type = 3;
            }
            memcpy(st->buf[st->ptr].str, tokens[minimal_position], strlen(tokens[minimal_position]) + 1);
            st->ptr++;
            break;
    }
}
