#include <sys/select.h>
#include <stddef.h>

int main(){
	int s1, s2;
	fd_set E1;
	FD_ZERO(&E1);
	FD_SET(s1, &E1);
	FD_SET(s2, &E1);
	select(max(s1,s2), &E1, NULL, NULL, 0); //bloquant
	// is s1 in E1 ?
	if(FD_ISSET(s1, &E1))
		recvfrom(s1,...);
	if(FD_ISSET(s2, &E1))
		recvfrom(s2,...);
	return 0;
}
