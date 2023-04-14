/* for orginizational purposes, this file will contain any defn's
that can be shared between the client and the server.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

extern int errno;

void receive_msg(int, void*, int);
int send_mesg(int, int, int, int, int, char*);
void send_data(int, char*, int);
void recv_data(int, char*, int);

#define MAX_DATA_SIZE 1024
#define STAT_OK 1
#define STAT_FAIL -1
#define CMD_SEND 1
#define CMD_RECV 2
#define CMD_RESP 3
#define CMD_DATA 4
#define SA struct sockaddr

struct send_msg {
    int msg_type;
    int file_size;
    char filename[128];
};

struct resp_msg {
    int msg_type;
    int status;
    int filesize;
};

struct data_msg {
    int msg_type;
    int data_leng;
    char buffer[1024];
};
