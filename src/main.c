#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdint.h>
#include <sys/stat.h>

#include "debug.h"
#include "lexer.h"
#include "html.h"
#include "blog.h"

void cli_help() {
    printf("Usage: cli [command]\n");
    printf("Commands:\n");
    printf("help               prints this message\n");
    printf("gen                generate articles\n");
}

void generate_blogs() {
    DIR *dp = opendir("articles");

    if (dp == NULL) {
        printf("[ERROR]: 'articles' directory not found\n");
        return;
    }

    struct stat st = {0};
    if (stat("blogs", &st) == -1) {
        mkdir("blogs", 0700);
    }

    size_t succ_blogs = 0;
    BlogPost blogs[1024];
    uint32_t blogs_c = 0;
    for (struct dirent *entry = readdir(dp); entry != NULL; entry = readdir(dp)) {
        if (strlen(entry->d_name) <= 3) {
            printf("[WARN]: Invalid file '%s'\n", entry->d_name);
            continue;
        }

        uint32_t f_len = strlen(entry->d_name);
        if (entry->d_name[f_len - 3] != '.' &&
            entry->d_name[f_len - 2] != 'm' &&
            entry->d_name[f_len - 1] != 'd') {
            continue;
        }

        blogs_c++;

        char article_path[strlen(entry->d_name) + strlen("./articles/")];
        sprintf(article_path, "./articles/%s", entry->d_name);
        dbg("Lexing: %s\n", article_path);

        Token tokens[256];
        size_t tokens_c = lex(article_path, tokens);

        dbg("Got %zu tokens:\n", tokens_c);
        for (size_t token = 0; token < tokens_c; token++) {
            dbg("Token [%zu] has type (%d) with content '%s'\n",
                token,
                tokens[token].type,
                tokens[token].content
            );
        }

        char f_name[128];
        strncpy(f_name, entry->d_name, strlen(entry->d_name) - 3);

        BlogPost new_post = parse_into_html(tokens_c, tokens, f_name);
        if (new_post.created == false) {
            FreeBlogPost(&new_post);
            printf("[ERROR]: Couldn't parse the tokens into a HTML file\n");
            continue;
        }
        blogs[succ_blogs++] = new_post;
    }

    if (update_blog_links(succ_blogs, blogs)) {
        printf("Generated: %zu/%d blogs.\n", succ_blogs, blogs_c);
    } else {
        printf("[ERROR]: Something went wrong while trying to update the blog file\n");
    }

    for (size_t i = 0; i < succ_blogs; i++) {
        FreeBlogPost(&blogs[i]);
    }

    closedir(dp);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("[ERROR]: Invalid number of arguments!\n");
        printf("Use 'cli help'\n");
        return 1;
    }

    char command[10];
    if (strlen(argv[1]) > sizeof(command)) {
        printf("[ERROR]: Invalid argument!\n");
        printf("Use 'cli help'\n");
        return 1;
    }

    strcpy(command, argv[1]);

    if (strcmp(command, "help") == 0) {
        cli_help();
    } else if (strcmp(command, "gen") == 0) {
        generate_blogs();
    } else {
        cli_help();
        printf("[ERROR]: %s unknown command\n", command);
    }
    return 0;
}
