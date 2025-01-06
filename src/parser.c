#include "ft_ping.h"

void ping_option_check(p_cmd **ping_cmd, const char *arg, const char *value)
{
    size_t count;

    if (strlen(arg) != 2 || arg[1] == '-')
        error_exit("ft_ping: invalid Option");
    else
    {
        switch (arg[1])
        {
        case 'h':
            (*ping_cmd)->options[HELP] = 1;
            break;
        case 'V':
            (*ping_cmd)->options[VERSION] = 1;
            break;
        case 'v':
            (*ping_cmd)->options[VERBOSE] = 1;
            break;
        case 'n':
            (*ping_cmd)->options[NUMERIC_ONLY] = 1;
            break;
        case 'c':
            (*ping_cmd)->options[COUNT] = get_option_value(value);
            break;
        case 'w':
            (*ping_cmd)->options[DEADLINE] = get_option_value(value);
            break;
        case 's':
            (*ping_cmd)->options[SEND_BUFF] = get_option_value(value);
            break;
        default:
            error_exit("ft_ping: Invalid Option");
        }
    }
}

void ping_destination_check(p_cmd **ping_command, const char *arg, dest_sockaddr *dest_addr)
{
    struct sockaddr_in *sa;
    // char host[1024];

    // memset(host, 0, 1024);

    (*ping_command)->destination = (char *)arg;
    (*ping_command)->dest_sockaddr = get_sock_addr(arg);
    sa = (struct sockaddr_in *)dest_addr->dest_addr;
    inet_ntop(sa->sin_family, &sa->sin_addr, (*ping_command)->network_repr, INET_ADDRSTRLEN);
    // sa->sin_addr.s_addr = inet_addr((*ping_command)->network_repr); // Convert string to binary representation
    // Perform reverse DNS resolution
    // int result = getnameinfo((*ping_command)->dest_sockaddr.dest_addr, (*ping_command)->dest_sockaddr.addr_len, host, sizeof(host), NULL, 0, 1024);
    // printf("HOST: %s\n",host);
}

p_cmd *ping_parser(int arg_num, const char **args)
{
    p_cmd *ping_command;

    ping_command = malloc(sizeof(p_cmd));
    if (!ping_command)
        error_exit("Malloc Ping Command: Memory Allocation Error");
    ping_command->destination = NULL;
    memset(ping_command->options, -1, sizeof(int) * OPTIONS);

    for (size_t i = 0; i < arg_num; i++)
    {
        if (args[i][0] == '-')
        {
            if (i + 1 < arg_num)
                ping_option_check(&ping_command, args[i], args[i + 1]);
            else
                ping_option_check(&ping_command, args[i], NULL);
        }
        else
            ping_destination_check(&ping_command, args[i], &ping_command->dest_sockaddr);
    }
    if (!ping_command->destination)
        error_exit("Destination address required");
    return ping_command;
}

dest_sockaddr get_sock_addr(const char *host_addrr)
{
    struct addrinfo *result;
    struct addrinfo hints;
    struct addrinfo *rp;
    struct sockaddr *destination_sockaddr;
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
            dest_address.dest_addr = destination_sockaddr;
            dest_address.addr_len = rp->ai_addrlen;
            return dest_address;
        }
    }
    freeaddrinfo(result);
    error_exit("get addr info address not found");
}