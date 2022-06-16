#include "functions.h"
#include "parsing.h"

int main() {
    FILE *input = fopen("input.txt", "r");
    char buff[INPUT_BUFFER_SIZE];
    stack *stacks[STACK_BUFFER_SIZE];
    int rows = 0;
    while (fgets(buff, INPUT_BUFFER_SIZE, input) != NULL) {
        eraseReductant(buff);
        stack *postfix = initStack();
        unsigned int token_cnt = 0;
        char **tokens = splitTokens(buff, &token_cnt);
        buildStack(tokens, token_cnt, postfix);
        memset(buff, 0, sizeof(buff));
        stacks[rows] = postfix;
        ++rows;
        free(tokens);
    }
    char* variables[STACK_BUFFER_SIZE];
    double complex var_values[STACK_BUFFER_SIZE];
    int var_counter = 0;

    for (int i = rows - 1; i >= 0; --i) {
        double complex res = evaluate(stacks[i], variables, var_values, &var_counter);
        if(i == 0) {
            if(fabs(cimag(res)) < 1e-7) {
                printf("RESULT IS: %f", creal(res));
            } else if(cimag(res) > 0) {
                printf("RESULT IS: %f + %fi ", creal(res), cimag(res));
            } else {
                printf("RESULT IS: %f%fi ", creal(res), cimag(res));
            }
        }
    }
    for (int i = 0; i < rows; ++i) {
        free(stacks[i]);
    }
    return 0;
}
