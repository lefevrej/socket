#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>


int main(){

	// Configuration
	char server_name[10], data[20];
	unsigned short server_port;
	//unsigned short server_port;
	printf("Server name: ");
	scanf("%s", server_name);
	printf("Server port: ");
	scanf("%hu", &server_port);
	printf("Message: ");
	scanf("%s", data);	
	
	// PID acquisition
	pid_t pid = getpid();

	printf("Begin client\n");
	printf("--------------------------------------\n");
	int soc, res, send;
	soc=socket(AF_INET, SOCK_DGRAM, 0);
	if(soc<=0) perror("Error, cannot create socket");

	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(6500);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	res = bind(soc, (struct sockaddr *)& my_addr,sizeof(my_addr));
	if(res==-1) perror("Error, cannot bind\n");

	// Send

	// Etiquette de la machine B
	struct sockaddr_in addr_server;
	addr_server.sin_port = htons(server_port);
	addr_server.sin_family = AF_INET;

	unsigned int addr_size = sizeof(addr_server);

	// recuperation @IP
	struct hostent* hp;
	hp=gethostbyname(server_name);
	memcpy(&addr_server.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);
	
	// PID and message send
	send=sendto(soc, data, sizeof(data), 0, (struct sockaddr *)& addr_server, sizeof(addr_server));
	if(send<=0)  perror("Error, cannot process send\n");
	
	send=sendto(soc, &pid, sizeof(pid), 0, (struct sockaddr *)& addr_server, sizeof(addr_server));
	if(send<=0)  perror("Error, cannot process send\n");

	// PID and message acquisition
	recvfrom(soc, data, sizeof(data), 0, (struct sockaddr *)& addr_server, &addr_size);
	printf("Data received: %s\n", data);
	
	recvfrom(soc, &pid, sizeof(pid), 0, (struct sockaddr *)& addr_server, &addr_size);
	printf("Data received: %u\n", pid);



	printf("Stop client\n");
	printf("--------------------------------------\n");
}