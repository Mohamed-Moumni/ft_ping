#include "./src/ft_ping.h"

int main(int ac, const char *av[])
{
    p_cmd * ping_cmd;

    if (ac - 1 < 1)
        error_exit("Destination address required");
    ping_cmd = ping_parser(ac - 1, &av[1]);
    print_ping_command(ping_cmd);
    return (0);
}