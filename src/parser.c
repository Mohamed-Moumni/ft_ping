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

void ping_destination_check(char **destination, const char *arg)
{
    (*destination) = (char *)arg;
}

p_cmd *ping_parser(int arg_num, const char **args)
{
    p_cmd *ping_command;

    ping_command = malloc(sizeof(p_cmd));
    if (!ping_command)
        error_exit("Malloc: Memory Allocation Error");
    ping_command->options = NULL;
    ping_command->destination = NULL;

    for (size_t i = 0; i < arg_num; i++)
    {
        if (args[i][0] == '-')
            ping_option_check(&ping_command->options, args[i]);
        else
            ping_destination_check(&ping_command->destination, args[i]);
    }
    if (!ping_command->destination)
        error_exit("Destination address required");
    return ping_command;
}