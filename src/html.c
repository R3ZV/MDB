#include "html.h"

#define html_elm(buff, tag, props, content) sprintf(buff, "<%s%s>%s</%s>", tag, props, content, tag)

typedef struct HtmlFile {
    char *name;
    char *path;

    // HTML contents
    size_t content_c;
    char **content;
} HtmlFile;

HtmlFile InitHtmlFile() {
    HtmlFile file;
    file.name = (char *) malloc(64 * sizeof(char));
    strcpy(file.name, "");

    file.path = (char *) malloc(64 * sizeof(char));
    strcpy(file.path, "");

    file.content_c = 0;
    file.content = malloc(256 * sizeof(char *));
    for (size_t i = 0; i < 256; i++) {
        file.content[i] = (char *) malloc(256 * sizeof(char));
        strcpy(file.content[i], "");
    }

    return file;
}

void FreeHtmlFile(HtmlFile *file) {
    free(file->name);
    free(file->path);

    for (size_t i = 0; i < 256; i++) {
        free(file->content[i]);
    }
    free(file->content);
}

size_t html_template(char **content, const char *file_name) {
    size_t i = 0;
    strcpy(content[i++], "<!DOCTYPE html>");
    strcpy(content[i++], "<html lang=\"en\">");
    strcpy(content[i++], "<head>");
    strcpy(content[i++], "<meta charset=\"utf-8\">");

    strcpy(content[i], "<meta name=\"viewport\" content=\"width=device-width, \
                        initial-scale=1\">");

    strcpy(content[i++], "<meta charset=\"utf-8\">");
    html_elm(content[i++], "title", "", "Blog");

    char css_src[64];
    dbg("Css file name: %s\n", file_name);
    sprintf(css_src, "<link rel=\"stylesheet\" href=\"styles/%s.css\">", file_name);
    strcpy(content[i++], css_src);
    strcpy(content[i++], "</head>");
    strcpy(content[i++], "<body>");
    return i;
}

/// strcopy copyes byte by byte the `src` content
/// into `dest` but starting from `dist_i`.
///
/// The user has to check that dset_i + strlen(src) < sizeof(dest) / sizeof(char)
size_t strcopy(char *dest, const size_t dest_i, const char *const src) {
    for (size_t i = 0; i < strlen(src); i++) {
        dest[dest_i + i] = src[i];
    }
    return strlen(src);
}

bool create_html(const HtmlFile *const file) {
    char path[128];
    sprintf(path, "%s/%s.html", file->path, file->name);

    dbg("Html path: %s\n", file->path);
    dbg("Html path file name: %s\n", file->name);
    FILE *fp = fopen(path, "wb");
    if (fp == NULL) {
        printf("[ERROR]: couldn't open %s\n", file->path);
        return false;
    }

    for (size_t i = 0; i < file->content_c; i++) {
        fwrite(file->content[i], sizeof(char), strlen(file->content[i]), fp);
        if (i + 1 < file->content_c) {
            fwrite("\n", sizeof(char), strlen("\n"), fp);
        }
    }

    fclose(fp);
    return true;
}

BlogPost parse_into_html(const size_t tokens_c, const Token *const tokens, const char *const f_name) {
    HtmlFile file = InitHtmlFile();
    strcpy(file.name, f_name);
    strcpy(file.path, "blogs");

    BlogPost post = InitBlogPost();
    strcpy(post.name, f_name);

    file.content_c += html_template(file.content, file.name);

    char content_buffer[1024];
    memset(content_buffer, 0, 1024);

    size_t content_buffer_c = 0;
    for (size_t i = 0; i < tokens_c; i++) {
        switch(tokens[i].type) {
            case H1:
            case H2:
            case H3:
            case H4:
            case H5:
            case H6: {
                // WARN: this assumes that the first 6 elements of the TokenType
                // enum are the 6 headers in this order H1, H2, H3, H4, H5, H6
                char tag[5];
                sprintf(tag, "h%d", tokens[i].type + 1);
                if (i + 1 < tokens_c && tokens[i + 1].type != TEXT) {
                    printf("[ERROR]: Invalid token, after an %s token \
                           there should be a TEXT TOKEN", tag);
                    return post;
                }

                if (strlen(post.title) == 0) {
                    strcpy(post.title, tokens[i + 1].content);
                }

                html_elm(file.content[file.content_c++], tag, "", tokens[i + 1].content);
                i++;
                break;
            }

            case TEXT:
                if (content_buffer_c + strlen(tokens[i].content) >= sizeof(content_buffer) / sizeof(char)) {
                    printf("[ERROR]: Content buffer is smaller then the content it needs to hold\n");
                    exit(1);
                }
                content_buffer_c += strcopy(content_buffer, content_buffer_c, tokens[i].content);

                if (i + 1 < tokens_c && tokens[i + 1].type == TEXT) {
                    content_buffer_c += strcopy(content_buffer, content_buffer_c, " ");
                    continue;
                }

                if (strlen(post.description) == 0) {
                    strcpy(post.description, content_buffer);
                }

                html_elm(file.content[file.content_c++], "p", "", content_buffer);
                memset(content_buffer, 0, 1024);
                content_buffer_c = 0;
                break;

            case LINE_BREAK:
            default:
                break;
        }
    }
    strcpy(file.content[file.content_c++], "</body>");
    strcpy(file.content[file.content_c++], "</html>");

    bool result = create_html(&file);
    FreeHtmlFile(&file);

    post.created = result;

    return post;
}

bool update_blog_links(const size_t blogs_c, const BlogPost *const posts) {
    HtmlFile file = InitHtmlFile();
    strcpy(file.name, "blog");
    strcpy(file.path, "./");

    file.content_c += html_template(file.content, file.name);
    for (size_t i = 0; i < blogs_c; i++) {
        strcpy(file.content[file.content_c++], "<div class=\"blog-post>\"");
        html_elm(file.content[file.content_c++], "h1", "", posts[i].title);
        html_elm(file.content[file.content_c++], "p", "", posts[i].description);

        char blog_link[64];
        sprintf(blog_link, " href=\"blogs/%s.html\"", posts[i].name);

        html_elm(file.content[file.content_c++], "a", blog_link, "Read");
        strcpy(file.content[file.content_c++], "</div>");
    }

    strcpy(file.content[file.content_c++], "</body>");
    strcpy(file.content[file.content_c++], "</html>");

    bool result = create_html(&file);

    FreeHtmlFile(&file);

    return result;
}
