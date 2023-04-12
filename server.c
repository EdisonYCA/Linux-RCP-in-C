#include "proj.h"

int main(int argc, char *argv[]){
    /* define TCP socket */
    int sd; // socket descriptor
    // FILE *file;
    // struct send_msg sendMessage;
    
    // int rem_sd, receive, send, size;
    
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){ // ensure sucess
        perror("Error creating server socket: ");
        exit(EXIT_FAILURE);
    }

    /* store address and port */
    int addr, port; // user defined address and port
     
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
    saddr.sin_addr.s_addr = addr;
    saddr.sin_port = htons(port);
    
    /* bind socket */
    if ((bind(sd, (SA*)&saddr, sizeof(saddr))) < 0) {
        perror("Bind failed: ");
        exit(EXIT_FAILURE);
    }

               
    /* listening for a client */
    if ((listen(sd, 128)) < 0) { // marks socket as passive
        perror("Listening failed: ");
        exit(EXIT_FAILURE);
    }
    printf("Server: listening\n");
    
    /* use accepts in a infinite loop to accept incoming connections */
    struct sockaddr_in client; // client to connect

    while(1) {
        int len = sizeof(client); // len of client struct
        int cli_sd; // client socket description
        if ((cli_sd = accept(sd, (SA*)&client, &len)) < 0) { // marks socket as passive
            perror("Accepting client failed: ");
            exit(EXIT_FAILURE);
        }
        printf("Server: Connected to %s, port %d\n", inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port));
    }

    close(sd);              
}
