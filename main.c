#include "./src/ft_ping.h"

p_packet * ping_request;

int main(int ac, const char *av[])
{
    p_cmd *ping_cmd;
    int ping_sock;

    if (ac - 1 < 1)
        error_exit("Destination address required");
    ping_cmd = ping_parser(ac - 1, &av[1]);
    ping_request = (p_packet *)malloc(sizeof(p_packet));
    if (!ping_request)
        error_exit("Malloc: Memory Allocation Error");
    ping_request->address = ping_cmd->addr;
    ping_request->id = getpid();
    ping_request->sequence = 1;
    // socket creation
    // send request
    // ping infinite loop handler
    printf("ping request id: %d\n", ping_request->id);
    printf("ping request sequence: %d\n", ping_request->sequence);
    ping_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (ping_sock == -1)
        error_exit("Socket: Socket creation Error");
    signal(SIGALRM, ping_send_handler);
    alarm(1);
    ping_loop();
    // print_ping_command(ping_cmd);
    return (0);
}