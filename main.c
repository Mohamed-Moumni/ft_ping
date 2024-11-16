#include<stdio.h>
#include<signal.h>
#include <unistd.h>

void alarm_handler(int signum){
    printf("Alaram Signal Recieved %d\n", signum);
}

int main(void){
    signal(SIGALRM, alarm_handler);
    alarm(5);
    printf("Alarm Method is Called!!!\n");
    sleep(10);
    printf("After the SLEEP METHOD is Called\n");
    return (0);
}