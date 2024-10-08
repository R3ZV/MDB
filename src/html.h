#ifndef HTML_H
#define HTML_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "token.h"
#include "blog.h"
#include "debug.h"

BlogPost parse_into_html(const size_t tokens_c, const Token *tokens, const char *f_name);
bool update_blog_links(const size_t blogs_c, const BlogPost *const posts);

#endif // HTML_H
