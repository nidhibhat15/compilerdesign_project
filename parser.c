#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 50

// Token types
typedef enum {
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_ASSIGN,   // :=
    TOKEN_SEMI,     // ;
    TOKEN_WRITE,    // write keyword
    TOKEN_PLUS,     // +
    TOKEN_MUL,      // *
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LEN];
} Token;

char input[1024];
int pos = 0;
Token currentToken;

// Function declarations
void getNextToken();
void error(const char *msg);
void stmt_sequence();
void stmt();
void assign_stmt();
void write_stmt();
void expr();
void term();
void factor();

void skipWhitespace() {
    while (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n' || input[pos] == '\r')
        pos++;
}

int isKeyword(const char *lex) {
    return strcmp(lex, "write") == 0;
}

void getNextToken() {
    skipWhitespace();

    if (input[pos] == '\0') {
        currentToken.type = TOKEN_EOF;
        strcpy(currentToken.lexeme, "EOF");
        return;
    }

    if (isalpha(input[pos])) {
        int i = 0;
        while (isalnum(input[pos])) {
            if (i < MAX_TOKEN_LEN - 1)
                currentToken.lexeme[i++] = input[pos];
            pos++;
        }
        currentToken.lexeme[i] = '\0';
        currentToken.type = isKeyword(currentToken.lexeme) ? TOKEN_WRITE : TOKEN_ID;
        return;
    }

    if (isdigit(input[pos])) {
        int i = 0;
        while (isdigit(input[pos])) {
            if (i < MAX_TOKEN_LEN - 1)
                currentToken.lexeme[i++] = input[pos];
            pos++;
        }
        currentToken.lexeme[i] = '\0';
        currentToken.type = TOKEN_NUMBER;
        return;
    }

    if (input[pos] == ':' && input[pos + 1] == '=') {
        strcpy(currentToken.lexeme, ":=");
        currentToken.type = TOKEN_ASSIGN;
        pos += 2;
        return;
    }

    switch (input[pos]) {
        case ';':
            currentToken.type = TOKEN_SEMI;
            strcpy(currentToken.lexeme, ";");
            pos++;
            break;
        case '+':
            currentToken.type = TOKEN_PLUS;
            strcpy(currentToken.lexeme, "+");
            pos++;
            break;
        case '*':
            currentToken.type = TOKEN_MUL;
            strcpy(currentToken.lexeme, "*");
            pos++;
            break;
        case '(':
            currentToken.type = TOKEN_LPAREN;
            strcpy(currentToken.lexeme, "(");
            pos++;
            break;
        case ')':
            currentToken.type = TOKEN_RPAREN;
            strcpy(currentToken.lexeme, ")");
            pos++;
            break;
        default:
            currentToken.type = TOKEN_UNKNOWN;
            snprintf(currentToken.lexeme, MAX_TOKEN_LEN, "%c", input[pos]);
            pos++;
            break;
    }
}

void error(const char *msg) {
    printf("Error: %s at '%s'\n", msg, currentToken.lexeme);
    exit(1);
}

// Parser functions
void stmt_sequence() {
    stmt(); // parse first statement

    while (currentToken.type == TOKEN_SEMI) {
        getNextToken();  // consume the semicolon

        // Stop if next token is EOF (end of input)
        if (currentToken.type == TOKEN_EOF)
            break;

        stmt(); // parse next statement
    }
}

void stmt() {
    if (currentToken.type == TOKEN_ID)
        assign_stmt();
    else if (currentToken.type == TOKEN_WRITE)
        write_stmt();
    else
        error("Expected statement");
}

void assign_stmt() {
    getNextToken(); // consume ID
    if (currentToken.type != TOKEN_ASSIGN)
        error("Expected ':='");
    getNextToken(); // consume :=
    expr();         // parse expression
}

void write_stmt() {
    getNextToken(); // consume 'write'
    expr();         // parse expression
}

void expr() {
    term();
    while (currentToken.type == TOKEN_PLUS) {
        getNextToken(); // consume +
        term();
    }
}

void term() {
    factor();
    while (currentToken.type == TOKEN_MUL) {
        getNextToken(); // consume *
        factor();
    }
}

void factor() {
    if (currentToken.type == TOKEN_NUMBER || currentToken.type == TOKEN_ID) {
        getNextToken(); // consume number or id
    } else if (currentToken.type == TOKEN_LPAREN) {
        getNextToken(); // consume (
        expr();
        if (currentToken.type != TOKEN_RPAREN)
            error("Expected ')'");
        getNextToken(); // consume )
    } else {
        error("Expected factor");
    }
}

// Main function
int main() {
    strcpy(input,
        "x := 2 +  * 4;\n"
        "write x;"
    );

    getNextToken();       // Initialize first token
    stmt_sequence();      // Start parsing

    if (currentToken.type != TOKEN_EOF) {
        error("Unexpected input after program");
    }

    printf("Parsing completed successfully.\n");
    return 0;
}