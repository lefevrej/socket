#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char** argv){

	if(argc!=2){
	  printf("Usage : %s server_port\n", argv[0]);
	  return 0;
	}
	
	unsigned short server_port;
	sscanf(argv[1], "%hu", &server_port);	
	
	printf("Begin server\n");
	printf("--------------------------------------\n");
	// Configuration
	int soc, res, client_pid, send;
	char data[20];

	soc=socket(AF_INET, SOCK_DGRAM, 0);
	if(soc<=0) perror("Error, cannot create socket\n");

	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(server_port);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	res = bind(soc, (struct sockaddr *)& my_addr, sizeof(my_addr));
	if(res==-1) perror("bind");

	// PID and message acquisition
	struct sockaddr_in addr_client;
	unsigned int addr_size = sizeof(addr_client);	
	recvfrom(soc, data, sizeof(data), 0, (struct sockaddr *)& addr_client, &addr_size);
	printf("Message received: %s\n", data);
	
	recvfrom(soc, &client_pid, sizeof(client_pid), 0, (struct sockaddr *)& addr_client, &addr_size);
	printf("Client pid: %u\n", client_pid);
	
	// PID and message send
	send=sendto(soc, data, sizeof(data), 0, (struct sockaddr *)& addr_client, sizeof(addr_client));
	if(send<=0)  perror("Error, cannot process send\n");
	
	send=sendto(soc, &client_pid, sizeof(client_pid), 0, (struct sockaddr *)& addr_client, sizeof(addr_client));
	if(send<=0)  perror("Error, cannot process send\n");
	
	printf("Stop server\n");
	printf("--------------------------------------\n");
	return 0;
}
