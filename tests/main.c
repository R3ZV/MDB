#include <stdio.h>
#include <stdint.h>

#include "tests.h"

int main(void) {
    uint32_t tests = 0;
    uint32_t errs = 0;

    errs += test_h1_token();
    tests++;

    errs += test_fake_header_token();
    tests++;

    errs += test_header_text_line_break();
    tests++;

    printf("Tests passed: %d/%d\n", tests - errs, tests);
    return 0;
}
