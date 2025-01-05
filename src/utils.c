#include "ft_ping.h"

void ping_help(void)
{
    printf("Usage:\n\n");
    printf("ft_ping [options] <destination>\n\n");
    printf("Options:\n\n");
    printf("<destination>            dns name or ip address\n");
    printf("-h                       print help and exit\n");
}

void error_exit(const char *error_msg)
{
    printf("ft_ping: %s\n", error_msg);
    ping_help();
    exit(1);
}

ping_opt *create_option_list(const short option, bool has_value, ...)
{
    va_list args;
    va_start(args, has_value);
    ping_opt *ping_options = (ping_opt *)malloc(sizeof(ping_opt));
    if (!ping_options)
        error_exit("Malloc: Memory Allocation Error");
    ping_options->option = option;
    if (has_value)
        ping_options->value = va_arg(args, size_t);
    ping_options->next = NULL;
    va_end(args);
    return ping_options;
}

ping_opt *last_option(ping_opt *ping_opt_list)
{
    ping_opt *ping_temp = ping_opt_list;

    while (ping_temp->next != NULL)
    {
        ping_temp = ping_temp->next;
    }
    return ping_temp;
}

void add_option_list(ping_opt **ping_opt_list, ping_opt *new_option)
{
    ping_opt *last_option_list;

    if (*ping_opt_list == NULL)
        (*ping_opt_list) = new_option;
    else
    {
        last_option_list = last_option(*ping_opt_list);
        last_option_list->next = new_option;
    }
}

void print_ping_command(p_cmd *ping_command)
{
    ping_opt *options = ping_command->options;
    printf("Destination: %s\n", ping_command->destination);
    printf("Options: \n");

    while (options != NULL)
    {
        printf("Option: %d\n", options->option);
        options = options->next;
    }
}

size_t get_option_value(const char *value)
{
    if (!value)
        error_exit("option requires an argument --");
    else
    {
        for (size_t i = 0; i < strlen(value); i++)
        {
            if (!isdigit(value[i]))
                error_exit("Invalid Option value");
        }
    }
    return atoi(value);
}