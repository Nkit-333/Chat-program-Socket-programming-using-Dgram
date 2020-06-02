#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <netdb.h>
#include <arpa/inet.h>


int main()
{
    int server_socket,new_fd,grab;
    char server_message[256];
    fd_set n;
    int ping;
    struct sockaddr_in client_address;
    socklen_t client_len,server_len;
   
    client_len = sizeof(client_address);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(3213);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int error_check = bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if(error_check == -1)
    {
        printf("Error while binding\n");
        exit(1);
    }
recvfrom(server_socket,&grab,sizeof(grab),0, (struct sockaddr *) &client_address, &client_len);    
//printf("%d",grab);

new_fd=socket(AF_INET, SOCK_DGRAM, 0);
if(new_fd==-1)
	printf("error");
bind(new_fd, (struct sockaddr *) &server_address, sizeof(server_address));
char net_buf[256]; 
while(1)
    {
     FD_ZERO(&n);
     FD_SET(0,&n);
     FD_SET(new_fd,&n);
     int ret=select(10,&n,0,0,0);
     if (ret==-1)
            {
                  printf("SELECT Error\n");
                  close(new_fd);
                  close(server_socket);
                  return 0;
            }
       else if (FD_ISSET(0,&n))
            {
                fgets(server_message, sizeof(server_message), stdin);

                
               int error_check2 = sendto(new_fd, server_message, sizeof(server_message), 0, (struct sockaddr *) &client_address, client_len);
               if(error_check2 == -1)
               {
                 printf("Error while sending message.\n");
                 exit(1);
               }    
                else if (strncmp(server_message,"bye",3)==0)
               {
                        close(new_fd);
                        close(server_socket);
                        return 0;
               }
               else if (strncmp(server_message,"audio",3)==0)
               {
                printf("\nplease enter a file name...\n"); 
                bzero(net_buf,sizeof(net_buf));
                scanf("%s", net_buf); 
                sendto(new_fd, net_buf, sizeof(net_buf), 0, (struct sockaddr *) &client_address, client_len);
               }

           }
      else if (FD_ISSET(new_fd,&n))
            {
		 
                 int error_check3 = recvfrom(new_fd,server_message, sizeof(server_message),0, (struct sockaddr *) &client_address, &client_len);
                  if(error_check3 == -1)
                  {
                        printf("Message Receive error\n");
                        close(new_fd);
                        close(server_socket);
                        exit(1);
                   }
                  else if (strncmp(server_message,"bye",3)==0)
                  {
                        close(new_fd);
                        close(server_socket);
                        return 0;
                  }
                  else if (strncmp(server_message,"audio",3)==0)
                  {
                    recvfrom(new_fd,net_buf, sizeof(net_buf),0, (struct sockaddr *) &client_address, &server_len);
                    printf("audio file recieved: %s",net_buf);
                  }
                  printf("\nMessage from Client: %s\n",server_message);      
            }
      }
    close(new_fd);
    close(server_socket);
    return 0;
}
