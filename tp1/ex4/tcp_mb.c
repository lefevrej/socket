#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BLOCK_SIZE 32

char* parse_request(const char* request){
	const char* start_of_path = strchr(request, '/') + 1;
    const char* end_of_path = strchr(start_of_path, ' ');  
	int path_size = end_of_path-start_of_path;

	char* path =(char*) malloc(path_size);
	strncpy(path, start_of_path, path_size);
	path[path_size]='\0';
	return path;
}

int main(int argc, char** argv){
	
	if(argc!=3){
	  printf("Usage : %s port port_log\n", argv[0]);
	  return 0;
	}
	
	// create log file
	char* log_file = "server.log";
	FILE * stream = fopen(log_file, "a+");
	if ( stream == NULL ) {
        fprintf( stderr, "Cannot open file for writing\n" );
        return -1;
	}

	int soc, stream_fd, res;
	struct sockaddr_in addr_serv, addr_client;
	char request[512];
	char* response_header = "HTTP/1.1 200 OK\r\n\r\n";
	char* error_404 = "HTTP/1.1 404 Not Found\r\n\r\n";
	char* filename;
	
	unsigned short port, log_port;
	sscanf(argv[1], "%hu", &port);
	sscanf(argv[2], "%hu", &log_port);

	addr_serv.sin_family = AF_INET;
	addr_serv.sin_port = htons(port);
	addr_serv.sin_addr.s_addr = INADDR_ANY;
	
	soc=socket(AF_INET, SOCK_STREAM, 0);
	if(soc<0){
	  perror("Error, cannot create socket");
	  return -1;
	}

	res = bind(soc, (struct sockaddr *)& addr_serv, sizeof(addr_serv));
	if(res==-1){
		perror("Error, cannot bind");
		return -1;
	}

	listen(soc, 10);

	unsigned int addr_client_size = sizeof(addr_client);
	
	printf("Waiting for connexion\n");
	stream_fd = accept(soc,(struct sockaddr *)& addr_client, &addr_client_size);
	if(stream_fd<0) perror("Error, cannot accept client");
	while(1){
	  printf("reading\n");
	  read(stream_fd, request, 512);
	  fprintf(stream,"%s\n", request);
	  fflush(0);
	  printf("parsing\n");
	  printf("Request: %s\n", request);
	  filename = parse_request(request);
	  printf("Filename: %s\n", filename); 
	  FILE* file = fopen(filename, "r");
	  if(file==NULL){
     	  perror("Error, cannot seek file");
		  write(soc, error_404, sizeof(error_404));
	  }
	  write(soc, "&", sizeof(char));
	  free(filename);
	  fclose(file);

	}
	close(soc);
	fclose(stream);
	return 0;
}
