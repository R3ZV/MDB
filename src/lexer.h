#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdbool.h>

#include "token.h"

size_t lex(const char *path, Token *tokens);
void tokenize(const char *stream, size_t *tokens_c, Token *tokens);

#endif // LEXER_H
