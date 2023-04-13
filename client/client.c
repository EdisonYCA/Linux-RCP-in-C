#include "../proj.h"

int main(int argc, char *argv[]){
    /* define TCP socket */
    int sd; // socket descriptor
    
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){ // ensure sucess
        perror("Error creating server socket: ");
        exit(EXIT_FAILURE);
    }

    /* store address and port */
    int addr, port; // user defined address and port
    char *ttp; // user defined transfer type
    FILE* file_name; // user defined file name

    if(argc != 5){ // user has not passed address and port through cmd
        /* read address and port */
        char iaddr[30]; // address from input
        char iport[10]; // port from input
        char tt[5]; // transfer type from input
        char file[30]; // file name from input

        printf("Enter address: ");
        scanf("%29s", iaddr);
        
        printf("Enter port: ");
        scanf("%9s", iport);

        printf("Enter transfer type (-s to send or -r to receive): ");
        scanf("%4s", tt);

        printf("Enter the name of the file: ");
        scanf("%29s", argv[4]);

        addr = inet_addr(iaddr);
        port = atoi(iport);
        ttp = tt;
        //file_name = file;
    }

    else { // user has passed address and port through cmd 
        /* convert inet address and port to the convert data types */
        addr = inet_addr(argv[1]);
        port = atoi(argv[2]);
        ttp = argv[3];
        //file_name = argv[4];
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
    printf("Client file name: %s\n", argv[4]);
    
    /* send transfer type to transfer */
    if(strcmp(ttp, "-s") == 0){
        file_name = fopen(argv[4], "r");
        if(file_name == NULL) {
	        perror("fopen");
	        exit(EXIT_FAILURE);
        }
        
        fseek(file_name, 0, SEEK_END);
        struct send_msg msg; // message to send
        msg.msg_type = CMD_SEND;
        msg.file_size = 0;
	    msg.file_size = ftell(file_name);
        
        int snd; // return value from send(2)
        if((snd = send(sd, &msg, sizeof(struct send_msg), 0)) < 0){ // send message and ensure success
            perror("Client could not send message: ");
            close(sd);
            exit(EXIT_FAILURE);
        }
        
        printf("Client sends command to server: type %d, filesize %d, filename %s\n", msg.msg_type,
        msg.file_size, argv[4]);
        printf("send_msg sd = %d, leng = #\n", sd);

        /*receiving message from client*/
        printf("receive_msg sd = %d\n", sd);

        printf("Client response received, type =, status =, filesize =\n");
    }
    
    close(sd);
}
