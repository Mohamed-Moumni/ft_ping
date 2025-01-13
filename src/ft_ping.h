#ifndef FT_PING_H
#define FT_PING_H

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <limits.h>

#define BUFFER 1024
#define REQ_BUFF 1024
#define PAYLOAD_BUFF 56
#define OPTIONS 7
#define NI_MAXHOST 1025

#define HELP 0
#define VERSION 1
#define VERBOSE 2
#define NUMERIC_ONLY 3
#define COUNT 4
#define DEADLINE 5
#define SEND_BUFF 6
#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x > y ? y : x)

/* Ping Structrues*/

typedef struct ft_icmp
{
    struct icmphdr icmp_header;
    unsigned char data[56];
} icmp_h;

typedef struct destionation_sockaddr
{
    struct sockaddr *dest_addr;
    socklen_t addr_len;
} dest_sockaddr;

typedef struct ping_cmd
{
    char *destination;
    char network_repr[INET_ADDRSTRLEN];
    char reverse_dns[NI_MAXHOST];
    int options[OPTIONS];
    dest_sockaddr dest_sockaddr;
} p_cmd;

typedef struct median_deviation
{
    double rtt;
    struct median_deviation *next;
} mdev;

typedef struct ping_packet
{
    struct timeval start_date;
    p_cmd *ping_command;
    uint16_t id;
    uint16_t sequence;
    uint16_t packet_sent;
    uint16_t packet_received;
    uint16_t bytes_sent;
    uint16_t bytes_received;
    int ping_counter;
    uint16_t ip_header_sent;
    int socket;
    bool rtt;
    double total_time;
    double min_time;
    double max_time;
    double avg_time;
    mdev *mdev;
} p_packet;

extern p_packet *ping_request;

/*   Utils Functions */
void ping_help(void);
void error_exit(const char *error_msg);
dest_sockaddr get_sock_addr(const char *host_addrr);
int get_option_value(const char *value);
void show_version(void);
mdev *create_mdev_list(const double mdev);
mdev *last_mdev(mdev *ping_mdev_list);
void add_mdev_list(mdev **ping_mdev_list, mdev *new_mdev);

/* Ping Parser Functions*/
p_cmd *ping_parser(int arg_num, const char **args);
void ping_option_check(p_cmd **ping_cmd, const char *arg, const char *value, int *i);
void ping_destination_check(p_cmd **ping_command, const char *arg, dest_sockaddr *dest_addr);

/* Ping Functions */
void setup_ping_socket(void);
struct icmphdr prep_echo_request(uint16_t pid, uint16_t seq);
uint16_t calcualte_checksum(unsigned char *data);
void send_request(void);
void ping_loop(void);
void ping_send_handler(int signal);
void ping_exit_handler(int signal);
void ping_send_echo(void);
void ping_init(p_cmd *ping_command);
void socket_init(void);
void ping_echo_replay(void);
double calculate_rtt(struct timeval *sending_time);
double calculate_mdev(void);

/* Ping Output Functions */
void print_ping_start(void);
void print_ping_packet(const int seq, const int ttl, const double rtt);
void print_ping_stats(void);

#endif