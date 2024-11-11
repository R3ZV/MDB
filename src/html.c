#include "html.h"

void html_elm(
    char *const buff,
    const char *const tag,
    const char *const props,
    const char *const content)
{

    if (buff == NULL) {
        printf("[ERROR]: Null (buffer) pointer for HTML element\n");
        exit(1);
    }

    if (tag == NULL) {
        printf("[ERROR]: Null (tag) pointer for HTML element\n");
        exit(1);
    }

    if (props == NULL) {
        printf("[ERROR]: Null (props) pointer for HTML element\n");
        exit(1);
    }

    if (content == NULL) {
        printf("[ERROR]: Null (content) pointer for HTML element\n");
        exit(1);
    }
    sprintf(buff, "<%s%s>%s</%s>", tag, props, content, tag);
}

typedef struct HtmlFile {
    char *name;
    char *path;

    // HTML contents
    size_t content_c;
    char **content;
} HtmlFile;

HtmlFile InitHtmlFile() {
    HtmlFile file;
    file.name = calloc(64, sizeof(char));
    if (file.name == NULL) {
        printf("[ERROR]: Couldn't allocate enough memmory for an html file name\n");
        exit(1);
    }

    file.path = calloc(64, sizeof(char));
    if (file.path == NULL) {
        printf("[ERROR]: Couldn't allocate enough memmory for an html file path\n");
        exit(1);
    }

    file.content_c = 0;
    file.content = calloc(256, sizeof(char *));
    if (file.content == NULL) {
        printf("[ERROR]: Couldn't allocate enough memmory for an html file content\n");
        exit(1);
    }

    for (size_t i = 0; i < 256; i++) {
        file.content[i] = calloc(1024, sizeof(char));
        if (file.content == NULL) {
            printf("[ERROR]: Couldn't allocate enough memmory for an html file content value\n");
            exit(1);
        }
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
    sprintf(css_src, "<link rel=\"stylesheet\" href=\"../styles/%s.css\">", file_name);
    strcpy(content[i++], css_src);
    strcpy(content[i++], "<link rel=\"stylesheet\" href=\"../styles/styles.css\">");
    if (strcmp(file_name, "blog") != 0) {
        strcpy(content[i++], "<link rel=\"stylesheet\" href=\"../styles/post.css\">");
    }
    strcpy(content[i++], "</head>");
    strcpy(content[i++], "<body>");
    strcpy(content[i++], "<main>");
    return i;
}

/// strcopy copyes byte by byte the `src` content
/// into `dest` but starting from `dist_i`.
///
/// The user has to check that dest_i + strlen(src) + 1 < sizeof(dest) / sizeof(char)
/// +1 comes from having to add '\0' at the end of the string.
size_t strcopy(char *dest, const size_t dest_i, const char *const src) {
    for (size_t i = 0; i < strlen(src); i++) {
        dest[dest_i + i] = src[i];
    }
    dest[dest_i + strlen(src)] = '\0';
    return strlen(src);
}

bool create_html(const HtmlFile *const file) {
    dbg("Creating html file with name: %s\n", file->name);
    char path[128];
    sprintf(path, "%s/%s.html", file->path, file->name);

    FILE *fp = fopen(path, "wb");
    if (fp == NULL) {
        printf("[ERROR]: couldn't open %s\n", file->path);
        return false;
    }

    for (size_t i = 0; i < file->content_c; i++) {
        dbg("Writing line %zu of html file with content: %s\n", i, file->content[i]);
        fwrite(file->content[i], sizeof(char), strlen(file->content[i]), fp);
        if (i + 1 < file->content_c) {
            fwrite("\n", sizeof(char), strlen("\n"), fp);
        }
    }

    dbg("Html file [%s] created!\n", file->name);
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

    html_elm(file.content[file.content_c++], "a", " href=\"./blog.html\"", "&larr; Back");
    strcpy(file.content[file.content_c++], "<div class=\"post-content\">");

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
                if (content_buffer_c + strlen(tokens[i].content) + 1 >= sizeof(content_buffer) / sizeof(char)) {
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
    strcpy(file.content[file.content_c++], "</div>");
    strcpy(file.content[file.content_c++], "</main>");
    strcpy(file.content[file.content_c++], "</body>");
    strcpy(file.content[file.content_c++], "</html>");

    bool result = create_html(&file);
    FreeHtmlFile(&file);

    post.created = result;

    return post;
}

bool update_blog_links(const size_t blogs_c, const BlogPost *const posts) {
    dbg("Updating blog links...\n", "");
    HtmlFile file = InitHtmlFile();
    strcpy(file.name, "blog");
    strcpy(file.path, "./blogs");

    file.content_c += html_template(file.content, file.name);
    html_elm(file.content[file.content_c++], "a", " href=\"../index.html\"", "Home");
    for (size_t i = 0; i < blogs_c; i++) {
        strcpy(file.content[file.content_c++], "<div class=\"blog-post\">");
        html_elm(file.content[file.content_c++], "h1", "", posts[i].title);
        html_elm(file.content[file.content_c++], "p", "", posts[i].description);

        char blog_link[64];
        sprintf(blog_link, " href=\"%s.html\"", posts[i].name);

        html_elm(file.content[file.content_c++], "a", blog_link, "Read");
        strcpy(file.content[file.content_c++], "</div>");
    }

    strcpy(file.content[file.content_c++], "</main>");
    strcpy(file.content[file.content_c++], "</body>");
    strcpy(file.content[file.content_c++], "</html>");

    bool result = create_html(&file);

    FreeHtmlFile(&file);

    return result;
}
