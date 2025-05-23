#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK 100
#define MAX_INPUT 100

// Token codes for terminals
#define ID 1
#define PLUS 2
#define MUL 3
#define LPAREN 4
#define RPAREN 5
#define DOLLAR 6

// Action table codes
#define SHIFT 1
#define REDUCE 2
#define ACCEPT 3
#define ERROR 0

// Grammar rules count
#define NUM_RULES 6

// Production rules:
// 1: E → E + T
// 2: E → T
// 3: T → T * F
// 4: T → F
// 5: F → ( E )
// 6: F → id

typedef struct {
    int action;    // SHIFT, REDUCE, ACCEPT, ERROR
    int number;    // state for SHIFT, rule number for REDUCE
} Action;

typedef struct {
    int lhs;    // left-hand side non-terminal (E=7, T=8, F=9)
    int rhs_len; // number of symbols on right-hand side
} Rule;

// Parsing tables (states × terminals/non-terminals)
// States: 0 to 11 (12 states)
Action ACTION[12][7];
int GOTO[12][10]; // For non-terminals E=7, T=8, F=9 (indices 7,8,9)

// Stack for states
int stack[MAX_STACK];
int top = -1;

// Input tokens
int input[MAX_INPUT];
int ip = 0;

// Grammar rules definition
Rule rules[NUM_RULES+1];

// Map token strings to token codes
int getTokenCode(char *tok) {
    if(strcmp(tok, "id") == 0) return ID;
    if(strcmp(tok, "+") == 0) return PLUS;
    if(strcmp(tok, "*") == 0) return MUL;
    if(strcmp(tok, "(") == 0) return LPAREN;
    if(strcmp(tok, ")") == 0) return RPAREN;
    if(strcmp(tok, "$") == 0) return DOLLAR;
    return -1;
}

void init_rules() {
    rules[1].lhs = 7; rules[1].rhs_len = 3; // E -> E + T
    rules[2].lhs = 7; rules[2].rhs_len = 1; // E -> T
    rules[3].lhs = 8; rules[3].rhs_len = 3; // T -> T * F
    rules[4].lhs = 8; rules[4].rhs_len = 1; // T -> F
    rules[5].lhs = 9; rules[5].rhs_len = 3; // F -> ( E )
    rules[6].lhs = 9; rules[6].rhs_len = 1; // F -> id
}

void init_parsing_table() {
    int i, j;
    // Initialize all ACTION and GOTO to ERROR/ -1
    for(i=0; i<12; i++) {
        for(j=0; j<7; j++) {
            ACTION[i][j].action = ERROR;
            ACTION[i][j].number = -1;
        }
        for(j=0; j<10; j++) GOTO[i][j] = -1;
    }

    // ACTION[state][token] = {action, number}
    // Manually filled based on SLR(1) table for grammar

    // State 0
    ACTION[0][ID] = (Action){SHIFT, 5};
    ACTION[0][LPAREN] = (Action){SHIFT, 4};
    GOTO[0][7] = 1; // E
    GOTO[0][8] = 2; // T
    GOTO[0][9] = 3; // F

    // State 1
    ACTION[1][PLUS] = (Action){SHIFT, 6};
    ACTION[1][DOLLAR] = (Action){ACCEPT, 0};

    // State 2
    ACTION[2][PLUS] = (Action){REDUCE, 2}; // E -> T
    ACTION[2][MUL] = (Action){SHIFT, 7};
    ACTION[2][RPAREN] = (Action){REDUCE, 2};
    ACTION[2][DOLLAR] = (Action){REDUCE, 2};

    // State 3
    ACTION[3][PLUS] = (Action){REDUCE, 4}; // T -> F
    ACTION[3][MUL] = (Action){REDUCE, 4};
    ACTION[3][RPAREN] = (Action){REDUCE, 4};
    ACTION[3][DOLLAR] = (Action){REDUCE, 4};

    // State 4
    ACTION[4][ID] = (Action){SHIFT, 5};
    ACTION[4][LPAREN] = (Action){SHIFT, 4};
    GOTO[4][7] = 8;
    GOTO[4][8] = 2;
    GOTO[4][9] = 3;

    // State 5
    ACTION[5][PLUS] = (Action){REDUCE, 6}; // F -> id
    ACTION[5][MUL] = (Action){REDUCE, 6};
    ACTION[5][RPAREN] = (Action){REDUCE, 6};
    ACTION[5][DOLLAR] = (Action){REDUCE, 6};

    // State 6
    ACTION[6][ID] = (Action){SHIFT, 5};
    ACTION[6][LPAREN] = (Action){SHIFT, 4};
    GOTO[6][8] = 9;
    GOTO[6][9] = 3;

    // State 7
    ACTION[7][ID] = (Action){SHIFT, 5};
    ACTION[7][LPAREN] = (Action){SHIFT, 4};
    GOTO[7][9] = 10;

    // State 8
    ACTION[8][PLUS] = (Action){SHIFT, 6};
    ACTION[8][RPAREN] = (Action){SHIFT, 11};

    // State 9
    ACTION[9][PLUS] = (Action){REDUCE, 1}; // E -> E + T
    ACTION[9][MUL] = (Action){SHIFT, 7};
    ACTION[9][RPAREN] = (Action){REDUCE, 1};
    ACTION[9][DOLLAR] = (Action){REDUCE, 1};

    // State 10
    ACTION[10][PLUS] = (Action){REDUCE, 3}; // T -> T * F
    ACTION[10][MUL] = (Action){REDUCE, 3};
    ACTION[10][RPAREN] = (Action){REDUCE, 3};
    ACTION[10][DOLLAR] = (Action){REDUCE, 3};

    // State 11
    ACTION[11][PLUS] = (Action){REDUCE, 5}; // F -> (E)
    ACTION[11][MUL] = (Action){REDUCE, 5};
    ACTION[11][RPAREN] = (Action){REDUCE, 5};
    ACTION[11][DOLLAR] = (Action){REDUCE, 5};
}

void push(int state) {
    if(top < MAX_STACK - 1) {
        stack[++top] = state;
    } else {
        printf("Stack overflow\n");
        exit(1);
    }
}

int pop() {
    if(top >= 0) return stack[top--];
    else {
        printf("Stack underflow\n");
        exit(1);
    }
}

int top_stack() {
    if(top >= 0) return stack[top];
    else return -1;
}

// Simple tokenizer: accepts input as tokens separated by spaces
// Input example: id + id * id $
void tokenize(char *line) {
    char *tok = strtok(line, " \n\t");
    int i = 0;
    while(tok != NULL) {
        int code = getTokenCode(tok);
        if(code == -1) {
            printf("Invalid token: %s\n", tok);
            exit(1);
        }
        input[i++] = code;
        tok = strtok(NULL, " \n\t");
    }
    input[i] = DOLLAR; // End marker
}

void parse() {
    push(0);
    int a = input[ip];

    while(1) {
        int s = top_stack();
        Action act = ACTION[s][a];

        if(act.action == SHIFT) {
            push(act.number);
            ip++;
            a = input[ip];
        }
        else if(act.action == REDUCE) {
            Rule r = rules[act.number];
            int rhs_len = r.rhs_len;
            while(rhs_len--) pop();
            int t = top_stack();
            push(GOTO[t][r.lhs]);
            printf("Reduced by rule %d\n", act.number);
        }
        else if(act.action == ACCEPT) {
            printf("Input string accepted.\n");
            break;
        }
        else {
            printf("Syntax error at token index %d\n", ip);
            break;
        }
    }
}

int main() {
    char line[256];

    printf("Enter the input string (tokens separated by spaces, end with $):\n");
    fgets(line, sizeof(line), stdin);

    init_rules();
    init_parsing_table();
    tokenize(line);

    parse();

    return 0;
}