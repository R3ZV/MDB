#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

typedef enum TokenType {
    H1,
    H2,
    H3,
    H4,
    H5,
    H6,
    P
} TokenType;

typedef struct Token {
    TokenType type;
    char content[100];
} Token;

size_t parse_article(const char *path, Token *data);

#endif // LEXER_H
