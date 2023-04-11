#include "proj.c"

int main(int argc, char *argv[]){
    /* define TCP socket */
    int sd; // socket descriptor
    FILE *file;
    struct send_msg sendMessage;
    
    int rem_sd, receive, send, size;
    
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){ // ensure sucess
        perror("Error creating server socket: ");
        exit(EXIT_FAILURE);
    }

    /* store address and port */
    int addr, port; // user defined address and port
    unsigned char buf[sizeof(struct in_addr)];
     

    if(argc != 3){ // user has not passed address and port through cmd
        /* read address and port */
        char iaddr[30]; // address from input
        char iport[10]; // port from input

        printf("Enter address: ");
        scanf("%29s", iaddr);
        
        printf("Enter port: ");
        scanf("%9s", iport);

        addr = inet_pton(AF_INET, iaddr, buf);
        port = atoi(iport);
    }
    else { // user has passed address and port through cmd 
        /* convert inet address and port to the convert data types */
        addr = inet_pton(AF_INET, argv[1], buf);
        port = atoi(argv[2]);
    }

    /* ensure success in reading addr and port */
    if(addr == -1){
        perror("Error converting Inet address: ");
        exit(EXIT_FAILURE);
    }

    if(port == 0){
        perror("Error converting port to integer: ");
        exit(EXIT_FAILURE);
    }

    /* define sockaddr_in */
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(addr);
    saddr.sin_port = htons(port);
    
    /*binding*/
    if ((bind(sd, (struct sockaddr *)&saddr, sizeof(saddr))) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    else
        printf("Successful bind...\n);
               
    /*listening*/
    if ((listen(sd, 128)) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }
    else
        printf("Server's listening...\n");
    
    while(1) {
        printf("Server: Connected to %s, port %d\n", addr, port);
        receive = receive_msg(rem_sd, &sendMessage, sizeof(sendMessage);
        printf("sd = %d, rem_sd = %d\n", sd, rem_sd);
        printf("receive_msg sd = %d\n", receive);
        printf("Server: Received message type %d from %s, port %d\n", sendMessage.msg_type, addr, port);
        
        
    }
               
}
