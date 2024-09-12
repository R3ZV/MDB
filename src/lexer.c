#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

void tokenize(const char *stream, size_t *tokens_c, Token *tokens) {
    size_t stream_len = strlen(stream);
    if (stream_len == 0) {
        tokens[*tokens_c].type = LINE_BREAK;
        strcpy(tokens[*(tokens_c)++].content, "");
        return;
    }

    char buff[stream_len];
    size_t buff_i = 0;
    for (size_t i = 0; i < stream_len; i++) {
        buff[buff_i++] = stream[i];
        if (strcmp(buff, "#") == 0) {
            if (i + 1 < stream_len && stream[i + 1] == ' ') {
                tokens[*tokens_c].type = H1;
                strcpy(tokens[(*tokens_c)++].content, "");

                buff_i = 0;
                memset(buff, 0, sizeof(buff));
            }
        } else if (strcmp(buff, "##") == 0) {
            if (i + 1 < stream_len && stream[i + 1] == ' ') {
                tokens[*tokens_c].type = H2;
                strcpy(tokens[(*tokens_c)++].content, "");

                buff_i = 0;
                memset(buff, 0, sizeof(buff));
            }
        } else if (strcmp(buff, "###") == 0) {
            if (i + 1 < stream_len && stream[i + 1] == ' ') {
                tokens[*tokens_c].type = H3;
                strcpy(tokens[(*tokens_c)++].content, "");

                buff_i = 0;
                memset(buff, 0, sizeof(buff));
            }
        } else if (strcmp(buff, "####") == 0) {
            if (i + 1 < stream_len && stream[i + 1] == ' ') {
                tokens[*tokens_c].type = H4;
                strcpy(tokens[(*tokens_c)++].content, "");

                buff_i = 0;
                memset(buff, 0, sizeof(buff));
            }
        } else if (strcmp(buff, "#####") == 0) {
            if (i + 1 < stream_len && stream[i + 1] == ' ') {
                tokens[*tokens_c].type = H5;
                strcpy(tokens[(*tokens_c)++].content, "");

                buff_i = 0;
                memset(buff, 0, sizeof(buff));
            }
        } else if (strcmp(buff, "######") == 0) {
            if (i + 1 < stream_len && stream[i + 1] == ' ') {
                tokens[*tokens_c].type = H6;
                strcpy(tokens[(*tokens_c)++].content, "");

                buff_i = 0;
                memset(buff, 0, sizeof(buff));
            }
        } else if (strcmp(buff, " ") == 0) {
                buff_i = 0;
                memset(buff, 0, sizeof(buff));
        } else {
            if (i + 1 == stream_len) {
                tokens[*tokens_c].type = TEXT;
                buff[buff_i] = '\0';
                strcpy(tokens[(*tokens_c)++].content, buff);

                buff_i = 0;
                memset(buff, 0, sizeof(buff));
            }
        }
    }
}

size_t lex(const char *path, Token *tokens) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("[ERROR]: '%s' not found in 'articles' directory\n", path);
        return 0;
    }

    char buff[1024];
    size_t tokens_c = 0;
    while (fgets(buff, sizeof(buff), fp)) {
        tokenize(buff, &tokens_c, tokens);
    }

    fclose(fp);
    return tokens_c;
}
