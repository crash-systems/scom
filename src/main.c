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
    puts(BIN_NAME " " VERSION " a minimal serial communication tool");
    puts("Usage: " BIN_NAME " -s DEVICE [-b BAUDRATE] [-h]");
    puts("");
    puts("Options:");
    puts("  -s DEVICE       Specify the serial device (e.g. /dev/ttyUSB0)");
    puts("  -b BAUDRATE     Set the baudrate (default: 9600)");
    puts("                  Available values:");
    puts("                    50, 75, 110, 150, 300, 1200, 2400, 4800,");
    puts("                    9600, 19200, 38400, 57600, 115200, 230400,");
    puts("                    460800, 500000, 921600, 1000000, 1115200");
    puts("  -h              Show this help message");
    puts("");
    puts("Project page:");
    puts("  <https://github.com/crash-systems/scom>");
    puts("");
    puts("License:");
    puts("  Copyright (C) 2025 crash-system");
    puts("  This program comes with ABSOLUTELY NO WARRANTY.");
    puts("  This is free software, and you are welcome to redistribute it");
    puts("  under certain conditions; for details, see:");
    puts("  <https://github.com/crash-systems/scom/blob/main/LICENSE>");
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
    if (ac < 2 || ac > 5) {
        print_usage();
        return false;
    }

    for (int opt = getopt(ac, av, OPT_FLAGS); opt != -1; opt = getopt(ac, av, OPT_FLAGS)) {
        switch(opt) {
            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);
            case 's':
                ctx->src = optarg;
                break;
            case 'b':
                ctx->baudrate = baud_from_int(atoi(optarg));
                if (!ctx->baudrate) {
                    fprintf(stderr, "Error: '%s' is not a valid baudrate.\n", optarg);
                    fprintf(stderr, "Valid baudrates are:\n");
                    fprintf(stderr,
                        "  50, 75, 110, 150, 300, 1200, 2400, 4800,\n"
                        "  9600, 19200, 38400, 57600, 115200, 230400,\n"
                        "  460800, 500000, 921600, 1000000, 1115200\n");
                    return false;
                }
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
