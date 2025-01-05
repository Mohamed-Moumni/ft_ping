#ifndef FT_PING_H
#define FT_PING_H

#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <ctype.h>

#define BUFFER 65536
#define REQ_BUFF 1024

#define HELP 0
#define VERSION 1
#define VERBOSE 2
#define NUMERIC_ONLY 3
#define COUNT 4
#define DEADLINE 5
#define SEND_BUFF 6

/* Ping Structrues*/
typedef struct ping_opt
{
    short option;
    bool has_value;
    size_t value;
    struct ping_opt *next;
} ping_opt;

typedef struct ft_icmp
{
    struct icmphdr icmp_header;
    void *data;
} icmp_h;

typedef struct destionation_sockaddr
{
    struct sockaddr *dest_addr;
    socklen_t addr_len;
} dest_sockaddr;

typedef struct ping_cmd
{
    char            *destination;
    ping_opt        *options;
    dest_sockaddr   dest_sockaddr;
} p_cmd;

typedef struct ping_packet
{
    p_cmd *ping_command;
    uint16_t id;
    uint16_t sequence;
    int socket;
    // start time
    // packets sent
    // packet recieved
    // recieved bytes
} p_packet;

extern p_packet *ping_request;

/*   Utils Functions */
void ping_help(void);
void error_exit(const char *error_msg);
ping_opt *create_option_list(const short option, bool has_value, ...);
ping_opt *last_option(ping_opt *ping_opt_list);
void add_option_list(ping_opt **ping_opt_list, ping_opt *ping_opt);
void print_ping_command(p_cmd *ping_command);
dest_sockaddr get_sock_addr(const char *host_addrr);
size_t get_option_value(const char *value);

/* Ping Parser Functions*/
p_cmd *ping_parser(int arg_num, const char **args);
void ping_option_check(ping_opt **options, const char *arg, const char *value);
void ping_destination_check(char **destination, const char *arg, dest_sockaddr *dest_addr);

/* Ping Functions */
void setup_ping_socket(void);
struct icmphdr prep_echo_request(uint16_t pid, uint16_t seq);
uint16_t calcualte_checksum(unsigned char *data);
void send_request(void);
void ping_loop(void);
void ping_send_handler(int signal);
void ping_send_echo(void);
void ping_init(p_cmd *ping_command);
void socket_init(void);
void ping_echo_replay(void);

#endif