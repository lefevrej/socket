#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int Temp(){return 5;}

int main(){
	int soc, temp, res, rec;
	temp=Temp(); 

	// etiquette de la machine B
	struct sockaddr_in addr_server;
	addr_server.sin_port = htons(7000);
	addr_server.sin_family = AF_INET;

	// recuperation @IP
	char* machine_name="pc5201a.lan.esiee.fr";
	struct hostent* hp;
	hp=gethostbyname(machine_name);
	memcpy(&addr_server.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);

	soc=socket(AF_INET, SOCK_STREAM, 0);
	if(soc!=0){perror("Erreur");}
	
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(6500);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	
 	res=bind(soc, (struct sockaddr *)& my_addr, sizeof(my_addr));
	if(res==-1) perror("Error, cannot bind");

	rec = connect(soc, (struct sockaddr *)& addr_server, sizeof(addr_server));
	if(rec==-1) perror("Error, cannot connect");
	write(soc, &temp, sizeof(temp));
	close(soc);
}
