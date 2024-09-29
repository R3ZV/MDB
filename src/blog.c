#include "blog.h"

BlogPost InitBlogPost() {
    BlogPost post;
    post.created = false;

    post.name = (char *) malloc(64 * sizeof(char));
    strcpy(post.name, "");

    post.title = (char *) malloc(64 * sizeof(char));
    strcpy(post.title, "");

    post.description = (char *) malloc(256 * sizeof(char));
    strcpy(post.description, "");

    return post;
}

void FreeBlogPost(BlogPost *post) {
    free(post->name);
    free(post->title);
    free(post->description);
}
