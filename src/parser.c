#include "ft_ping.h"

void ping_option_check(ping_opt **options, const char *arg, const char *value)
{
    size_t count;

    if (strlen(arg) != 2 || arg[1] == '-')
        error_exit("ft_ping: invalid Option");
    else
    {
        switch (arg[1])
        {
        case 'h':
            add_option_list(options, create_option_list(HELP, false));
            break;
        case 'V':
            add_option_list(options, create_option_list(VERSION, false));
            break;
        case 'v':
            add_option_list(options, create_option_list(VERBOSE, false));
            break;
        case 'n':
            add_option_list(options, create_option_list(NUMERIC_ONLY, false));
            break;
        case 'c':
            add_option_list(options, create_option_list(COUNT, true, get_option_value(value)));
            break;
        case 'w':
            add_option_list(options, create_option_list(DEADLINE, true, get_option_value(value)));
            break;
        case 's':
            add_option_list(options, create_option_list(SEND_BUFF, true, get_option_value(value)));
            break;
        default:
            error_exit("ft_ping: Invalid Option");
        }
    }
}

void ping_destination_check(char **destination, const char *arg, dest_sockaddr *dest_addr)
{
    // printf("ping_destination_check\n");
    (*destination) = (char *)arg;
    (*dest_addr) = get_sock_addr(arg);
    // printf("finish ping_destination_check\n");
}

p_cmd *ping_parser(int arg_num, const char **args)
{
    // printf("Start Parser\n");
    p_cmd *ping_command;

    ping_command = malloc(sizeof(p_cmd));
    if (!ping_command)
        error_exit("Malloc Ping Command: Memory Allocation Error");
    ping_command->options = NULL;
    ping_command->destination = NULL;

    for (size_t i = 0; i < arg_num; i++)
    {
        if (args[i][0] == '-')
        {
            if (i + 1 < arg_num)
                ping_option_check(&ping_command->options, args[i], args[i + 1]);
            else
                ping_option_check(&ping_command->options, args[i], NULL);
        }
        else
            ping_destination_check(&ping_command->destination, args[i], &ping_command->dest_sockaddr);
    }
    if (!ping_command->destination)
        error_exit("Destination address required");
    // printf("Finish Parser\n");
    return ping_command;
}

dest_sockaddr get_sock_addr(const char *host_addrr)
{
    // printf("start getsokc_addr\n");
    struct addrinfo *result;
    struct addrinfo hints;
    struct addrinfo *rp;
    struct sockaddr *destination_sockaddr;
    struct sockaddr_in *socka_in;
    dest_sockaddr dest_address;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = 0;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_protocol = 0;

    int s = getaddrinfo(host_addrr, NULL, &hints, &result);
    destination_sockaddr = NULL;
    if (s != 0)
        error_exit(gai_strerror(s));
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        if (rp->ai_family == AF_INET && rp->ai_addr)
        {
            destination_sockaddr = (struct sockaddr *)malloc(sizeof(struct sockaddr));
            destination_sockaddr = memcpy(destination_sockaddr, rp->ai_addr, sizeof(rp->ai_addr));
            socka_in = (struct sockaddr_in *)destination_sockaddr;
            dest_address.dest_addr = destination_sockaddr;
            dest_address.addr_len = rp->ai_addrlen;
            // printf("finish getsokc_addr\n");
            return dest_address;
        }
    }
    freeaddrinfo(result);
    error_exit("get addr info address not found");
}