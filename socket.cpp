#include<winsock2.h>
#include<stdio.h>

#pragma comment(lib,"ws2_32.lib")
SOCKET server_sockfd, client_sockfd[1024];
int server_len;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
int c_len=sizeof(client_address);
int count=0;
char buffer[1024], buffer2[1024];
HANDLE hThread[1024];
void trans(int num)
{
	int pos;
	while(1){
	
		memset(buffer,0,sizeof(buffer)) ;
		memset(buffer2,0,sizeof(buffer2));
	    char *ch;
	  	if(client_sockfd[count] <0) {
      	printf("Accept Error\n");
	}
	    pos=recv(client_sockfd[num], buffer, 1023, 0);  
		if(pos==0)
		{
			printf("Client  disconnected!!\n");
			printf("Client Socket %d is closed!!\n",num);
			break;
		}
		if(pos>0)
		printf("Client[%d]  send: %s!!\n",num,buffer);
		else
		printf("Client[%d]  send error!!\n",num);
	    strcpy(buffer2, buffer);
	    send(client_sockfd[num], buffer2, strlen(buffer2), 0);
	    if(strcmp(buffer2, "close")==0)
	    {
	    	closesocket(client_sockfd[num]); 
	    	printf("Client  choose to  disconnected!!\n");
	    	printf("Client Socket %d is closed!!\n",num);
	    	break;
		}
	}
	CloseHandle(hThread[num]);
}



int main() {


  WSADATA wsadata;
  if(WSAStartup(0x101,(LPWSADATA)&wsadata) != 0) {
    printf("Winsock Error\n");
    exit(1);                                         
  }
 
  server_sockfd = socket(AF_INET, SOCK_STREAM, 0); 
  if(server_sockfd == SOCKET_ERROR) {
    printf("Socket Error\n");
    exit(1);
  } 
  server_address.sin_family = AF_INET; 
  server_address.sin_addr.s_addr = inet_addr("192.168.1.152");  
  server_address.sin_port = htons(1234);  
  server_len = sizeof(server_address);
 
  if(bind(server_sockfd, (struct sockaddr *)&server_address, server_len) < 0) {
    printf("Bind Error\n");
    exit(1);
  }

  if(listen(server_sockfd, 5) < 0) {
    printf("Listen Error\n");
    exit(1);
  }
 	printf("Server waiting...\n");
    
  while(1) {
  	
    client_sockfd[count] = accept(server_sockfd, (struct sockaddr*)&client_address, &c_len);
	if(client_sockfd[count] == SOCKET_ERROR) {
      printf("Accept Error\n");
      exit(1);
    } 
	printf("Client[%d]  connected!!\n",count);
	printf("Client[%d] IP: %s\nClient[%d] Port: %d\n",count,inet_ntoa(client_address.sin_addr),count,ntohs(client_address.sin_port));
  	
	hThread[count]=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)trans, count, 0, NULL);
	count++;
  }
  return 0;
}
