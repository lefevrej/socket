#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/select.h>

char* parse_request(const char* request){
	const char* start_of_path = strchr(request, '/') + 1;
    const char* end_of_path = strchr(start_of_path, ' ');  
	int path_size = end_of_path-start_of_path;

	char* path =(char*) malloc(path_size);
	strncpy(path, start_of_path, path_size);
	path[path_size]='\0';
	return path;
}

char* current_time(time_t* rawtime){
  	struct tm* timeinfo;
	time(rawtime);
  	timeinfo = localtime(rawtime);
  	return asctime (timeinfo);
}

int main(int argc, char** argv){
	
	if(argc!=3){
	  printf("Usage : %s port port_log\n", argv[0]);
	  return 0;
	}
	
	// create log file
	char* log_file_name = "server.log";
	FILE* log_file = fopen(log_file_name, "a+");
	if ( log_file == NULL ) {
        fprintf( stderr, "Cannot open file for writing\n" );
        return -1;
	}

	int soc, soc_log, stream_fd, res;
	struct sockaddr_in addr_serv, addr_client;
	char request[512];
	char* response_header = "HTTP/1.1 200 OK\r\n\r\n";
	char* error_404 = "HTTP/1.1 404 Not Found\r\n\r\n";
	char* filename;
	char* response;
	char* body;
	fd_set fds;

	unsigned short port, log_port;
	sscanf(argv[1], "%hu", &port);
	sscanf(argv[2], "%hu", &log_port);

	addr_serv.sin_family = AF_INET;
	addr_serv.sin_port = htons(port);
	addr_serv.sin_addr.s_addr = INADDR_ANY;
	
  	time_t rawtime;


	soc=socket(AF_INET, SOCK_STREAM, 0);
	if(soc<0){
	  perror("Error, cannot create http socket");
	  return -1;
	}
	soc_log=socket(AF_INET, SOCK_STREAM, 0);
	if(soc_log<0){
	  perror("Error, cannot create log socket");
	  return -1;
	}

	res = bind(soc, (struct sockaddr *)& addr_serv, sizeof(addr_serv));
	if(res==-1){
		perror("Error, cannot bind");
		return -1;
	}

	addr_serv.sin_port= htons(log_port);
	res = bind(soc_log, (struct sockaddr *)& addr_serv, sizeof(addr_serv));
	if(res==-1){
		perror("Error, cannot bind");
		return -1;
	}
	listen(soc, 10);
	listen(soc_log, 10);
	unsigned int addr_client_size = sizeof(addr_client);
	
	FD_ZERO(&fds);
	FD_SET(soc, &fds);
	FD_SET(soc_log, &fds);
	
	while(1){	
	  printf("Waiting for connexion...\n");
	  
      
	  printf("Waiting for request...\n");
	  if(select(FD_SETSIZE, &fds, NULL, NULL, NULL)<0){	
		perror("Error, cannot select socket");
		return -1;
	  }
	  perror("1");
	  if(FD_ISSET(soc, &fds)){
	  	stream_fd = accept(soc,(struct sockaddr *)& addr_client, &addr_client_size);
	  	if(stream_fd<0) perror("Error, cannot accept client");
	  
	  	// Read the request and parse the filename
	  	printf("Reading\n");
	  	read(stream_fd, request, 512);
	  	printf("Parsing\n");
	  	filename = parse_request(request);
  	  	fprintf(log_file,"%s\n%s\n", current_time(&rawtime), request);
	  	fflush(log_file);
	  	printf("Filename: %s\n", filename); 
		
	  	FILE* file = fopen(filename, "r");
	  	if(file==NULL){
     		perror("Error, cannot find file");
		  	write(stream_fd, error_404, (strlen(error_404)+1)*sizeof(char));
	 	}else{
		
	    	// find file length and send content
	    	fseek(file, 0, SEEK_END);
		    long file_size = ftell(file);
		    fseek(file, 0, SEEK_SET);
		    body = malloc(file_size+1);
		    fread(body, 1, file_size, file);
		    body[file_size]='\0';

		    response = malloc((file_size+strlen(response_header)+1)*sizeof(char));
		    strncpy(response, response_header, strlen(response_header));
		    strcat(response, body);
		    write(stream_fd, response, strlen(response));
		   	free(body);
		    fclose(file);
		  }
		  close(stream_fd);
		  free(filename);
		}
	}
	free(response); // issue idk why
	fclose(log_file);
	close(soc);
	return 0;
}
