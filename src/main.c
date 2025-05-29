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
    scom_ctx ctx = {{0}, NULL, 0};

    if (!parse_args(ac, av, &ctx))
        return EXIT_FAILURE;
    ctx.src_fd = open_serial(&ctx);
    if (ctx.src_fd < 0)
        return EXIT_FAILURE;
    run_serial_io(ctx.src_fd);
    return EXIT_SUCCESS;
}
