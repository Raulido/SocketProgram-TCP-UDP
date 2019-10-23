//Code skeleton was taken from discussion sections slides. I used
//geekforgeeks, stack overflow, and linux man pages to find out
//what functions i need to use to complete the homework assignment and how to 
//error handle. 
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
int main(int argc, char * argv[]) {
    //error message
    char STOP[] = "Sorry, cannot compute!";
    //argument testing
    if(argc < 3 || argc > 3){
        printf("Error : Input Error \n");
        exit(EXIT_FAILURE); 
    }
    //initialization
    char server_ip[20];
    memset(server_ip, 0, 20);
    strcpy(server_ip, argv[1]);
    int port = atoi(argv[2]);
    //port error check
    if(port < 0 || port > 65535){
        printf("Error : Invalid Port Number \n");
        exit(EXIT_FAILURE); 
    }

    //main buffers, retrieve user input
    char message[256];
    char input[256];
    memset(message, 0 , sizeof(message));
    memset(input, 0 , sizeof(input));
    printf("Enter string: ");
    fgets(input, 256, stdin);
    memcpy(message,input, strlen(input)-1);

    //socket decleration and error check
    int socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_id < 0){
        printf("Error : Could not create socket \n");
        exit(EXIT_FAILURE); 
    }
    
    //check to see if ip is good, ****255.255.255.255 will return an error please use different ip****
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    if(inet_addr(server_ip) == -1){
        printf("Error : inet_addr returned -1, please use a different IP \n");
        exit(EXIT_FAILURE);
    }
    server_address.sin_addr.s_addr = inet_addr( server_ip );
    server_address.sin_port = htons(port);

    //second socket with error handling
    int connection_socket_id = connect(
    socket_id,(struct sockaddr *) &server_address,
    sizeof(server_address) );
    if(connection_socket_id < 0){
        printf("Error : Could not connect\n");
        exit(EXIT_FAILURE);
    }

    //send main message
    send(socket_id, message, strlen(message), 0);
    while(1){
        //retrieve multiple messages from client with different iterations of sum

        //buffer + read
        memset(message, 0 , sizeof(message));
        read(socket_id, message, 256);

        //error case
        if(strcmp(STOP, message) == 0){
            printf("From server: '%s'\n",message);
            close(connection_socket_id);
            close(socket_id);
            return 0;
        }//end case
        else if(strlen(message) <= 1){
            printf("From server: '%s'\n",message);
            close(connection_socket_id);
            close(socket_id);
            return 0;
        }
        else{//continue
            printf("From server: '%s'\n",message);
        }
    }
}