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

    /* define sockaddr_in */
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = addr;
    saddr.sin_port = htons(port);
    
    /* connect to the server */
    int con; // return of connect func
    
    if((con = connect(sd, (SA*)&saddr, sizeof(saddr))) < 0){
        perror("Connection denied: ");
        exit(EXIT_FAILURE);
    }
    
    printf("Client: Connected to %s, port %d\n", inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port));
    
    close(sd);
}
