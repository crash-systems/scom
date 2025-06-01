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

#ifndef COMMON_H
    #define COMMON_H
    #include <termio.h>

    #define VERSION "1.1"
    #define BIN_NAME "scom"
    #define OPT_FLAGS "hb:s:"

typedef struct {
    struct termios saved_term_flags;
    char *src;
    int src_fd;
    speed_t baudrate;
} scom_ctx;

#endif /* COMMON_H */
