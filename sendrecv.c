#include "proj.h"

int send_mesg(int resp_msg, int msg_type, int sd, int size, int status, char *filename) {
    int send_sd;

    if (resp_msg == CMD_RESP) {
        struct resp_msg resp;
        resp.msg_type = msg_type;
        resp.status = status;
        resp.filesize = size;

        if ((send_sd = send(sd, &resp, sizeof(resp), 0)) == -1) {
            perror("send_msg send");
            close(sd);
            exit(1);
        }
    }
    else {
        struct send_msg ssend;
        ssend.msg_type = msg_type;
        ssend.file_size = size;
        strcpy(ssend.filename, filename);

        if ((send_sd = send(sd, &ssend, sizeof(ssend), 0)) == -1) {
            perror("send_msg send");
            close(sd);
            exit(1);
        }
    }

    return send_sd;
}

int receive_msg(int sd, void *recv_str, int size) {
    int receive_sd;
    if ((receive_sd = recv(sd, recv_str, size, 0)) == -1) {     
        perror("receive_msg recv");
        close(sd);
        exit(1);
    }

    return receive_sd;
}

void send_data(int sd, char *filename, int size) {
    ssize_t data_len;
    int currSize = 0;
    int init = 1;
    int fd = open(filename, O_RDONLY, 0644);

    while (currSize < size) {
        struct data_msg data;
        if ((data_len = read(fd, &data.buffer, MAX_DATA_SIZE)) == -1) {
            perror("send_data read");
            exit(1);
        }

        data.msg_type = CMD_SEND;
        data.data_leng = data_len;

        if (init) {
            printf("send_data: fd = %d, sd = %d, size = %d\n", fd, sd, size);
            init--;
        }

        if (send(sd, &data, sizeof(data), 0) == -1) {
            perror("send_data send");
            close(sd);
            exit(1);
        }

        currSize += data.data_leng;      

        printf("send_data sends %d bytes\n", data.data_leng);
    }

    close(fd);
}

void recv_data(int sd, char *filename, int size) {
    int fd = open(filename, O_RDWR | O_CREAT, 0644);
    int currSize = 0;
    int init = 1;
    int receive_sd;
    struct data_msg data; 

    while (currSize < size) {
        if ((receive_sd = recv(sd, &data, sizeof(data), 0)) == -1) {         
            perror("recv_data recv");
            close(fd);
            close(sd);
            exit(1);
        }

        if (init) {
            printf("receive_data: fd = %d, sd = %d, size = %d\n", fd, sd, size);
            init--;
        }

        write(fd, &data.buffer, data.data_leng);
        currSize += data.data_leng;

        printf("receive_data writes %d bytes to file (total = %d)\n", data.data_leng, currSize);
    }

    close(fd);
}
