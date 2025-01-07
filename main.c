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
    for (int i = 0; i < 7;i++)
    {
        printf("%d\n", ping_request->ping_command->options[i]);
    }
    printf("Counter: %ld\n", ping_request->ping_counter);
    signal(SIGALRM, ping_send_handler);
    signal(SIGINT, ping_exit_handler);
    socket_init();
    if (gettimeofday(&ping_request->start_date, NULL) != 0)
        error_exit("Gettime of Day Error");
    ping_loop();
    return (0);
}