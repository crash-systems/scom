#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#include "common.h"
#include "serial.h"

int open_serial(scom_ctx *ctx)
{
    int fd = open(ctx->src, O_RDWR | O_NOCTTY | O_SYNC);
    struct termios tty;

    if (fd < 0) {
        perror(BIN_NAME);
        return -1;
    }
    if (tcgetattr(fd, &tty) != 0) {
        perror(BIN_NAME);
        close(fd);
        return -1;
    }

    // save actual term flags
    ctx->saved_term_flags = tty;

    // set baudrate
    cfsetospeed(&tty, ctx->baudrate);
    cfsetispeed(&tty, ctx->baudrate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 1;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror(BIN_NAME);
        close(fd);
        return -1;
    }

    return fd;
}

speed_t baud_from_int(int baudrate)
{
#define MATCH_BAUD(b, s) if (baudrate == (b)) return s;
    BAUDRATE_MAP(MATCH_BAUD)
#undef MATCH_BAUD
    return (speed_t)0;
}

void run_serial_io(int fd)
{
    char buff[256];
    fd_set fds;
    int maxfd = fd > STDIN_FILENO ? fd : STDIN_FILENO;

    while (true) {
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        FD_SET(fd, &fds);

        if (select(maxfd + 1, &fds, NULL, NULL, NULL) < 0 && errno != EINTR) {
            perror(BIN_NAME);
            break;
        }

        if (FD_ISSET(fd, &fds)) {
            ssize_t n = read(fd, buff, sizeof(buff));
            if (n > 0)
                write(STDOUT_FILENO, buff, n);
        }

        if (FD_ISSET(STDIN_FILENO, &fds)) {
            ssize_t n = read(STDIN_FILENO, buff, sizeof(buff));
            if (n > 0)
                write(fd, buff, n);
        }
    }
}
