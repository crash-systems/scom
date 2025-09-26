/*
    scom, Minimal serial communication tool
    Copyright (C) 2025  crash-systems

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
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

    ctx->saved_term_flags = tty;

    cfsetospeed(&tty, ctx->baudrate);
    cfsetispeed(&tty, ctx->baudrate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~(IGNBRK | ICRNL | INLCR | IGNCR | IXON | IXOFF | IXANY);
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 1;
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD | CSTOPB | CRTSCTS);

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

bool run_serial_io(int fd)
{
    struct termios orig_tty;
    struct termios raw_tty;
    char buff[256];
    fd_set fds;
    int maxfd;

    if (tcgetattr(STDIN_FILENO, &orig_tty) == -1) {
        perror(BIN_NAME);
        return false;
    }

    raw_tty = orig_tty;
    raw_tty.c_lflag &= ~(ICANON | ECHO);
    raw_tty.c_cc[VMIN] = 1;
    raw_tty.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_tty) == -1) {
        perror(BIN_NAME);
        return false;
    }

    maxfd = fd > STDIN_FILENO ? fd : STDIN_FILENO;

    signal(SIGINT, SIG_IGN);

    for (;;) {
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        FD_SET(fd, &fds);

        if (select(maxfd + 1, &fds, NULL, NULL, NULL) < 0 && errno != EINTR) {
            perror(BIN_NAME);
            break;
        }

        if (FD_ISSET(fd, &fds)) {
            ssize_t n = read(fd, buff, sizeof(buff));
            if (n <= 0) break;
            write(STDOUT_FILENO, buff, n);
        }

        if (FD_ISSET(STDIN_FILENO, &fds)) {
            ssize_t n = read(STDIN_FILENO, buff, sizeof(buff));
            if (n <= 0) break;
            if (*buff == CTRL_D_OCT_SEQ) break;
            write(fd, buff, n);
        }
    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_tty);
    return true;
}

