#include "ft_ping.h"

uint16_t calculate_checksum(uint16_t *data, size_t data_len)
{
    uint16_t checksum;
    uint16_t odd_byte;
    uint32_t sum;

    sum = 0;
    odd_byte = 0;
    while (data_len > 1)
    {
        sum += *(data);
        data++;
        data_len -= 2;
    }

    if (!data_len)
    {
        *(unsigned char *)&odd_byte = *(unsigned char *)data;
        sum += odd_byte;
    }

    sum += (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return sum;
}

void ping_loop(void)
{
    while (1)
    {
        sleep(10);
    }
}

void ping_send_handler(int signal)
{
    ping_send_echo();
    alarm(1);
}

void ping_echo_replay(void)
{
    printf("Ping Echo Replay\n");
    
}

void ping_send_echo(void)
{
    printf("Ping Send Echo Call\n");
    icmp_h *buffer_send;
    size_t data_len;
    struct sockaddr *dest_addr = NULL;
    socklen_t des_addr_len;
    struct addrinfo *rp;

    data_len = 0;
    buffer_send = (icmp_h *)malloc(sizeof(icmp_h) + 3);
    if (!buffer_send)
        error_exit("Buffer Send error");
    buffer_send->icmp_header = (struct icmphdr *)malloc(sizeof(struct icmphdr));
    if (!buffer_send->icmp_header)
        error_exit("Icmp Header Error");

    buffer_send->icmp_header->code = 0;
    buffer_send->icmp_header->type = 8;
    buffer_send->icmp_header->un.echo.id = ping_request->id;
    buffer_send->icmp_header->un.echo.sequence = ping_request->sequence++;
    buffer_send->icmp_header->checksum = 0;
    data_len += sizeof(buffer_send->icmp_header);
    buffer_send->data = (char *)malloc(sizeof(char) * 2 + 1);
    if (!buffer_send->data)
        error_exit("Malloc: Memory Allocation Error");
    memcpy(buffer_send->data, "HI", 2);
    data_len += 3;
    buffer_send->icmp_header->checksum = calculate_checksum((uint16_t *)buffer_send, data_len);
    for (rp = ping_request->address; rp != NULL; rp = rp->ai_next)
    {
        if (rp->ai_family == AF_INET)
        {
            dest_addr = rp->ai_addr;
            des_addr_len = rp->ai_addrlen;
            break;
        }
    }
    int send = sendto(ping_request->socket, buffer_send, data_len, 0, dest_addr, des_addr_len);
    if (send < 0)
    {
        perror("Send To Error: ");
    }
}

void ping_init(p_cmd * ping_command)
{
    ping_request = (p_packet *)malloc(sizeof(p_packet));
    if (!ping_request)
        error_exit("Malloc: Memory Allocation Error");
    ping_request->address = ping_command->addr;
    ping_request->id = getpid();
    ping_request->sequence = 1;
}

void socket_init(void)
{
    int ping_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping_sock == -1)
        error_exit("Socket: Socket creation Error");
    ping_request->socket = ping_sock;
    signal(SIGALRM, ping_send_handler);
    alarm(1);
}