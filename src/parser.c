#include "ft_ping.h"

void ping_option_check(p_cmd **ping_cmd, const char *arg, const char *value, int * i)
{
    if (strlen(arg) != 2 || arg[1] == '-')
        error_exit("ft_ping: invalid Option");
    else
    {
        switch (arg[1])
        {
        case 'h':
            (*ping_cmd)->options[HELP] = 1;
            ping_help();
            (*i)++;
            break;
        case 'V':
            (*ping_cmd)->options[VERSION] = 1;
            show_version();
            (*i)++;
            break;
        case 'v':
            (*ping_cmd)->options[VERBOSE] = 1;
            (*i)++;
            break;
        case 'n':
            (*ping_cmd)->options[NUMERIC_ONLY] = 1;
            (*i)++;
            break;
        case 'c':
            (*ping_cmd)->options[COUNT] = get_option_value(value);
            (*i) += 2;
            break;
        case 'w':
            (*ping_cmd)->options[DEADLINE] = get_option_value(value);
            (*i) += 2;
            break;
        case 's':
            (*ping_cmd)->options[SEND_BUFF] = get_option_value(value);
            (*i) += 2;
            break;
        default:
            error_exit("ft_ping: Invalid Option");
        }
    }
}

void ping_destination_check(p_cmd **ping_command, const char *arg, dest_sockaddr *dest_addr)
{
    struct sockaddr_in  *sa;
    int                 result;

    (*ping_command)->destination = (char *)arg;
    (*ping_command)->dest_sockaddr = get_sock_addr(arg);

    sa = (struct sockaddr_in *)dest_addr->dest_addr;
    inet_ntop(sa->sin_family, &sa->sin_addr, (*ping_command)->network_repr, INET_ADDRSTRLEN);
    // doing dns resolution if the -n option is not enabled
    if ((*ping_command)->options[NUMERIC_ONLY] == -1)
    {
        result = getnameinfo((struct sockaddr *)sa, sizeof(struct sockaddr_in), (*ping_command)->reverse_dns, NI_MAXHOST, NULL, 0, 0);
        if (result != 0)
            error_exit("Get Name Info Error");
    }
}

p_cmd *ping_parser(int arg_num, const char **args)
{
    p_cmd       *ping_command;
    int         i;

    ping_command = malloc(sizeof(p_cmd));
    if (!ping_command)
        error_exit("Malloc Ping Command: Memory Allocation Error");
    ping_command->destination = NULL;
    memset(ping_command->options, -1, sizeof(int) * OPTIONS);
    memset(ping_command->reverse_dns, 0, sizeof(char) * NI_MAXHOST);

    i = 0;
    while (i < arg_num)
    {
        if (args[i][0] == '-')
        {
            if (i + 1 < arg_num)
                ping_option_check(&ping_command, args[i], args[i + 1], &i);
            else
                ping_option_check(&ping_command, args[i], NULL, &i);
        }
        else
        {
            ping_destination_check(&ping_command, args[i], &ping_command->dest_sockaddr);
            i++;
        }
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
    dest_sockaddr   dest_address;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = 0;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_protocol = 0;

    int s = getaddrinfo(host_addrr, NULL, &hints, &result);
    if (s != 0)
        error_exit(gai_strerror(s));
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        if (rp->ai_family == AF_INET && rp->ai_addr)
        {
            dest_address.dest_addr = (struct sockaddr *)malloc(sizeof(struct sockaddr));
            if (!dest_address.dest_addr)
                error_exit("Malloc: Memory Allocation Error");
            memcpy(dest_address.dest_addr, rp->ai_addr, sizeof(struct sockadd));
            dest_address.addr_len = rp->ai_addrlen;
            freeaddrinfo(result);
            return dest_address;
        }
    }
    freeaddrinfo(result);
    error_exit("get addr info address not found");
}