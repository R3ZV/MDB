#include "token.h"

bool token_equal(Token a, Token b) {
    if (a.type != b.type) {
        return false;
    }

    if (strcmp(a.content, b.content) != 0) {
        return false;
    }

    return true;
}
