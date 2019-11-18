// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#define empty "No File."
#define PORT 6969

#include "tigers.h"


int bytesRead;
int byesToRead;
char fileBuffer[1024] = {0};
char* files_folder = "/home/bxc4566/datacomm/datacomm project 1/";

void clear_buffer(char* buffer){
    for(int i = 0; i < 1024; i++)
    {
        buffer[i] = '\0';
    }
}


int main(int argc, char const *argv[]) 
{ 
    int sock = 0;
    int bytesReceived = 0;
    char final_path[1024] = {0};
    char recvBuff[256];
    //int valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char user_input[1024] = {0};
    char server_response[1024] = {0};
    char file_name[1024] = {0};
    FILE *file_pointer;
    //char input_list[10][20];
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    }
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    printf("%s\n",buffer);
    printf("\nConnection Successful \n");
    printf("%s\n",buffer);
    printf("\nWelcome to TigerS, what would you like to do today? \n"
           "Your options are: tconnect, tget and tput.\n");
    while(fgets(user_input, MAX_BUFFER,stdin))
    {
        printf("%s\n",buffer);
        printf("Input received, processing.\n");
        write(sock, user_input, strlen(user_input));
        printf("%s\n",buffer);
        printf("\nRequest Sent \n");
        printf("%s\n",buffer);
        recv(sock, server_response, strlen(server_response),0);
        printf("Processing server response.\n");
        if(strncmp(server_response,"tput", sizeof(buffer)) == 0){
            recv(sock,server_response,strlen(server_response),0);
            strcat(final_path,files_folder);
            strcat(final_path,server_response);
            FILE *download_file = fopen(final_path,"rb");
            while (1) {
                unsigned char buff[256]={0};
                int nread = fread(buff,1,256,download_file);
                if(nread > 0){
                    printf("Sending \n");
                    write(socket, buff, nread);
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
        if(strncmp(server_response,"tget", sizeof(buffer)) ==0){
            recv(sock, server_response, strlen(server_response),0);
            strcat(file_name,files_folder);
            strcat(file_name,server_response);
            file_pointer =fopen(file_name,"ab");
            if(NULL == file_pointer){
                printf("Error opening file");
                return 1;
            }
            while((bytesReceived = read(sock,recvBuff,256)) > 0){
                printf("Bytes received %d\n",bytesReceived);
                fwrite(recvBuff,1,bytesReceived,file_pointer);
            }
            if(bytesReceived < 0)
            {
                printf("\n Read Error \n");
            }
        }
        if(strncmp(server_response,"exit", sizeof(buffer))==0){
            break;
        }
        printf("\nPlease input another command. \n");
    }
    printf("%s\n",buffer);
    printf("\nClient Closed, thank you for using TigerS \n");
    return 0; 
} 