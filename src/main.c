#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "serial.h"

static
void print_usage(void)
{
    puts(BIN_NAME ", version " VERSION);
    puts("Usage: " BIN_NAME " SOURCE eg. /dev/USBtty0 [BAUDRATE default 9600]");
    puts("scom home page: <https://github.com/crash-systems/scom>");
}

/*
 * Parse args
 * Available options:
 *  -h print help message
 *  -s specify the source
 *  -b specify the baudrate
 */
static
bool parse_args(int ac, char **av, scom_ctx *ctx)
{
    for (int opt = getopt(ac, av, OPT_FLAGS); opt != -1; opt = getopt(ac, av, OPT_FLAGS)) {
        switch(opt) {
            case 'h':
                print_usage();
                return true;
            case 's':
                ctx->src = optarg;
                break;
            case 'b':
                ctx->baudrate = baud_from_int(atoi(optarg));
                if (!ctx->baudrate)
                    return fputs("Baudrate is not valid.\nValid baudrate are:"
                        " 50, 75, 110, 150, 300, 1200, 2400, 4800, 9600, 19200,"
                        " 38400, 57600, 115200, 230400, 460800, 500000, 921600,"
                        " 1000000, 1115200\n", stderr), false;
                break;
            default:
                print_usage();
                return false;
        }
    }
    return true;
}

int main(int ac, char **av)
{
    scom_ctx ctx = {{0}, NULL, 0, B9600};

    if (!parse_args(ac, av, &ctx))
        return EXIT_FAILURE;
    ctx.src_fd = open_serial(&ctx);
    if (ctx.src_fd < 0)
        return EXIT_FAILURE;
    return run_serial_io(ctx.src_fd);
}
