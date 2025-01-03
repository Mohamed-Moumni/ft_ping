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
    socket_init();
    ping_loop();
    // socket creation
    // send request
    // ping infinite loop handler
    // printf("ping request id: %d\n", ping_request->id);
    // printf("ping request sequence: %d\n", ping_request->sequence);
    // print_ping_command(ping_cmd);
    return (0);
}