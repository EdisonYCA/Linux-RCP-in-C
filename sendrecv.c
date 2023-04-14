#include "proj.h"

/* sends a single message to the network */
int send_mesg(int resp_msg, int msg_type, int sd, int size, int status, char *filename) {
    int send_sd; // return value from send(2)

    /* determine message type */
    if (resp_msg == CMD_RESP) { // if server/client is sending a resp message
        struct resp_msg resp;
        resp.msg_type = msg_type;
        resp.status = status;
        resp.filesize = size;

        if ((send_sd = send(sd, &resp, sizeof(resp), 0)) == -1) {
            perror("send_msg send");
            close(sd);
            exit(EXIT_FAILURE);
        }
    }

    else { // if server/client is sending a regular message
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

/* receives a single message from the network */
void receive_msg(int sd, void *recv_str, int size) {
    int receive_sd;
    if ((receive_sd = recv(sd, &recv_str, size, 0)) == -1) {     
        perror("receive_msg recv");
        close(sd);
        exit(1);
    }
}

/* loops through reads on the file and sends 1024 blocks to the
network */
void send_data(int sd, char *filename, int filesize) {
    /* open filename */
    int rd; // return value of read(2)
    int bytes_sent = 0; // bytes sent to remote connection
    int init = 1; // bool value for initial send message
    int fd = open(filename, O_RDONLY); // file descriptor

    /* send data while bytes sent are less than size of file */
    while (bytes_sent < filesize) {
        /* read bytes */
        struct data_msg data;
        if ((rd = read(fd, &data.buffer, MAX_DATA_SIZE)) == -1) {
            perror("send_data read");
            exit(EXIT_FAILURE);
        }

        /* send data */
        data.msg_type = CMD_SEND;
        data.data_leng = rd;

        if (init) {
            printf("send_data: fd = %d, sd = %d, size = %d\n", fd, sd, size);
            init--;
        }

        if (send(sd, &data, sizeof(data), 0) == -1) {
            perror("send_data send");
            close(sd);
            exit(EXIT_FAILURE);
        }

        bytes_sent += data.data_leng;      

        printf("send_data sends %d bytes\n", data.data_leng);
    }

    close(fd);
}

/* loops through reads on the network and writes the blocks
received to the file */
void recv_data(int sd, char *filename, int filesize) {
    /* create the output file if does not exist */
    int fd = open(filename, O_RDWR);
    if(fd < 0) {
        perror("Recv_data open");
        close(sd);
        exit(EXIT_FAILURE);
    } 

    int bytes_read = 0; // bytes recieved
    int init = 1; // boolean for initial message 
    struct data_msg data; // data transfer structer

    /* read bytes while we haven't read entire file */
    while (bytes_read < filesize) {
        if (recv(sd, &data, sizeof(data), 0) == -1) { // receive message  
            perror("recv_data recv");
            close(fd);
            close(sd);
            exit(EXIT_FAILURE);
        }

        if (init) { // print on initial transfer
            printf("receive_data: fd = %d, sd = %d, size = %d\n", fd, sd, filesize);
            init--;
        }

        write(fd, &data.buffer, data.data_leng); // write data to output file
        bytes_read += data.data_leng;

        printf("receive_data writes %d bytes to file (total = %d)\n", data.data_leng, bytes_read);
    }

    close(fd);
}
