#include "html.h"

#define html_elm(buff, tag, props, content) sprintf(buff, "<%s %s>%s</%s>", tag, props, content, tag)

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
    char *html_content[128];
    for (size_t i = 0; i < 128; i++) {
        html_content[i] = (char *) malloc(100 * sizeof(char));
    }

    html_content[0] = "<html>";

    size_t html_i = 1;
    for (size_t i = 0; i < tokens_c; i++) {
        switch(tokens[i].type) {
            case H1:
                if (i + 1 < tokens_c && tokens[i + 1].type != TEXT) {
                    printf("[ERROR]: Invalid token, after an H1 token \
                           there should be a TEXT TOKEN");
                    return false;
                }
                html_elm(html_content[html_i++], "h1", "", tokens[i + 1].content);
                i++;
                break;

            case H2:
                if (i + 1 < tokens_c && tokens[i + 1].type != TEXT) {
                    printf("[ERROR]: Invalid token, after an H2 token \
                           there should be a TEXT TOKEN");
                    return false;
                }
                html_elm(html_content[html_i++], "h2", "", tokens[i + 1].content);
                i++;
                break;

            case H3:
                if (i + 1 < tokens_c && tokens[i + 1].type != TEXT) {
                    printf("[ERROR]: Invalid token, after an H3 token \
                           there should be a TEXT TOKEN");
                    return false;
                }
                html_elm(html_content[html_i++], "h3", "", tokens[i + 1].content);
                i++;
                break;

            case H4:
                if (i + 1 < tokens_c && tokens[i + 1].type != TEXT) {
                    printf("[ERROR]: Invalid token, after an H4 token \
                           there should be a TEXT TOKEN");
                    return false;
                }
                html_elm(html_content[html_i++], "h4", "", tokens[i + 1].content);
                i++;
                break;

            case H5:
                if (i + 1 < tokens_c && tokens[i + 1].type != TEXT) {
                    printf("[ERROR]: Invalid token, after an H5 token \
                           there should be a TEXT TOKEN");
                    return false;
                }
                html_elm(html_content[html_i++], "h5", "", tokens[i + 1].content);
                i++;
                break;

            case H6:
                if (i + 1 < tokens_c && tokens[i + 1].type != TEXT) {
                    printf("[ERROR]: Invalid token, after an H6 token \
                           there should be a TEXT TOKEN");
                    return false;
                }
                html_elm(html_content[html_i++], "h6", "", tokens[i + 1].content);
                i++;
                break;

            case TEXT:
                html_elm(html_content[html_i++], "p", "", tokens[i].content);
                break;

            case LINE_BREAK:
            default:
                break;
        }
    }

    strcpy(html_content[html_i++], "<html>");
    return create_html(html_i, html_content, f_name, "./blogs");
}

bool update_blog_links(const size_t blogs_c, const char **blogs_name) {
    char *html_content[128];
    for (size_t i = 0; i < 128; i++) {
        html_content[i] = (char *) malloc(100 * sizeof(char));
    }

    html_content[0] = "<html>";
    size_t html_c = 1;

    for (size_t i = 0; i < blogs_c; i++) {
        char blog_link[128];
        sprintf(blog_link, "href=\"./blogs/%s.html\"", blogs_name[i]);

        html_elm(html_content[html_c++], "a", blog_link, blogs_name[i]);
    }

    html_content[html_c++] = "<html>";
    return create_html(html_c, html_content, "blog", ".");
}
