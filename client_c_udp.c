//Code skeleton was taken from discussion sections slides. I used
//geekforgeeks, stack overflow, and linux man pages to find out
//what functions i need to use to complete the homework assignment and to 
//error handle. 
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
    //initialization of variables
    char STOP[] = "Sorry, cannot compute!";
    struct sockaddr_in server_address, return_address;

    //check arguments
    if(argc < 3 || argc > 3){
        printf("Error : Input Error \n");
        exit(EXIT_FAILURE); 
    }
    //initialization of data
    char server_ip[20];
    memset(server_ip, 0, 20);
    strcpy(server_ip, argv[1]);
    int port = atoi(argv[2]);
    //port error check
    if(port < 0 || port > 65535){
        printf("Error : Invalid Port Number \n");
        exit(EXIT_FAILURE); 
    }
    
    //initialization of buffers, take user input string
    char message[256];
    char input[256];
    memset(message, 0 , sizeof(message));
    memset(input, 0 , sizeof(input));
    printf("Enter string: ");
    fgets(input, 256, stdin);
    memcpy(message,input, strlen(input)-1);

    //basic error handling and initialization
    int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_id < 0){
        printf("Error : Could not create socket \n");
        exit(EXIT_FAILURE); 
    }
    if(inet_addr(server_ip) == -1){
        printf("Error : inet_addr returned -1, please use a different IP \n");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(port);



    //send main string
    sendto( socket_id, message, strlen(message), 0,
    (struct sockaddr *) &server_address, sizeof(struct sockaddr_in));

    while(1){
        //waiting for server responses and sending back uncompleted iterations of message

        //receive message with some initialization and buffer clearing
        memset(message, 0 , sizeof(message));
        unsigned int return_len;
        recvfrom( socket_id, message, 256, 0,
        (struct sockaddr *) &return_address, &return_len );
        //in case string had an error stop
        if(strcmp(STOP, message) == 0){
            printf("From server: '%s'\n",message);
            close(socket_id);
            return 0;
        }//end case
        else if(strlen(message) == 1){
            printf("From server: '%s'\n", message);
            close(socket_id);
            return 0;
        }
        else{//send iteration retrieved back to server to get next iteration of sum
            printf("From server: '%s'\n", message);
            sendto( socket_id, message, strlen(message), 0,
            (struct sockaddr *) &server_address, sizeof(struct sockaddr_in));
        }
    }
    
  
    close(socket_id);
}
    /*
    struct sockaddr_in server_address, return_address;
    int socket_id = 0;
    char server_ip[] = "127.0.0.1";
    int port = 13805;
    char message[129];
    printf("Enter string: ");
    fgets(message, 129, stdin);
    strtok(message, "\n");
    strcat(message, "\0");

    if ( socket_id = socket(AF_INET, SOCK_DGRAM, 0) < 0){
        printf("\n Error : Could not create socket \n");
        exit(EXIT_FAILURE); 
    }

    memset(&server_address, '0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(port);

    printf("Sending '%s' to %s:%i\n", message, server_ip, port);
    sendto( socket_id, message, strlen(message), MSG_CONFIRM, (struct sockaddr *) &server_address, sizeof(struct sockaddr_in) );
    
    printf("Waiting for Response\n");
    unsigned int return_len, n = 0;

    recvfrom( socket_id, message, strlen(message), MSG_WAITALL, (struct sockaddr *) &return_address, &return_len);
    printf("Received '%s'\n", message);
    close(socket_id);
    return 0;
    */
