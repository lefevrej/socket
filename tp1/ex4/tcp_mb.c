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

/*
	Parse request to find the name of the file to return
*/
char* parse_request(const char* request){
	const char* start_of_path = strchr(request, '/') + 1;
    const char* end_of_path = strchr(start_of_path, ' ');  
	int path_size = end_of_path-start_of_path;

	char* path =(char*) malloc(path_size);
	strncpy(path, start_of_path, path_size);
	// donc forget the end of file character
	path[path_size]='\0';
	return path;
}

/*
	Return current time
*/
char* current_time(time_t* rawtime){
  	struct tm* timeinfo;
	time(rawtime);
  	timeinfo = localtime(rawtime);
  	return asctime (timeinfo);
}

/*
	Create http reponse for a given file and a given header
*/
char* response_from_file(FILE* file, char* header){	
	char *body, *response;
	
	// find file length of the file
	fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    body = malloc(file_size+1);
    fread(body, 1, file_size, file);
    // donc forget the end of file character
    body[file_size]='\0';
    
    // calloc to avoid issues
   	response = calloc((file_size+strlen(header)+1), sizeof(char));
    strncpy(response, header, strlen(header));
    strcat(response, body);
    
    free(body);    
    return response;
}

int main(int argc, char** argv){
	
	if(argc!=3){
	  printf("Usage : %s port port_log\n", argv[0]);
	  return 0;
	}
	
	char* log_file_name = "server.log";
	int soc, soc_log, curr_soc, stream_fd, res;
	struct sockaddr_in addr_serv, addr_client;
	char request[512];
	char *error_404 = "HTTP/1.1 404 Not Found\r\n\r\n";
	char *code_200 = "HTTP/1.1 200 OK\r\n\r\n";
	char *filename, *response;
	fd_set fds;
  	time_t rawtime;
  	
	unsigned short port, log_port;
	unsigned int addr_client_size;
	
	// retrieve port and 	log port from args
	sscanf(argv[1], "%hu", &port);
	sscanf(argv[2], "%hu", &log_port);

	addr_serv.sin_family = AF_INET;
	addr_serv.sin_port = htons(port);
	addr_serv.sin_addr.s_addr = INADDR_ANY;
	
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
	
	// switch to log port and bind
	addr_serv.sin_port= htons(log_port);
	res = bind(soc_log, (struct sockaddr *)& addr_serv, sizeof(addr_serv));
	if(res==-1){
		perror("Error, cannot bind");
		return -1;
	}
	listen(soc, 10);
	listen(soc_log, 10);
	addr_client_size = sizeof(addr_client);
	
	while(1){		
		// initialize fd set at each iteration with the two socket file descriptors
		FD_ZERO(&fds);
		FD_SET(soc, &fds);
		FD_SET(soc_log, &fds);
		
	  	printf("Waiting for connexion...\n");
	  
	  	if(select(FD_SETSIZE, &fds, NULL, NULL, NULL)<0){	
			perror("Error, cannot select socket");
			continue;
	  	}
	  		  
	  	// assign current selected socket file descriptor to 'curr_fd'
		curr_soc = FD_ISSET(soc, &fds) ? soc : soc_log;
		printf("\nConnection...\n");
  		
  		stream_fd = accept(curr_soc,(struct sockaddr *)& addr_client, &addr_client_size);
  		if(stream_fd<0){
  		 	perror("Error, cannot accept client");
  			continue;
  		}
  		  		
	  	// Read the request and parse the filename
	  	printf("Reading\n");
	  	read(stream_fd, request, 512);
	  	printf("Parsing\n");
	  	filename = parse_request(request);
	    
		FILE* log_file = fopen(log_file_name, "a+");
		if ( log_file == NULL ) {
		 	fprintf( stderr, "Cannot open file for writing\n" );
			continue;
		}	
		printf("Filename: %s\n", filename);
  		FILE *file;
		// if the current socket is soc_log we just want to return the log file
		if(curr_soc==soc_log)
	  		file = fopen(log_file_name, "r");
	  	else{
			file = fopen(filename, "r");
			fprintf(log_file,"%sFilename: %s\nClient address: %u\n\n", 
			current_time(&rawtime), filename, addr_client.sin_addr.s_addr);
		fclose(log_file);
	  	

  	    	free(filename);
		}
	  	if(file==NULL){
   	  		perror("Error, cannot find file");
		  	file = fopen("404.html", "r");
		  	response = response_from_file(file, error_404);
	 	}else	response = response_from_file(file, code_200);
	 	
        write(stream_fd, response, strlen(response)*sizeof(char));	   
	    free(response);
        fclose(file);
        close(stream_fd);
	}
	close(soc);
	return 0;
}
