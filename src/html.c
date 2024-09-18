#include "html.h"

#define html_elm(buff, tag, props, content) sprintf(buff, "<%s%s>%s</%s>", tag, props, content, tag)

bool create_html(
    const size_t content_c,
    char *const content[],
    const char *f_name,
    const char *f_path)
{
    char path[128];
    sprintf(path, "%s/%s.html", f_path, f_name);

    FILE *fp = fopen(path, "wb");
    if (fp == NULL) {
        printf("[ERROR]: couldn't open %s\n", path);
        return false;
    }

    for (size_t i = 0; i < content_c; i++) {
        fwrite(content[i], sizeof(char), strlen(content[i]), fp);
        if (i + 1 < content_c) {
            fwrite("\n", sizeof(char), strlen("\n"), fp);
        }
    }

    fclose(fp);
    return true;
}

bool parse_into_html(const size_t tokens_c, const Token *tokens, const char *f_name) {
    const size_t MAX_HTML_ELEMENTS = 128;
    char *html_content[MAX_HTML_ELEMENTS];
    for (size_t i = 0; i < MAX_HTML_ELEMENTS; i++) {
        html_content[i] = (char *) malloc(100 * sizeof(char));
    }

    strcpy(html_content[0], "<html>");

    size_t html_c = 1;
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
                    return false;
                }
                html_elm(html_content[html_c++], tag, "", tokens[i + 1].content);
                i++;
                break;
            }

            case TEXT:
                html_elm(html_content[html_c++], "p", "", tokens[i].content);
                break;

            case LINE_BREAK:
            default:
                break;
        }
    }

    strcpy(html_content[html_c++], "<html>");
    bool result = create_html(html_c, html_content, f_name, "./blogs");

    for (size_t i = 0; i < MAX_HTML_ELEMENTS; i++) {
        free(html_content[i]);
    }

    return result;
}

bool update_blog_links(const size_t blogs_c, char **const blogs_name) {
    const size_t MAX_HTML_ELEMENTS = 128;
    char *html_content[MAX_HTML_ELEMENTS];
    for (size_t i = 0; i < MAX_HTML_ELEMENTS; i++) {
        html_content[i] = (char *) malloc(100 * sizeof(char));
    }

    strcpy(html_content[0], "<html>");
    size_t html_c = 1;

    for (size_t i = 0; i < blogs_c; i++) {
        char blog_link[64];
        sprintf(blog_link, " href=\"./blogs/%s.html\"", blogs_name[i]);

        html_elm(html_content[html_c++], "a", blog_link, blogs_name[i]);
    }

    strcpy(html_content[html_c++], "<html>");
    bool result = create_html(html_c, html_content, "blog", ".");

    for (size_t i = 0; i < MAX_HTML_ELEMENTS; i++) {
        free(html_content[i]);
    }

    return result;
}
