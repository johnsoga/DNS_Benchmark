#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include "dns_types.h"
#include "data_struct.h"

#define BUFFER_SIZE 512 // https://tools.ietf.org/html/rfc1035 Section  4.2.1
#define MAX_LINE_LENGTH 1000
#define MAX_PROVIDER_NAME 30
#define MAX_IP_LENGTH 16
#define MAX_ATTEMPTS 50

// LinkedList *serverList;
node *serverList;
// serverList->HEAD = NULL;
// serverList->size = 0;
const char *host = "google.com";
int socket_fd;
int count_average_time = 0;
struct sockaddr_in server;
struct timespec average_time;
int DEBUG_ON = 1;

void averageTime(struct timespec new_time) {

    count_average_time++;

    average_time.tv_sec += new_time.tv_sec;
    if(average_time.tv_nsec + new_time.tv_nsec >= 1000000000) {
        average_time.tv_sec++;
        average_time.tv_nsec = (1000000000 - average_time.tv_nsec) - new_time.tv_nsec;
    } else {
        average_time.tv_nsec += new_time.tv_nsec;
    }
}
void getIP(char *string, char buf[], int len) {

    int i;

    for(i = 0; i < strlen(string); i++) {
        if(string[i] != ',') {
            buf[i] = string[i];
        } else {
            buf[i] = '\0';
            return;
        }
    }
}
void getProviderName(char *string, char buf[], int len) {

    // proivder is the second item in the comma delimited line
    int i, j, count = 0;

    //TODO: ERROR HANDELING
    for(i = 0; i < strlen(string); i++) {
        if(string[i] == ',') {
            j = 0;
            i++;
            while(string[i] != ',') {
                buf[j] = string[i];
                j++;
                i++;
            }
            buf[j] = '\0';
            return;
        }
    }
}
void createConnection(char *server_ip) {

    struct timeval timeout;

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // TODO: Error Handling
    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("Error");
    }

    if(socket_fd > 0) {
        memset(&server, '\0', sizeof(server));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(server_ip);
        server.sin_port = htons(53);
    } else {
        perror("Failed to Create Socket");
        exit(1);
    }
}
void closeConnection() {

    if(close(socket_fd) != 0) {
        perror("Failed to Close Socket");
        exit(1);
    }
}
struct timespec sendData() {

    int i, message_len, rtn, offset, len, qname_size;
    struct DNS_HEADER *dns = NULL;
    char rcv_buf[BUFFER_SIZE];
    qname_size = strlen(host) + 1;
    message_len = sizeof(struct DNS_HEADER) + qname_size + 5;
    unsigned char message[message_len];
    unsigned short flags = 0, qd_count = 0, an_count = 0, ns_count = 0, ar_count = 0, qtype = 0, qclass = 0;
    struct timespec start, end, result;

    // put the DNS Header info into the buffer
    dns = (struct DNS_HEADER*)&message;
    dns->id = (unsigned short)getpid();             // set id
    flags = flags ^ 0x0000;                         // set query                0... .... .... ....
    flags = flags ^ 0x0000;                         // set opcode               .000 0... .... ....
    flags = flags ^ 0x0000;                         // set authoritative answer .... .0.. .... ....
    flags = flags ^ 0x0000;                         // set truncated message    .... ..0. .... ....
    flags = flags ^ 0x0100;                         // set recursion desired    .... ...1 .... ....
    flags = flags ^ 0x0000;                         // set recursion available  .... .... 0... ....
    flags = flags ^ 0x0000;                         // set z (reserved)         .... .... .000 ....
    flags = flags ^ 0x0000;                         // set rcode                .... .... .... 0000
    dns->flags = htons(flags);
    dns->qd_count = htons(qd_count ^ 0x0001);       // set query count
    dns->an_count = an_count ^ 0x0000;              // set number of answer
    dns->ns_count = ns_count ^ 0x0000;              // set number of authoritative RR
    dns->ar_count = ar_count ^ 0x0000;              // set number of additional RR


    // put the QNAME, QTYPE, and QCLASS info into the buffer
    offset = sizeof(struct DNS_HEADER);
    char hostname[100];
    strncpy(hostname, host, strlen(host));
    char *tmp = hostname, *token;
    while ((token = strsep(&tmp, ".")) != NULL) {
        len = strlen(token);
        message[offset++] = (unsigned char)(strlen(token));
        for(i = 0; i < len; i++) {
            message[offset++] = token[i];
        }
    }
    message[offset++] = (unsigned char)0;
    message[offset++] = (unsigned char)0;
    message[offset++] = (unsigned char)1;
    message[offset++] = (unsigned char)0;
    message[offset] = (unsigned char)1;

    clock_gettime(CLOCK_MONOTONIC, &start);
    if((rtn = sendto(socket_fd, (char *)message, message_len, 0, (struct sockaddr*)&server, sizeof(server))) < 0) {
        printf("return value: %d\n", rtn);
        perror("send failed");
    }

    if((rtn = recvfrom(socket_fd, rcv_buf, BUFFER_SIZE, 0, (struct sockaddr*)&server, (socklen_t*)&i)) > 0) {
        clock_gettime(CLOCK_MONOTONIC, &end);
    } else {
        printf("rtn value: %d\n", rtn);
        perror("recieve failed");
    }

    // https://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
    result.tv_sec = 0;
    result.tv_nsec = 0;
    if (end.tv_nsec < start.tv_nsec) {
        result.tv_sec = end.tv_sec - start.tv_sec - 1;
        result.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        result.tv_sec = end.tv_sec - start.tv_sec;
        result.tv_nsec = end.tv_nsec - start.tv_nsec;
    }

    return result;
}
void readDNSList(char *filename) {

    FILE *fp;
    char str[MAX_LINE_LENGTH];
    char *token;
    char buf_ip[MAX_IP_LENGTH], buf_prvd[MAX_PROVIDER_NAME];
    char *server_ip = buf_ip, *provider_name = buf_prvd;
    struct timespec total_time;
    struct timespec avg_time;
    int count;
    int debug_counter;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("File Open Error\n");
        exit(1);
    }

    average_time.tv_sec = 0;
    average_time.tv_nsec = 0;
    while (fgets(str, MAX_LINE_LENGTH, fp) != NULL) {
        getIP(str, buf_ip, MAX_IP_LENGTH);
        getProviderName(str, buf_prvd, MAX_PROVIDER_NAME);
        printf("Provider: %-15s\tIP: %s\n", provider_name, server_ip);

        for(count = 0, debug_counter = 0; count < MAX_ATTEMPTS; count++, debug_counter++) {
            createConnection(server_ip);
            total_time = sendData();
            averageTime(total_time);

            if(DEBUG_ON) {

                if(total_time.tv_sec > 0) {
                    printf("T%03d:%3ld.%3lds", (count + 1), total_time.tv_sec, total_time.tv_nsec);
                } else {
                    printf("T%03d:%3ldms\t", (count + 1), (total_time.tv_nsec/1000000));
                }
                if((count == MAX_ATTEMPTS - 1) || ((count+1) % 10 == 0)) {
                    printf("\n");
                }
            }
            closeConnection();
        }
        if(average_time.tv_sec > 0) {
            printf("Average: %ld.%lds\n", average_time.tv_sec, (average_time.tv_nsec/1000000/count_average_time));
        } else {
            printf("Average: %ldms\n", (average_time.tv_nsec/1000000/count_average_time));
        }
        average_time.tv_sec = 0;
        average_time.tv_nsec = 0;
    }

    fclose(fp);
}

dnsserver parseDNSServer(char str_buf[]) {

    int counter;
    int position = 0;
    char buf_ip[MAX_IP_LENGTH];
    char buf_prvd[MAX_PROVIDER_NAME];
    dnsserver *tmp = malloc(sizeof(dnsserver));

    // Get the DNS Server IP
    counter = 0;
    while(1) {
        if(str_buf[position] != ',') {
            buf_ip[counter] = str_buf[position];
        } else {
            buf_ip[counter] = '\0';
            tmp->DNSProviderIP = malloc(sizeof(buf_ip));
            strcpy(tmp->DNSProviderIP, buf_ip);
            position++;
            break;
        }
        position++;
        counter++;
    }

    // Get the DNS Server Name
    counter = 0;
    while(1) {
        if(str_buf[position] != ',') {
            buf_prvd[counter] = str_buf[position];
        } else {
            buf_prvd[counter] = '\0';
            tmp->DNSProviderName = malloc(sizeof(buf_prvd));
            strcpy(tmp->DNSProviderName, buf_prvd);
            position++;
            break;
        }
        position++;
        counter++;
    }

    return *tmp;
}
void loadDNSList(char *filename) {

    FILE *fp;
    dnsserver dnsserver;
    char str_buf[MAX_LINE_LENGTH];

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("File Open Error\n");
        exit(1);
    }

    // printf("success");
    printf("%d", serverList->size);
    // printf("fail");

    // while (fgets(str_buf, MAX_LINE_LENGTH, fp) != NULL) {
    //     dnsserver = parseDNSServer(str_buf);
    //     insertAtPostion(serverList, 1, &dnsserver);
    //     // printf("%s:%s\n", dnsserver.DNSProviderName, dnsserver.DNSProviderIP);
    // }
    fclose(fp);
}
int main(int argc, char **argv) {

    loadDNSList(argv[1]);

    // readDNSList(argv[1]);
}
