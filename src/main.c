#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

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
bool parse_args(int ac, char **av)
{
    for (int opt = getopt(ac, av, OPT_FLAGS); opt != -1; opt = getopt(ac, av, OPT_FLAGS)) {
        switch(opt) {
            case 'h':
                print_usage();
                return true;
            case 's':
                break;
            case 'b':
                break;
            default:
                print_usage();
                return false;
        }
    }
    print_usage();
    return false;
}

int main(int ac, char **av)
{
    if (!parse_args(ac, av))
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
