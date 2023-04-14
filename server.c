#include "proj.h"

int main(int argc, char *argv[]){
    /* define TCP socket */
    int sd; // socket descriptor

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){ // ensure sucess
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    /* store address, port, filename, and fd */
    int addr, port; // user defined address and port
    int fd; // file descriptor
     
    if(argc != 3){ // user has not passed address and port through cmd
        /* read address and port */
        char iaddr[30]; // address from input
        char iport[10]; // port from input

        printf("Enter address: ");
        scanf("%29s", iaddr);
        
        printf("Enter port: ");
        scanf("%9s", iport);

        addr = inet_addr(iaddr);
        port = atoi(iport);
    }
    else { // user has passed address and port through cmd 
        /* convert inet address and port to the convert data types */
        addr = inet_addr(argv[1]);
        port = atoi(argv[2]);
    }

    /* ensure success in reading addr and port */
    if(addr == -1){
        perror("Error converting Inet address");
        exit(EXIT_FAILURE);
    }

    if(port == 0){
        perror("Error converting port to integer");
        exit(EXIT_FAILURE);
    }

    /* define sockaddr_in */
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = addr;
    saddr.sin_port = htons(port);
    
    /* bind socket */
    if ((bind(sd, (SA*)&saddr, sizeof(saddr))) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    /* listening for a client */
    if ((listen(sd, 128)) < 0) { // marks socket as passive
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }
    printf("Server: listening\n");
    
    /* use accepts in a infinite loop to accept incoming connections */
    struct sockaddr_in client; // client to connect
    while(1) {
        socklen_t len = sizeof(client); // len of client struct
        int cli_sd; // client socket description
        if ((cli_sd = accept(sd, (SA*)&client, &len)) < 0) { // marks socket as passive
            perror("Accepting client failed: ");
            exit(EXIT_FAILURE);
        }
        printf("Server: Connected to %s, port %d\n", inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port));
        printf("sd = %d, rem_sd = %d\n", sd, cli_sd); // socket descriptors
        printf("receive_msg sd = %d\n", cli_sd);

        /* receive the transfer type from client */
        struct send_msg rec_msg; // message received 
        receive_msg(cli_sd, &rec_msg, sizeof(struct send_msg));
        printf("Server: Received message type %d from %s, port %d\n", rec_msg.msg_type, inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port));

        /* open output file */
        char *file_name = rec_msg.filename;
        int fd = open(file_name, O_RDWR | O_CREAT, 0644);
        if(fd < 0) {
            perror("Server open");
            close(sd);
            close(cli_sd);
            exit(EXIT_FAILURE);
        }

        /* sending message to client and receiving data from client */
        if(rec_msg.msg_type == CMD_SEND){ 
            /* sending response */
            int file_size = lseek(fd, 0, SEEK_END);
            int snd = send_mesg(CMD_RESP, CMD_RESP, cli_sd, file_size, STAT_OK, "None");
            printf("Server: sends response: type %d, status %d, and file size %d\n", CMD_RESP, STAT_OK, file_size);
            printf("send_msg sd = %d, leng = %d\n", cli_sd, snd);
            printf("Server: response sent\nServer awaits data\n");

            /* receiving data */
	        recv_data(cli_sd, rec_msg.filename, rec_msg.file_size);
	        printf("To server data transfer succeeded\n");
            close(cli_sd);
        } else if(rec_msg.msg_type == CMD_RECV){
            /* sending response */
            int file_size = lseek(fd, 0, SEEK_END);
            int snd = send_mesg(CMD_RESP, CMD_RESP, cli_sd, file_size, STAT_OK, "None");
            printf("Server: sends response: type %d, status %d, and file size %d\n", CMD_RESP, STAT_OK, file_size);
            printf("send_msg sd = %d, leng = %d\n", cli_sd, snd);
            printf("Server: response sent\n");

            /* sending data */
            send_data(cli_sd, file_name, file_size);
            printf("Server: %d bytes successfully sent\n", file_size);
        }
        
        close(cli_sd);
    }

    close(sd);          
}
