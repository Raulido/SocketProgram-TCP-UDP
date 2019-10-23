//Code skeleton was taken from discussion sections slides. I used
//geekforgeeks, stack overflow, and linux man pages to find out
//what functions i need to use to complete the homework assignment and to 
//error handle. 
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
int main (int argc, char * argv[]) {
    //check for arguments
    if(argc < 2 || argc > 2){
        printf("Error : Input Error \n");
        exit(EXIT_FAILURE); 
    }

    //initialization of variables
    char response[256];
    int port = atoi(argv[1]), size = 0;
    int num_connections = 3;
    int socket_id = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    //bind to ip
    bind(socket_id, (struct sockaddr *) &server_address, sizeof(server_address));
    //listen for connections
    listen(socket_id, num_connections);
    //more initialization
    struct sockaddr_in client_address;
    unsigned int return_len;
    char message[256];
    char sum[20];
    //second socket
    int connection_socket_id = accept( socket_id,(struct sockaddr *) &client_address,
    &return_len );
    //clear buffer
    memset(message, 0 , sizeof(message));
    memset(response, 0 , sizeof(response));
    //read and size gather to add null terminator
    size = read(connection_socket_id, message, 256);
    message[size] = '\0';
    int total = 10;
    while(1){
        //calculate message mutliple times and send each iteration to client
        if(total < 10){
            listen(socket_id, num_connections);
            connection_socket_id = accept( socket_id,(struct sockaddr *) &client_address,
            &return_len );
            size = read(connection_socket_id, message, 256);
            message[size] = '\0';
        }
        total = 0;
        for(int i = 0; message[i] != '\0'; i++){
            int x = message[i] + 0 - 48;
            if(x >= 0 && x <= 9){total = total + message[i] - 48;}
            else{
                //incase a non number is in cstring, error
                char temp[100] = "Sorry, cannot compute!";
                strcat(response, temp);
                send(connection_socket_id, response, strlen(response), 0);
                close(connection_socket_id);
                
            }  
        }
        //sending part and clearing of buffers
        memset(sum, 0, sizeof(sum));
        sprintf(sum, "%d", total);
        strcat(response, sum);
        send(connection_socket_id, response, strlen(response), 0);
        memset(message, 0 , sizeof(message));
        memset(response, 0 , sizeof(response));
        strcat(message, sum);
        if(total < 10){
            //end case
            close(connection_socket_id);
            
        }
        //slow down sending to prevent jumbled buffers
        sleep(.1);
    }
    close(socket_id);
}