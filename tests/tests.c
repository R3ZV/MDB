#include "tests.h"
#include "../src/lexer.h"

// Basically it is like assert but if
// the condition evalutes to false we print
// a message to know what the failed assertion means.
bool expect(bool eval, char *msg) {
    if (!eval) {
        printf("%s\n", msg);
        return true;
    }
    return false;
}

bool test_h1_token() {
    char h1[] = "# Header1";

    Token want[2]; 
    want[0].type = H1;
    strcpy(want[0].content, "");

    want[1].type = TEXT;
    strcpy(want[1].content, "Header1");

    size_t tokens_c = 0;
    Token got[2];
    tokenize(h1, &tokens_c, got);

    for (size_t i = 0; i < 2; i++) {
        char err_msg[100];
        sprintf(err_msg, "'test_headers_tokens' failed: Tokens at pos %ld don't match", i);
        bool err = expect(token_equal(got[i], want[i]), err_msg);
        if (err) {
            printf("Got token with id: %d and content '%s'\n", got[i].type, got[i].content);
            printf("Wanted token with id: %d and content '%s'\n", want[i].type, want[i].content);
            return true;
        }
    }

    return false;
}

bool test_fake_header_token() {
    char header[] = "###";

    Token want[1];
    want[0].type = TEXT;
    strcpy(want[0].content, "###");

    size_t tokens_c = 0;
    Token got[1];
    tokenize(header, &tokens_c, got);

    for (size_t i = 0; i < 1; i++) {
        char err_msg[100];
        sprintf(err_msg, "'test_fake_header_token' failed: Tokens at pos %ld don't match", i);
        bool err = expect(token_equal(got[i], want[i]), err_msg);
        if (err) {
            printf("Got token with id: %d and content '%s'\n", got[i].type, got[i].content);
            printf("Wanted token with id: %d and content '%s'\n", want[i].type, want[i].content);
            return true;
        }
    }

    return false;
}

bool test_header_text_line_break() {
    char file_content[3][20] = {
        "# Header 1\n",
        "\n",
        "Some paragraph text\n"
    };

    Token want[4];
    want[0].type = H1;
    strcpy(want[0].content, "");

    want[1].type = TEXT;
    strcpy(want[1].content, "Header 1");

    want[2].type = LINE_BREAK;
    strcpy(want[2].content, "");

    want[3].type = TEXT;
    strcpy(want[3].content, "Some paragraph text");

    size_t tokens_c = 0;
    Token got[4];
    for (size_t i = 0; i < 3; i++) {
        file_content[i][strcspn(file_content[i], "\r\n")] = 0;
        tokenize(file_content[i], &tokens_c, got);
    }

    for (size_t i = 0; i < 4; i++) {
        char err_msg[100];
        sprintf(err_msg, "'test_fake_header_token' failed: Tokens at pos %ld don't match", i);
        bool err = expect(token_equal(got[i], want[i]), err_msg);
        if (err) {
            printf("Got token with id: %d and content '%s'\n", got[i].type, got[i].content);
            printf("Wanted token with id: %d and content '%s'\n", want[i].type, want[i].content);
            return true;
        }
    }

    return false;
}
