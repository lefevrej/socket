#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(){
	int soc, temp, stream_fd;
	struct sockaddr_in addr_client;

	soc=socket(AF_INET, SOCK_STREAM, 0);
	if(soc!=0) printf("Error, cannot create socket");

	listen(soc, 10);
	stream_fd = accept(soc,(struct sock_addr *)& addr_client, sizeof(addr_client));
	if(stream_fd<0) printf("Error, cannot accept client");
	read(stream_fd, &temp, sizeof(temp));
	close(soc);
}
