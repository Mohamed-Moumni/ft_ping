#ifndef FT_PING_H
#define FT_PING_H

#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>

#define HELP 0
#define VERSION 1
#define VERBOSE 2

/* Ping Structrues*/
typedef struct ping_opt
{
    short option;
    struct ping_opt *next;
} ping_opt;

typedef struct ping_cmd
{
    char *destination;
    ping_opt *options;
    struct addrinfo *addr;
} p_cmd;

typedef struct ping_packet
{
    struct addrinfo *address;
    uint16_t id;
    uint16_t sequence;
} p_packet;

extern p_packet *ping_request;

/*   Utils Functions */
void ping_help(void);
void error_exit(const char *error_msg);
ping_opt *create_option_list(const short option);
ping_opt *last_option(ping_opt *ping_opt_list);
void add_option_list(ping_opt **ping_opt_list, ping_opt *ping_opt);
void print_ping_command(p_cmd *ping_command);
struct addrinfo *get_addr_info(const char *host_addrr);

/* Ping Parser Functions*/
p_cmd *ping_parser(int arg_num, const char **args);
void ping_option_check(ping_opt **options, const char *arg);
void ping_destination_check(char **destination, const char *arg, struct addrinfo **result);

/* Ping Functions */
void setup_ping_socket(void);
struct icmphdr prep_echo_request(uint16_t pid, uint16_t seq);
uint16_t calcualte_checksum(void);
void send_request(void);
void ping_loop(void);
void ping_send_handler(int signal);

#endif