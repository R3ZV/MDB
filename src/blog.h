#ifndef BLOG_H
#define BLOG_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct BlogPost {
    bool created;

    char *name;
    char *title;
    char *description;
} BlogPost;

BlogPost InitBlogPost();
void FreeBlogPost(BlogPost *post);

#endif // BLOG_H
