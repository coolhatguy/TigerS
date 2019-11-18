// Tiger Server that handles user authentication, with username creation.
// Users able to handle the action to download and upload files.
// 
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#define PORT 6969
#define TRUE 1
#define FALSE 0
#define MAX_BUFFER 120
#define MAX_FILES  100
#define MAX_USERS 101
#define MAX_LENGTH 20
#define RECEIVED "\n Data has been received."
#define INVALID "Received wrong input."
#define users_database "users_database.txt"


void clear_buffer(char* buffer){
    for(int i = 0; i < 1024; i++)
    {
        buffer[i] = '\0';
    }
}


int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket;
    FILE* file_pointer;
    char recvBuff[256];
    //int valread;
    struct sockaddr_in address;
    int bytesReceived = 0;
    char final_path[1024] = {0};
    int opt = 1; 
    int addresslength = sizeof(address);
    char buffer[1024] = {0};
    char* command1;
    char* command2;
    char* command3;
    char* files_folder = "/home/bxc4566/datacomm/datacomm project 1/files/";
    int logged;
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addresslength))<0)
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    printf("%s\n",buffer);
    printf("\nServer Successfully Launched\n");

    logged = 0;
    while(1) {
        recv(new_socket, buffer, 1024, 0);
        if (buffer == NULL){
            printf("%s\n",buffer);
            printf("\nInvalid Command Received \n");
            printf("%s\n",buffer);
            printf("\nPlease input another command. \n");
            clear_buffer(buffer);
        }
        else {
            printf("\n%s", buffer);
            command1 = strtok(buffer, " \n");
            command2 = strtok(NULL, " \n");
            command3 = strtok(NULL, " \n");
        }
        if (strncmp(command1,"tconnect", sizeof(buffer))==0) {
            printf("\nInput received.");
            printf("\n Your inputted Username is : %s",command2);
            fflush(stdout);
            printf("\n Your inputted Password is : %s",command3);
            fflush(stdout);
            printf("\nRunning Credentials.\n");
            file_pointer = fopen("/home/bxc4566/datacomm/datacomm project 1/users_database.txt","r");
            if (fscanf(file_pointer,"%s",command2) == 1){
                printf("\nUser already exists.\n");
                if(fscanf(file_pointer,"%s",command3) == 1) {
                    printf("\nPassword Correct.\n");
                    logged = 1;
                    printf("\nYou are now logged in.\n");
                    write(new_socket, buffer, strlen(buffer));
                }
            }
            else{
                fclose(file_pointer);
                fopen("/home/bxc4566/datacomm/datacomm project 1/users_database.txt","a");
                fprintf(file_pointer,"%s %s\n",command2,command3);
                printf("\nUser Created.\n");
                logged = 1;
                printf("\nYou are now logged in.\n");
                fclose(file_pointer);
                write(new_socket, buffer, strlen(buffer));
            }
            clear_buffer(buffer);
        }
        else if (strncmp(command1,"tget", sizeof(buffer))==0) {
            printf("Input received.\n");
            if(logged == 0){
                printf("You are not logged in.\n");
                write(new_socket, buffer, strlen(buffer));
            }
            else{
                if(strncmp(command2, "", sizeof(buffer)) == 0){
                    printf("No specified file.\n");
                }
                else{
                    write(new_socket,command2,strlen(command2));
                    strcat(final_path,files_folder);
                    strcat(final_path,command2);
                    FILE *download_file = fopen(final_path,"rb");
                    while (1) {
                        unsigned char buff[256]={0};
                        int nread = fread(buff,1,256,download_file);
                        if(nread > 0){
                            printf("Sending \n");
                            write(new_socket, buff, nread);
                        }
                        if(nread < 256){
                            if(feof(download_file)){
                                printf("End of file\n.");
                            }
                            if(ferror(download_file)){
                                printf("Error reading\n");
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (strncmp(command1,"tput", sizeof(buffer))==0) {
            printf("Input received.\n");
            if(logged == 0){
                printf("You are not logged in.\n");
                write(new_socket, buffer, strlen(buffer));
            }
            else{
                if(strncmp(command2, "", sizeof(buffer)) == 0){
                    printf("No specified file.\n");
                }
                else{
                    if(strncmp(command2, "", sizeof(buffer)) == 0){
                        printf("No specified file.\n");
                    }
                    else {
                        strcat(final_path,files_folder);
                        strcat(final_path,command2);
                        FILE *download_file = fopen(final_path,"rb");
                        if(NULL == file_pointer){
                            printf("Error opening file");
                            return 1;
                        }
                        while((bytesReceived = read(new_socket,recvBuff,256)) > 0){
                            printf("Bytes received %d\n",bytesReceived);
                            fwrite(recvBuff,1,bytesReceived,file_pointer);
                        }
                        if(bytesReceived < 0)
                        {
                            printf("\n Write Error \n");
                        }
                    }
                }
            }
        }
        else if (strncmp(buffer,"exit", sizeof(buffer))==0){
            write(new_socket, buffer, strlen(buffer));
            break;
        }
    }
    return 0; 
} 