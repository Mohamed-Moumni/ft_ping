#include "ft_ping.h"

unsigned short calculate_checksum(void *b, int len) {
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
    while (1)
    {
        ping_echo_replay();
        usleep(1000);
    }
}

void ping_send_handler(int signal)
{
    ping_send_echo();
    alarm(1);
}

void ping_echo_replay(void)
{
    // printf("Ping Echo Replay\n");
    char recv_buff[BUFFER];
    struct iphdr *ip_header;
    icmp_h *payload;

    
    memset(recv_buff, 0, BUFFER);
    size_t packet_len = recvfrom(ping_request->socket, recv_buff, BUFFER, 0, ping_request->ping_command->dest_sockaddr.dest_addr, &ping_request->ping_command->dest_sockaddr.addr_len);
    if (packet_len < 0)
    {
        perror("Receiving the packet failed: ");
    }
    ip_header = (struct iphdr *)recv_buff;

    size_t ip_header_len = ip_header->ihl * 4;
    size_t payload_len = packet_len - ip_header_len;
    payload = (icmp_h *)(recv_buff + ip_header_len);

    printf("Show ICMP Header Attributes: \n");
    // icmp_h *recv_header;

    // recv_header = (icmp_h *)recv_buff;
    printf("Recv code %d\n", payload->icmp_header.code);
    printf("Recv type %d\n", payload->icmp_header.type);
    printf("Recv id   %d\n", payload->icmp_header.un.echo.id);
    printf("Recv seq  %d\n", payload->icmp_header.un.echo.sequence);

    // struct sockaddr *dest_addr = NULL;
    // socklen_t des_addr_len;
    // struct addrinfo *rp;

    // memset(recv_buff, 0, 16);
    // for (rp = ping_request->address; rp != NULL; rp = rp->ai_next)
    // {
    //     if (rp->ai_family == AF_INET)
    //     {
    //         dest_addr = rp->ai_addr;
    //         des_addr_len = rp->ai_addrlen;
    //         break;
    //     }
    // }
    // printf("Dest Addr: %p\n", dest_addr);
    // printf("Dest Addr Len: %d\n", des_addr_len);
    // printf("Before Recv From\n");
    // int recv = recvfrom(ping_request->socket, recv_buff, 8, 0, dest_addr, &des_addr_len);
    // if (recv < 0)
    // {
    //     perror("Recv Method Error");
    //     error_exit("Recv From Error");
    // }
    // printf("After Recv From\n");
    // printf("Show ICMP Header Attributes: \n");
    // icmp_h *recv_header;

    // recv_header = (icmp_h *)recv_buff;
    // printf("Recv code %d\n", recv_header->icmp_header.code);
    // printf("Recv type %d\n", recv_header->icmp_header.type);
    // printf("Recv id   %d\n", recv_header->icmp_header.un.echo.id);
    // printf("Recv seq  %d\n", recv_header->icmp_header.un.echo.sequence);
}

void ping_send_echo(void)
{
    // printf("Ping Send Echo Call\n");
    // printf("ID %d\n", ping_request->id);
    char buffer[REQ_BUFF];
    icmp_h *buffer_send;
    size_t data_len;

    memset(buffer, 0, REQ_BUFF);

    buffer_send = (icmp_h *)buffer;
    buffer_send->icmp_header.code = 0;
    buffer_send->icmp_header.type = 8;
    buffer_send->icmp_header.un.echo.id = ping_request->id;
    buffer_send->icmp_header.un.echo.sequence = ping_request->sequence++;
    buffer_send->icmp_header.checksum = 0;
    data_len = sizeof(buffer_send->icmp_header);
    // printf("data Len: %ld\n", data_len);
    buffer_send->icmp_header.checksum = calculate_checksum(buffer, data_len);
    // printf("Recv code %d\n", buffer_send->icmp_header.code);
    // printf("Recv type %d\n", buffer_send->icmp_header.type);
    // printf("Recv id   %d\n", buffer_send->icmp_header.un.echo.id);
    // printf("Recv seq  %d\n", buffer_send->icmp_header.un.echo.sequence);
    int send = sendto(ping_request->socket, buffer, data_len, 0, ping_request->ping_command->dest_sockaddr.dest_addr, ping_request->ping_command->dest_sockaddr.addr_len);
    if (send < 0)
        perror("Send To Error: ");
}

void ping_init(p_cmd * ping_command)
{
    ping_request = (p_packet *)malloc(sizeof(p_packet));
    if (!ping_request)
        error_exit("Malloc: Memory Allocation Error");
    ping_request->ping_command = ping_command;
    ping_request->id = getpid();
    ping_request->sequence = 1;
}

void socket_init(void)
{
    // struct timeval read_timeout;

    int ping_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (ping_sock == -1)
        error_exit("Socket: Socket creation Error");
    ping_request->socket = ping_sock;
    signal(SIGALRM, ping_send_handler);
    alarm(1);
}