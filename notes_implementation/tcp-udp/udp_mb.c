#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>


int main(){

	// Configuration

	int soc, res, temp, rec;
	soc=socket(AF_INET, SOCK_DGRAM, 0);
	if(soc<=0) perror("Error, cannot create socket");

	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(7000);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	res = bind(soc, (struct sockaddr *)& my_addr, sizeof(my_addr));
	if(res==-1) perror("Error, cannot bind");

	// Acquisition

	struct sockaddr_in addr_client;
	unsigned int addr_client_size = sizeof(addr_client);
	rec = recvfrom(soc, &temp, sizeof(temp), 0, (struct sockaddr *)& addr_client, &addr_client_size);
	if(rec==-1) perror("Error, cannot receive");
}

