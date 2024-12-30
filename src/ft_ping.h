#pragma once

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip_icmp.h>

#define HELP 0
#define VERSION 1
#define VERBOSE 2

typedef struct ping_opt
{
    short option;
    struct ping_opt *next;
} ping_opt;

typedef struct ping_cmd
{
    char *destination;
    ping_opt *options;
} p_cmd;

/*   Utils Functions */

void ping_help(void);
void error_exit(const char *error_msg);
ping_opt *create_option_list(const short option);
ping_opt *last_option(ping_opt *ping_opt_list);
void add_option_list(ping_opt **ping_opt_list, ping_opt *ping_opt);
void print_ping_command(p_cmd *ping_command);

/* Ping Parser Functions*/
p_cmd *ping_parser(int arg_num, const char **args);
void ping_option_check(ping_opt **options, const char *arg);
void ping_destination_check(char **destination, const char *arg);