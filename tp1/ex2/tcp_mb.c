#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(){
	int soc, stream_fd, res;
	struct sockaddr_in addr_serv, addr_client;
	unsigned short port;
	char data[512];

	printf("Port number: ");
	scanf("%hu", &port);

	addr_serv.sin_family = AF_INET;
	addr_serv.sin_port = htons(port);
	addr_serv.sin_addr.s_addr = INADDR_ANY;
	
	soc=socket(AF_INET, SOCK_STREAM, 0);
	if(soc<0){
	  perror("Error, cannot create socket");
	  return -1;
	}

	res = bind(soc, (struct sockaddr *)& addr_serv, sizeof(addr_serv));
	if(res==-1) perror("Error, cannot bind");

	listen(soc, 10);

	unsigned int addr_client_size = sizeof(addr_client);
	stream_fd = accept(soc,(struct sockaddr *)& addr_client, &addr_client_size);
	if(stream_fd<0) perror("Error, cannot accept client");
	while(1){
		read(stream_fd, data, 512);
		printf("%s",data);
	}
	close(soc);
}
