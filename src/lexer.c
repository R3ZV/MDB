#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

size_t parse_article(const char *path, Token *tokens) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("[ERROR]: '%s' not found in 'articles' directory\n", path);
        return 0;
    }

    char buff[1024];
    size_t tokens_len = 0;
    Token token;
    while (fgets(buff, sizeof(buff), fp)) {
        size_t buff_len = strlen(buff);
        char content[buff_len];

        size_t idx = 0;
        if (buff[idx] == '#') {
            int level = 0;
            while (idx < buff_len && buff[idx] == '#') {
                level++;
                idx++;
            }

            if (level > 6) {
                strncpy(content, buff, buff_len - 2);
                token.type = P;
            } else {
                if (idx < buff_len - 1 && buff[idx] == ' ') {
                    strncpy(content, buff + idx + 1, buff_len - 1);
                    token.type = H1 + level - 1;
                } else {
                    token.type = P;
                }
            }
            strcpy(token.content, content);
            tokens[tokens_len++] = token;
        } else if (isalpha(buff[idx])) {
            strncpy(content, buff, buff_len - 1);
            strcpy(token.content, content);
            token.type = P;
            tokens[tokens_len++] = token;
        } else {
            continue;
        }
    }

    fclose(fp);
    return tokens_len;
}
