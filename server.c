#include "proj.c"

extern int errno; 
int main(int argc, char *argv[]){
    /* define TCP socket */
    int sd; // socket descriptor
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
}

    /*binding*/
    if (bind(sd, 
