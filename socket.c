#include <stdio.h>
#include <sys/types.h>   //
#include <sys/socket.h>  // socket()  bind()
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int clientnum=0;
// 建立並繫結一個socket
int startup(const char* _ip, int _port)
{
	// 建立一個socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("----sock----fail\n");
		exit(-1);
	}

	// 繫結一個本地socket
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr("192.168.88.128");
	local.sin_port = htons(4545);

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) != 0)
	{
		perror("---bind---fail\n");
		close(sock);
		exit(-2);
	}

	if(listen(sock, 5) != 0)
	{
		perror("----listen----fail\n");
		close(sock);
		exit(-1);
	}
	
	return sock;		
}

int main(int argc, char**argv)
{


	// 獲取一個local socket
	int listen_sock = startup(argv[1], atoi(argv[2]));
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	
	char buf[1024];
	while(1)
	{
		int new_sock = accept(listen_sock,(struct sockaddr*)&client, &len);
		clientnum+=1;
		if(new_sock < 0)
		{
			perror("----accept----fail\n");
			close(listen_sock);
			exit(-5);
		}
		pid_t id = fork();
		if(id == 0)
		{
			printf("client[%d] connect !!!!\n",clientnum);
			printf("Client[%d] IP: %s\nClient[%d] Port: %d\n",clientnum,inet_ntoa(client.sin_addr),clientnum,ntohs(client.sin_port));
  	
			close(listen_sock); 
			while(1)
			{
			    ssize_t s = read(new_sock,buf, sizeof(buf)-1);
			    if(s > 0 )
			    {
			    	buf[s] = 0;
			    	printf("client[%d] say : %s \n",clientnum, buf);
			    	
			    	write(new_sock, buf, strlen(buf));
			    }
				else if( s == 0)
				{
					printf("client quit. \n");
					close(new_sock); 
					break;
				}
				else
					break;
			}
			exit(0);

		}
		else if (id > 0)
		{
			
			close(new_sock); 
			if(fork() > 0)
			{
				exit(0);
			}
		}
	}
	
	return 0;
}
