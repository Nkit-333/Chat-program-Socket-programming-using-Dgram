#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
int main()
{
    int client_socket,grab=10;
    int error_check;
    fd_set n;
    struct sockaddr_in client_address,server_address;
    socklen_t server_len,client_len;
    char client_message[256];

    server_len = sizeof(client_address);

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_socket == -1)
    {
        printf("Error creating socket\n");
        exit(1);
    }

    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(3213);
    client_address.sin_addr.s_addr = INADDR_ANY;
    char net_buf[256];
    sendto(client_socket,&grab, sizeof(grab),0, (struct sockaddr *) &client_address, server_len);
   while(1)
    {
    FD_ZERO(&n);
    FD_SET(0,&n);
    FD_SET(client_socket,&n);
    int ret=select(10,&n,0,0,0);
    if (ret==-1)
    {
      printf("SELECT Error\n");
      close(client_socket);
      return 0;
    }
    else if (FD_ISSET(0,&n))
    {
     
      fgets(client_message, sizeof(client_message), stdin);      
      int error_check = sendto(client_socket, client_message, sizeof(client_message),0, (struct sockaddr *) &client_address, server_len);
      if(error_check == -1)
      {
        printf("Error sending message\n");
        exit(1);
      }
      else if (strncmp(client_message,"bye",3)==0)
      {
        close(client_socket);
        return 0;
      }
      else if (strncmp(client_message,"audio",3)==0)
               {
                printf("\nplease enter a file name...\n"); 
                bzero(net_buf,sizeof(net_buf));
                scanf("%s", net_buf); 
                sendto(client_socket, net_buf, sizeof(net_buf), 0, (struct sockaddr *) &client_address, client_len);
               }
    }
    else if (FD_ISSET(client_socket,&n))
    {

    int error_check2=recvfrom(client_socket,client_message, sizeof(client_message),0, (struct sockaddr *) &client_address, &server_len);
    if(error_check2 == -1)
    {
        printf("error receiving message from client.\n");
        close(client_socket);
        exit(1);
    }
    else if (strncmp(client_message,"bye",3)==0)
      {
        close(client_socket);
        return 0;
      }
    else if (strncmp(client_message,"audio",3)==0)
    {
      recvfrom(client_socket,net_buf, sizeof(net_buf),0, (struct sockaddr *) &client_address, &server_len);
      printf("audio file recieved: %s",net_buf);
    }
      printf("\nMessage from Server: %s\n",client_message);  
    }
    }
    

    return 0;
}
