#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char** argv){

	if(argc!=4){
	  printf("Usage : %s server_name server_port file_name\n", argv[0]);
	  return 0;
	}
	
	int soc, res, rec;

	char server_name[20], file_name[15], request[512], response[4000];
	unsigned short server_port;
	
	// retrieve server_name, server_port, and the requested file name
	sscanf(argv[1], "%s", server_name);
	sscanf(argv[2], "%hu", &server_port);
	sscanf(argv[3], "%s", file_name);
	
	snprintf(request, 512,"GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", file_name, server_name);
	printf("%s", request);
	// etiquette de la machine B
	struct sockaddr_in addr_server;
	addr_server.sin_port = htons(server_port);
	addr_server.sin_family = AF_INET;

	// recuperation @IP
	struct hostent* hp;
	hp=gethostbyname(server_name);
	if(hp==NULL){
		printf("Erreur, cannot resolve host");
		return 0;
	}
	memcpy(&addr_server.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);

	soc=socket(AF_INET, SOCK_STREAM, 0);
	if(soc<0){perror("Erreur, cannot create socket");}
	
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(6500);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	
 	res=bind(soc, (struct sockaddr *)& my_addr, sizeof(my_addr));
	if(res==-1) perror("Error, cannot bind");

	rec = connect(soc, (struct sockaddr *)& addr_server, sizeof(addr_server));
	if(rec==-1) perror("Error, cannot connect");
	write(soc, request, sizeof(request));
	int i = 10;
	while(i>0){
	  	i--;
		read(soc, response, 4000);
		printf("%s",response);
	}
	close(soc);
}
