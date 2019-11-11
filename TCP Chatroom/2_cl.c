#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

char msg[500];

void *recvmg(void *my_sock)
{
    int sock = *((int *)my_sock);
    int len;
    // client thread always ready to receive message
    while((len = recv(sock,msg,500,0)) > 0) {
        msg[len] = '\0';
        fputs(msg,stdout);
    }
}

int main(){
    pthread_t recvt;
    int len;
    int sock;
    char send_msg[50] = {'\0'};
    struct sockaddr_in ServerIp;
    char client_name[100];
	// strcpy(client_name, argv[1]);
    sock = socket( AF_INET, SOCK_STREAM,0);
    ServerIp.sin_port = htons(1234);
    ServerIp.sin_family= AF_INET;
    ServerIp.sin_addr.s_addr = inet_addr("192.168.43.207");
    if( (connect( sock ,(struct sockaddr *)&ServerIp,sizeof(ServerIp))) == -1 )
        printf("\n connection to socket failed \n");
	
	recv(sock, send_msg, 50, 0);
	printf("Recev:  %s", send_msg);
       memset(send_msg, '\0', 50);
       fgets(send_msg,50,stdin);
       send(sock,send_msg,strlen(send_msg), 0);
	recv(sock, send_msg, 50, 0);
	printf("Recev:  %s", send_msg);
    //creating a client thread which is always waiting for a message
    pthread_create(&recvt,NULL,(void *)recvmg,&sock);
	
    //ready to read a message from console
    while(fgets(send_msg,50,stdin) > 0) 
    {
      /*  strcpy(send_msg,client_name);
        strcat(send_msg,":");
        strcat(send_msg,msg);
        */
       printf("Send mess: %s", send_msg);
      // len = write(sock,send_msg,strlen(send_msg));
       len = send(sock,send_msg,strlen(send_msg), 0);
       memset(send_msg, '\0', 50);
        if(len < 0) 
            printf("\n message not sent \n");
            
    }

    //thread is closed
    pthread_join(recvt,NULL);
    close(sock);
    return 0;
}
