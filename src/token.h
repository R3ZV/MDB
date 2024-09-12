#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>
#include <string.h>

typedef enum TokenType {
    H1,
    H2,
    H3,
    H4,
    H5,
    H6,
    TEXT,
    LINE_BREAK
} TokenType;

typedef struct Token {
    TokenType type;
    char content[1024];
} Token;

bool token_equal(Token a, Token b);

#endif // TOKEN_H
