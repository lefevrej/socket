#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>


int main(){

	// Configuration

	int soc, res, temp;
	soc=socket(AF_INET, SOCK_DGRAM, 0);
	if(soc!=0) printf("Error, cannot create socket");

	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(7000);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	res = bind(soc, (struct sock_addr *)& my_addr, sizeof(my_addr));
	if(res==-1) printf("Error, cannot bind");

	// Acquisition

	struct sockaddr_in addr_trans;

	recvfrom(soc, &temp, sizeof(temp), 0, (struct sock_addr *)& addr_trans, sizeof(addr_trans));
}
