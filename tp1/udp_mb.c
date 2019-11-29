#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>


int main(){

	// Configuration
	printf("begin\n");
	fflush(0);
	int soc, res, temp;
	soc=socket(AF_INET, SOCK_DGRAM, 0);
	if(soc<=0) perror("Error, cannot create socket\n");

	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(25765);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	res = bind(soc, (struct sockaddr *)& my_addr, sizeof(my_addr));
	if(res==-1) perror("bind");

	// Acquisition

	struct sockaddr_in addr_trans;
	uint addr_size = sizeof(addr_trans);	
	recvfrom(soc, &temp, sizeof(temp), 0, (struct sockaddr *)& addr_trans, &addr_size);
	printf("Server : %d\n", temp);
	fflush(0);
	return 0;
}
