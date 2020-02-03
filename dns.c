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

#define BUFFER_SIZE 512 // https://tools.ietf.org/html/rfc1035 Section  4.2.1
#define MAX_LINE_LENGTH 1000
#define MAX_PROVIDER_NAME 30
#define MAX_IP_LENGTH 16

const char *host = "google.com";
int socket_fd;
struct sockaddr_in server;

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

    socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // create the socket to send data on
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
void printTime(struct timeval time) {

    if(time.tv_sec > 0) {
        printf("request took: %ld.%ds\n", time.tv_sec, time.tv_usec);
    } else {
        printf("request took: %dms\n", (time.tv_usec/1000));
    }
}
void sendData() {

    int i, message_len, rtn, offset, len, qname_size;
    struct DNS_HEADER *dns = NULL;
    char rcv_buf[BUFFER_SIZE];
    qname_size = strlen(host) + 1;
    message_len = sizeof(struct DNS_HEADER) + qname_size + 5;
    unsigned char message[message_len];
    unsigned short flags = 0, qd_count = 0, an_count = 0, ns_count = 0, ar_count = 0, qtype = 0, qclass = 0;
    struct timeval t1, t2, result;

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

    gettimeofday(&t1, NULL);
    if((rtn = sendto(socket_fd, (char *)message, message_len, 0, (struct sockaddr*)&server, sizeof(server))) < 0) {
        printf("return value: %d\n", rtn);
        perror("send failed");
    }

    if((rtn = recvfrom(socket_fd, rcv_buf, BUFFER_SIZE, 0, (struct sockaddr*)&server, (socklen_t*)&i)) > 0) {
        gettimeofday(&t2, NULL);
    } else {
        printf("rtn value: %d\n", rtn);
        perror("recieve failed");
    }

    result.tv_sec = t2.tv_sec - t1.tv_sec;
    result.tv_usec = t2.tv_usec - t1.tv_usec;
    printTime(result);
}
void readDNSList(char *filename) {

    FILE *fp;
    char str[MAX_LINE_LENGTH];
    char *token;
    char buf_ip[MAX_IP_LENGTH], buf_prvd[MAX_PROVIDER_NAME];
    char *server_ip = buf_ip, *provider_name = buf_prvd;


    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("File Open Error");
        exit(1);
    }

    while (fgets(str, MAX_LINE_LENGTH, fp) != NULL) {
        getIP(str, buf_ip, MAX_IP_LENGTH);
        getProviderName(str, buf_prvd, MAX_PROVIDER_NAME);
        printf("IP: %s\t Provider: %s\n", server_ip, provider_name);
        createConnection(server_ip);
        sendData();
        closeConnection();
    }

    fclose(fp);
}
int main(int argc, char **argv) {

    readDNSList(argv[1]);
}
