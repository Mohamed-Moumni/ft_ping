#include "ft_ping.h"

void ping_option_check(ping_opt **options, const char *arg)
{
    if (strlen(arg) != 2 || arg[1] == '-')
        error_exit("ft_ping: invalid Option");
    else
    {
        switch (arg[1])
        {
        case 'h':
            add_option_list(options, create_option_list(HELP));
            break;
        case 'V':
            add_option_list(options, create_option_list(VERSION));
            break;
        case 'v':
            add_option_list(options, create_option_list(VERBOSE));
            break;
        default:
            error_exit("ft_ping: Invalid Option");
        }
    }
}

void ping_destination_check(char **destination, const char *arg, struct addrinfo **result)
{
    (*destination) = (char *)arg;
    (*result) = get_addr_info((*destination));
}

p_cmd *ping_parser(int arg_num, const char **args)
{
    p_cmd *ping_command;

    ping_command = malloc(sizeof(p_cmd));
    if (!ping_command)
        error_exit("Malloc: Memory Allocation Error");
    ping_command->options = NULL;
    ping_command->destination = NULL;
    ping_command->addr = NULL;

    for (size_t i = 0; i < arg_num; i++)
    {
        if (args[i][0] == '-')
            ping_option_check(&ping_command->options, args[i]);
        else
            ping_destination_check(&ping_command->destination, args[i], &ping_command->addr);
    }
    if (!ping_command->destination)
        error_exit("Destination address required");
    return ping_command;
}

struct addrinfo *get_addr_info(const char *host_addrr)
{
    struct addrinfo *result;
    struct addrinfo hints;
    struct addrinfo *rp;
    char address[128];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = 0;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_protocol = 0;

    int s = getaddrinfo(host_addrr, NULL, &hints, &result);
    if (s != 0)
        error_exit(gai_strerror(s));
    // for (rp = result; rp != NULL; rp = rp->ai_next)
    // {
    //     void *addr;
    //     printf("ai_family: %d\n", rp->ai_family);
    //     printf("ai_socket: %d\n", rp->ai_socktype);
    //     printf("ai_flags: %d\n", rp->ai_flags);
    //     printf("ai_protocol: %d\n", rp->ai_protocol);
    //     if (rp->ai_family == AF_INET)
    //     {
    //         struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
    //         addr = &(ipv4->sin_addr);
    //     }
    //     inet_ntop(rp->ai_family, addr, address);
    //     printf("address: %s\n", address);
    //     printf("---------------------------------------\n");
    // }
    return result;
}