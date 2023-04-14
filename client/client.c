#include "../proj.h"

int main(int argc, char *argv[]){
    /* define TCP socket */
    int sd; // socket descriptor
    int sock_sd;
     
    //sock_sd = socket(AF_INET, SOCK_STREAM, 0);
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){ // ensure sucess
        perror("Error creating server socket: ");
        exit(EXIT_FAILURE);
    }

    /* store address and port */
    int addr, port; // user defined address and port
    char *ttp; // user defined transfer type
    char *file_name; // user defined file name

    if(argc != 5){ // user has not passed address and port through cmd
        /* read address and port */
        char iaddr[30]; // address from input
        char iport[10]; // port from input
        char tt[5]; // transfer type from input
        char ifile_name[30]; // file name from input

        printf("Enter address: ");
        scanf("%29s", iaddr);
        
        printf("Enter port: ");
        scanf("%9s", iport);

        printf("Enter transfer type (-s to send or -r to receive): ");
        scanf("%4s", tt);

        printf("Enter the name of the file: ");
        scanf("%29s", ifile_name);

        addr = inet_addr(iaddr);
        port = atoi(iport);
        ttp = tt;
        file_name = ifile_name;
    }

    else { // user has passed address and port through cmd 
        /* convert inet address and port to the convert data types */
        addr = inet_addr(argv[1]);
        port = atoi(argv[2]);
        ttp = argv[3];
        file_name = argv[4];
    }

    /* define sockaddr_in */
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = addr;
    saddr.sin_port = htons(port);
    
    /* connect to the server */
    int con; // return of connect func
    
    if((con = connect(sd, (SA*)&saddr, sizeof(saddr))) < 0){
        perror("Connection denied");
        close(sd);
        exit(EXIT_FAILURE);
    }
    
    printf("Client: Connected to %s, port %d\n", inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port));
    printf("Client file name: %s\n", file_name);
    
    /* send transfer type to transfer */
    if(strcmp(ttp, "-s") == 0){
        /* open input file */
        int fd = open(file_name, O_RDONLY);
        if(fd < 0) {
	        perror("open");
	        exit(EXIT_FAILURE);
        }

        /* sending transfer type */
        int filesize = lseek(fd, 0, SEEK_END);
        int snd = send_mesg(CMD_SEND, CMD_SEND, sd, filesize, 0, file_name);
        printf("Client sends command to server: type %d, filesize %d, filename %s\n", CMD_SEND,
        filesize, file_name);
        printf("send_msg sd = %d, leng = %d\n", sd, snd);

        /* receiving response from server */
        printf("receive_msg sd = %d\n", sd);
        struct resp_msg rec; // message received
        receive_msg(sd, &rec, sizeof(struct resp_msg));
        printf("Client response received, type = %d, status = %d, filesize = %d\n", rec.msg_type, rec.status, rec.filesize);
        printf("Client awaits server response\n");

        /* sending data */
        send_data(sd, file_name,filesize);
        printf("Client: %d bytes successfully sent\n", filesize);
    }
    
    close(sd);
}
