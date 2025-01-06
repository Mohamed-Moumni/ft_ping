#include "./src/ft_ping.h"

p_packet * ping_request;

int main(int ac, const char *av[])
{
    p_cmd *ping_cmd;
    int ping_sock;

    if (ac - 1 < 1)
        error_exit("Destination address required");

    ping_cmd = ping_parser(ac - 1, &av[1]);
    ping_init(ping_cmd);
    signal(SIGALRM, ping_send_handler);
    signal(SIGINT, ping_exit_hanlder);
    socket_init();
    ping_loop();
    return (0);
}