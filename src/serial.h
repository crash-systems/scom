#ifndef SERIAL_H
    #define SERIAL_H
    #include <stdbool.h>

    #include "common.h"

// supported baudrade
#define BAUDRATE_MAP(_) \
    _(50, B50) \
    _(75, B75) \
    _(110, B110) \
    _(150, B150) \
    _(300, B300) \
    _(1200, B1200) \
    _(2400, B2400) \
    _(4800, B4800) \
    _(9600, B9600) \
    _(19200, B19200) \
    _(38400, B38400) \
    _(57600, B57600) \
    _(115200, B115200) \
    _(230400, B230400) \
    _(460800, B460800) \
    _(500000, B500000) \
    _(921600, B921600) \
    _(1000000, B1000000) \
    _(1152000, B1152000)

int open_serial(scom_ctx *ctx);
bool run_serial_io(int fd);
speed_t baud_from_int(int baudrate);
#endif /* SERIAL_H */
