//Code skeleton was taken from discussion sections slides. I used
//geekforgeeks, stack overflow, and linux man pages to find out
//what functions i need to use to complete the homework assignment and to 
//error handle. 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
int main (int argc, char * argv[]) {
    //check arguments
    if(argc < 2 || argc > 2){
        printf("Error : Input Error \n");
        exit(EXIT_FAILURE); 
    }
    //initialization of variables
    char response[256];
    int port = atoi(argv[1]), size = 0;
    int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    //bind to socket
    bind(socket_id, (struct sockaddr *) &server_address, sizeof(server_address));
    char message[256];
    struct sockaddr_in client_address;
    unsigned int return_len;
    char sum[20];
    //while loop that keeps listening for messages to socket
    while(1){
        //receive message on cleared buffers
        memset(message, 0 , sizeof(message));
        memset(response, 0 , sizeof(response));
        size = recvfrom( socket_id, message, 256, 0,
        (struct sockaddr *) &client_address, &return_len );
        message[size] = '\0';
        //add terminating character to string

        //calculate message and send each iteration
        int total = 0;
        for(int i = 0; message[i] != '\0'; i++){
            int x = message[i] + 0 - 48;
            if(x >= 0 && x <= 9){total = total + message[i] - 48;}
            else{
                char temp[100] = "Sorry, cannot compute!";
                strcat(response, temp);
                printf("%s" , response);
                sendto(socket_id, response, strlen(response), 0, (struct sockaddr *) &client_address, sizeof(struct sockaddr_in));
            }  
        }
        //send response to client
        memset(sum, 0, sizeof(sum));
        sprintf(sum, "%d", total);
        strcat(response, sum);
        printf("%s" , response);
        sendto(socket_id, response, strlen(response), 0, (struct sockaddr *) &client_address, sizeof(struct sockaddr_in));
    }
    close(socket_id);
}