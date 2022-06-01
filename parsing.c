#include "parsing.h"

char **splitTokens(char *infix, size_t *token_cnt) {
    size_t string_size = strlen(infix);
    char **tokens = malloc(sizeof(char *) * string_size);
    memset(tokens, 0, sizeof(char *) * string_size);
    int last_operand = -1;
    for (int i = 0; i < string_size; ++i) {
        char is_operand = 0;
        for (int j = 0; j < operands_count; ++j) {
            if (operands[j] == infix[i]) {
                is_operand = 1;
            }
        }
        if (is_operand) {
            if (i - last_operand > 1) {
                tokens[*token_cnt] = malloc(i - last_operand);
                memcpy(tokens[*token_cnt], infix + last_operand + 1, i - last_operand - 1);
                tokens[*token_cnt][i - last_operand - 1] = 0;
                (*token_cnt)++;
            }
            last_operand = i;
            tokens[*token_cnt] = malloc(2);
            memcpy(tokens[*token_cnt], infix + i, 1);
            tokens[*token_cnt][1] = 0;
            (*token_cnt)++;
        }
    }
    if (string_size - last_operand > 1) {
        tokens[*token_cnt] = malloc(string_size - last_operand);
        memcpy(tokens[*token_cnt], infix + last_operand + 1, string_size - last_operand - 1);
        tokens[*token_cnt][string_size - last_operand - 1] = 0;
        (*token_cnt)++;
    }
    return tokens;
}

double complex evaluate(stack *input, char** variables, double complex *var_values, int *var_counter) {
    double complex nums[STACK_BUFFER_SIZE];
    int num_ptr = 0;
    char found = 0, complex_func = 0, func_ind = 0;
    char* var_name = NULL;
    for (int i = 0; i < input->ptr; ++i) {
        switch (input->buf[i].type) {
            case 1:
                nums[num_ptr] = input->buf[i].num;
                num_ptr++;
                break;
            case 2:
                found = 0;
                for (int j = 0; j < *var_counter; ++j) {
                    if (strcmp(input->buf[i].str, variables[j]) == 0) {
                        found = 1;
                        nums[num_ptr] = var_values[j];
                        num_ptr++;
                        break;
                    }
                }
                if(!found) {
                    if(var_name != NULL) {
                        printf("Using unknown variable %s\n", input->buf[i].str);
                        exit(1);
                    } else {
                        var_name = input->buf[i].str;
                    }
                }
                break;
            case 0:
                complex_func = 1;
                for (int j = 0; j < simple_function_cnt; ++j) {
                    if (strcmp(input->buf[i].str, simple_functions[j]) == 0) {
                        complex_func = 0;
                        func_ind = j;
                        break;
                    }
                }
                if (complex_func) {
                    for (int j = 0; j < complex_function_cnt; ++j) {
                        if (strcmp(input->buf[i].str, complex_functions[j]) == 0) {
                            complex_func = 1;
                            func_ind = j;
                            break;
                        }
                    }
                }
                if(complex_func) {
                    if(num_ptr <= 1) {
                        printf("Not enough arguments for function %s", input->buf[i].str);
                        exit(1);
                    }
                    --num_ptr;
                    double complex first = nums[num_ptr];
                    --num_ptr;
                    double complex second = nums[num_ptr];
                    nums[num_ptr] = complex_function_ptrs[func_ind](first, second);
                    ++num_ptr;
                } else {
                    if(num_ptr < 1) {
                        printf("Not enough arguments for function %s", input->buf[i].str);
                        exit(1);
                    }
                    --num_ptr;
                    double complex first = nums[num_ptr];
                    nums[num_ptr] = simple_function_ptrs[func_ind](first);
                    ++num_ptr;
                }
                break;
            case 3:
                if(input->buf[i].str[0] == '=') {
                    if(num_ptr != 1) {
                        printf("We've done something strange. Check your expression again pls");
                        exit(1);
                    }
                    variables[*var_counter] = malloc(strlen(var_name) + 1);
                    memcpy(variables[*var_counter], var_name, strlen(var_name) + 1);
                    var_values[*var_counter] = nums[num_ptr - 1];
                    (*var_counter)++;
                    return var_values[(*var_counter) - 1];
                }
                if(num_ptr <= 1) {
                    printf("Not enough arguments for function %s", input->buf[i].str);
                    exit(1);
                }
                --num_ptr;
                double complex second = nums[num_ptr];
                --num_ptr;
                double complex first = nums[num_ptr];
                double complex result = 0;
                switch (input->buf[i].str[0]) {
                    case '+':
                        result = first + second;
                        break;
                    case '-':
                        result = first - second;
                        break;
                    case '*':
                        result = first * second;
                        break;
                    case '/':
                        if(second == 0) {
                            printf("Division by zero :(");
                            exit(0);
                        }
                        result = first / second;
                        break;
                    case '^':
                        result = cpow(first, second);
                        break;
                }
                nums[num_ptr] = result;
                ++num_ptr;
                break;
            case 4:
                if(num_ptr < 1) {
                    printf("Not enough arguments for function %s", input->buf[i].str);
                    exit(1);
                }
                --num_ptr;
                double complex val = nums[num_ptr];
                double complex res = 0;
                switch (input->buf[i].str[0]) {
                    case '+':
                        res = val;
                        break;
                    case '-':
                        res = -val;
                        break;
                }
                nums[num_ptr] = res;
                ++num_ptr;
                break;
        }
    }
    if(num_ptr != 1) {
        printf("We've done something strange. Check your expression again pls");
        exit(1);
    }
    return nums[0];
}
