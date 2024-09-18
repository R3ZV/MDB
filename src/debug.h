#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define dbg(fmt, ...) \
    do { \
        if (DEBUG) \
            fprintf(stderr, "%s:%d:%s(): " fmt, \
                    __FILE__, __LINE__, __func__, __VA_ARGS__); \
    } while (0)
#else
#define dbg(fmt, ...) ((void)0)
#endif // DEBUG

#endif // DEBUG_H
