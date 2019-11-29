#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(){
	int soc, temp, stream_fd, res;
	struct sockaddr_in addr_serv, addr_client;

	addr_serv.sin_family = AF_INET;
	addr_serv.sin_port = htons(7000);
	addr_serv.sin_addr.s_addr = INADDR_ANY;
	
	soc=socket(AF_INET, SOCK_STREAM, 0);
	if(soc!=0) printf("Error, cannot create socket");

	res = bind(soc, (struct sock_addr *)& addr_serv, sizeof(addr_serv));
	if(res==-1) printf("Error, cannot bind");

	listen(soc, 10);
	stream_fd = accept(soc,(struct sock_addr *)& addr_client, sizeof(addr_client));
	if(stream_fd<0) printf("Error, cannot accept client");
	read(stream_fd, &temp, sizeof(temp));
	close(soc);
}
