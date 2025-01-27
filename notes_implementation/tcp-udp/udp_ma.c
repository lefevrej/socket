#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int Temp(){return 5;}


int main(){

	// Configuration

	int soc, res, temp, rec;
	temp = Temp();
	soc=socket(AF_INET, SOCK_DGRAM, 0);
	if(soc<=0) perror("Error, cannot create socket");

	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(6000);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	res = bind(soc, (struct sockaddr *)& my_addr, sizeof(my_addr));
	if(res==-1) perror("Error, cannot bind");

	// Send

	// Etiquette de la machine B
	struct sockaddr_in addr_dest;
	addr_dest.sin_port = htons(7000);
	addr_dest.sin_family = AF_INET;

	// recuperation @IP
	char* machine_name="pc5201a.lan.esiee.fr";
	struct hostent* hp;
	hp=gethostbyname(machine_name);
	memcpy(&addr_dest.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);

	rec = sendto(res, &temp, sizeof(temp), 0, (struct sockaddr *)& addr_dest, sizeof(addr_dest));
	if(rec==-1) perror("Error, cannot send");
}
