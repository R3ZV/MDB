#include "blog.h"

BlogPost InitBlogPost() {
    BlogPost post;
    post.created = false;

    post.name = calloc(64, sizeof(char));
    if (post.name == NULL) {
        printf("[ERROR]: Couldn't allocate enough memmory for a blog post name\n");
        exit(1);
    }

    post.title = calloc(64, sizeof(char));
    if (post.title == NULL) {
        printf("[ERROR]: Couldn't allocate enough memmory for a blog post title\n");
        exit(1);
    }

    post.description = calloc(1024, sizeof(char));
    if (post.description == NULL) {
        printf("[ERROR]: Couldn't allocate enough memmory for a blog post description\n");
        exit(1);
    }

    return post;
}

void FreeBlogPost(BlogPost *post) {
    free(post->name);
    free(post->title);
    free(post->description);
}
