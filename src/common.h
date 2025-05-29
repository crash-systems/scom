#ifndef COMMON_H
    #define COMMON_H
    #include <termio.h>

    #define VERSION "dev"
    #define BIN_NAME "scom"
    #define OPT_FLAGS "hb:s:"

typedef struct {
    struct termios saved_term_flags;
    char *src;
    int src_fd;
    speed_t baudrate;
} scom_ctx;

#endif /* COMMON_H */
