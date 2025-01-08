#include "ft_ping.h"

unsigned short calculate_checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;

    for (; len > 1; len -= 2)
        sum += *buf++;

    if (len == 1)
        sum += *(unsigned char *)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    return ~sum;
}

void ping_loop(void)
{
    while (ping_request->ping_counter > 0 || ping_request->ping_counter == -1)
    {
        ping_echo_replay();
        usleep(1000);
        if (ping_request->ping_counter > 0)
            ping_request->ping_counter--;
    }
    print_ping_stats();
}

void ping_send_handler(int signal)
{
    ping_send_echo();
    alarm(1);
}

void ping_echo_replay(void)
{
    struct iphdr    *ip_header;
    char            recv_buff[BUFFER];
    struct timeval  *sending_time;
    icmp_h          *payload;
    size_t          ip_header_len;
    double          rtt;

    sending_time = NULL;
    memset(recv_buff, 0, BUFFER);
    size_t packet_len = recvfrom(ping_request->socket, recv_buff, BUFFER, 0, ping_request->ping_command->dest_sockaddr.dest_addr, &ping_request->ping_command->dest_sockaddr.addr_len);
    if (packet_len < 0)
        perror("Receiving the packet failed: ");

    ping_request->packet_received++;
    ip_header = (struct iphdr *)recv_buff;
    ip_header_len = ip_header->ihl * 4;
    payload = (icmp_h *)(recv_buff + ip_header_len);
    rtt = -1;

    if (ping_request->rtt)
    {
        sending_time = (struct timeval *)payload->data;
        rtt = calculate_rtt(sending_time);
    }

    if (payload->icmp_header.type == 0 && payload->icmp_header.un.echo.id == ping_request->id)
        print_ping_packet(payload->icmp_header.un.echo.sequence, sending_time, ip_header->ttl, rtt);
}

void ping_send_echo(void)
{
    struct timeval  sending_time;
    char            buffer[REQ_BUFF];
    icmp_h          *buffer_send;
    size_t          data_len;

    memset(buffer, 0, REQ_BUFF);
    buffer_send = (icmp_h *)buffer;
    memset(buffer_send->data, 0, PAYLOAD_BUFF);

    buffer_send->icmp_header.code = 0;
    buffer_send->icmp_header.type = 8;
    buffer_send->icmp_header.un.echo.id = ping_request->id;
    buffer_send->icmp_header.un.echo.sequence = ping_request->sequence++;
    buffer_send->icmp_header.checksum = 0;
    data_len = sizeof(buffer_send->icmp_header);
    data_len += sizeof(struct timeval);

    if (gettimeofday(&sending_time, NULL) != 0)
        perror("Get time of Day Error: ");
    memcpy(buffer_send->data, &sending_time, sizeof(struct timeval));
    buffer_send->icmp_header.checksum = calculate_checksum(buffer, data_len);

    int send = sendto(ping_request->socket, buffer, data_len, 0, ping_request->ping_command->dest_sockaddr.dest_addr, ping_request->ping_command->dest_sockaddr.addr_len);
    if (send < 0)
        perror("Send To Error: ");
    ping_request->packet_sent++;
}

void ping_init(p_cmd *ping_command)
{
    ping_request = (p_packet *)malloc(sizeof(p_packet));
    if (!ping_request)
        error_exit("Malloc: Memory Allocation Error");
    ping_request->ping_command = ping_command;
    ping_request->id = getpid();
    ping_request->sequence = 1;
    ping_request->packet_sent = 0;
    ping_request->packet_received = 0;
    ping_request->ip_header_sent = 20;
    ping_request->bytes_sent = ping_request->ping_command->options[SEND_BUFF] >= 0 ? ping_request->ping_command->options[SEND_BUFF] : 56;
    ping_request->bytes_received = ping_request->ping_command->options[SEND_BUFF] >= 0 ? ping_request->ping_command->options[SEND_BUFF] + 8 : 64;
    ping_request->ping_counter = MIN(ping_request->ping_command->options[DEADLINE], ping_request->ping_command->options[COUNT]);
    ping_request->rtt = ping_request->bytes_sent >= 16 ? true : false;
    ping_request->min_time = INT_MAX;
    ping_request->max_time = 0;
    ping_request->avg_time = 0;
    ping_request->total_time = 0;
    ping_request->ip_header_sent += ping_request->bytes_received;
    ping_request->mdev = NULL;
}

void socket_init(void)
{
    int ping_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping_sock == -1)
        error_exit("Socket: Socket creation Error");
    ping_request->socket = ping_sock;
    print_ping_start();
    alarm(1);
}