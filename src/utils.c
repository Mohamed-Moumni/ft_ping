#include "ft_ping.h"

void ping_help(void)
{
    printf("Usage:\n\n");
    printf("ft_ping [options] <destination>\n\n");
    printf("Options:\n\n");
    printf("<destination>            dns name or ip address\n");
    printf("-h                       print help and exit\n");
    printf("-n                       no dns name resolution\n");
    printf("-c <count>               stop after <count> replies\n");
    printf("-w <deadline>            reply wait <deadline> in seconds\n");
    printf("-s <size>                use <size> as number of data bytes to be sent\n");
    exit(0);
}

void error_exit(const char *error_msg)
{
    printf("ft_ping: %s\n", error_msg);
    ping_help();
}

int get_option_value(const char *value)
{
    if (!value)
        error_exit("option requires an argument --");
    else
    {
        for (size_t i = 0; i < strlen(value); i++)
        {
            if (!isdigit(value[i]))
                error_exit("out of range: 0 <= value <= 2147483647");
        }
    }
    return atoi(value);
}

void print_ping_start(void)
{
    printf("FT_PING %s (%s) %d(%d) bytes of data.\n", ping_request->ping_command->destination, ping_request->ping_command->network_repr, ping_request->bytes_sent, ping_request->ip_header_sent);
}

double calculate_rtt(struct timeval *sending_time)
{
    struct timeval receiving_time;
    double rtt;

    if (gettimeofday(&receiving_time, NULL) != 0)
        error_exit("Get Time of Day Error");
    rtt = (receiving_time.tv_sec - (*sending_time).tv_sec) * 1000 + ((receiving_time.tv_usec - (*sending_time).tv_usec) * 0.001);
    return rtt;
}

void print_ping_packet(const int seq, struct timeval *sending_time, const int ttl, const double rtt)
{
    ping_request->max_time = MAX(ping_request->max_time, rtt);
    ping_request->min_time = MIN(ping_request->min_time, rtt);

    if (ping_request->ping_command->options[NUMERIC_ONLY] != -1)
    {
        if (ping_request->rtt)
            printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n", ping_request->bytes_received, ping_request->ping_command->network_repr, seq, ttl, rtt);
        else
            printf("%d bytes from %s: icmp_seq=%d ttl=%d\n", ping_request->bytes_received, ping_request->ping_command->network_repr, seq, ttl);
    }
    else
    {
        if (ping_request->rtt)
            printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n", ping_request->bytes_received, ping_request->ping_command->reverse_dns, ping_request->ping_command->network_repr, seq, ttl, rtt);
        else
            printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d \n", ping_request->bytes_received, ping_request->ping_command->reverse_dns, ping_request->ping_command->network_repr, seq, ttl);
    }
    ping_request->total_time += rtt;
    add_mdev_list(&ping_request->mdev, create_mdev_list(rtt));
}

void print_ping_stats(void)
{
    double loss;
    double total_time;
    double mdev;

    total_time = calculate_rtt(&ping_request->start_date);

    loss = 100 - (ping_request->packet_received / ping_request->packet_sent) * 100;
    mdev = calculate_mdev();
    printf("\n--- %s ft_ping statistics ---\n", ping_request->ping_command->destination);
    if (ping_request->rtt)
    {
        printf("%d packets transmitted, %d received, %.f%% packet loss, time %.0fms\n", ping_request->packet_sent, ping_request->packet_received, loss, total_time);
        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", ping_request->min_time, ping_request->total_time / ping_request->packet_received, ping_request->max_time, mdev);
    }
}

void ping_exit_handler(int signal)
{
    print_ping_stats();
    exit(0);
}

void show_version(void)
{
    printf("ft_ping 1.0\n");
    exit(0);
}

mdev *create_mdev_list(const double rtt)
{
    mdev *ping_mdev = (mdev *)malloc(sizeof(mdev));
    if (!ping_mdev)
        error_exit("Malloc: Memory Allocation Error");
    ping_mdev->rtt = rtt;
    ping_mdev->next = NULL;
    return ping_mdev;
}

mdev *last_mdev(mdev *ping_mdev_list)
{
    mdev *ping_temp = ping_mdev_list;

    while (ping_temp->next != NULL)
    {
        ping_temp = ping_temp->next;
    }
    return ping_temp;
}

void add_mdev_list(mdev **ping_mdev_list, mdev *new_mdev)
{
    mdev *last_mdev_list;

    if (*ping_mdev_list == NULL)
        (*ping_mdev_list) = new_mdev;
    else
    {
        last_mdev_list = last_mdev(*ping_mdev_list);
        last_mdev_list->next = new_mdev;
    }
}

double calculate_mdev(void)
{
    double n;
    double sum;
    double mean;
    mdev *temp;

    n = ping_request->packet_received;
    sum = 0;
    mean = 0;
    temp = ping_request->mdev;
    while (temp != NULL)
    {
        mean += temp->rtt;
        temp = temp->next;
    }
    mean /= n;
    temp = ping_request->mdev;
    while (temp != NULL)
    {
        sum += pow(2, (temp->rtt - mean));
        temp = temp->next;
    }
    return sqrt(sum / n);
}