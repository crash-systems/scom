#ifndef SERIAL_H
    #define SERIAL_H
    #include "common.h"

int open_serial(scom_ctx *ctx);
void run_serial_io(int fd);
#endif /* SERIAL_H */
