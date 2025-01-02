#include "ft_ping.h"

// void setup_ping_socket(void)
// {
//     int sockfd;

//     sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
//     if (sockfd == -1)
//         error_exit("Socket: Socket Creation Error.");
    
// }

// struct icmphdr prep_echo_request(uint16_t pid, uint16_t seq)
// {
//     // prepare header

//     struct icmphdr icmp_header;

//     icmp_header.type = ICMP_ECHO;
//     icmp_header.code = 0;
//     icmp_header.un.echo.id = pid;
//     icmp_header.un.echo.sequence = seq;

//     return icmp_header;
// }

// uint16_t calcualte_checksum(void)
// {
//     struct iphdr *ip_header;
// }

void ping_loop(void)
{
    while (1)
    {
        sleep(10);
    }
}

void ping_send_handler(int signal)
{
    printf("Sender: %p\n", (p_packet *)ping_request);
    alarm(1);
}