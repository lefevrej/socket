# Sockets

## UDP

UDP est un protocole de communication qui permet la transmission de donnée entre deux entités définies par une @IP et un numéro de port. 

### Configuration

#### Machine A

```
int soc, res, temp;
temp = Temp();
soc=socket(AF_INET, SOCK_DGRAM, 0);
if(soc!=0) printf("Error, cannot create socket");

struct sockaddr_in my_addr;
my_addr.sin_family = AF_INET;
my_addr.sin_port = htons(6500);
my_addr.sin_addr.s_addr = INADDR_ANY;

res = bind(soc, (struct sock_addr *)& my_addr, sizeof(my_addr));
if(res==-1) printf("Error, cannot bind");
```

#### Machine B


```
int soc, res, temp;
soc=socket(AF_INET, SOCK_DGRAM, 0);
if(soc!=0) printf("Error, cannot create socket");

struct sockaddr_in my_addr;
my_addr.sin_family = AF_INET;
my_addr.sin_port = htons(7000);
my_addr.sin_addr.s_addr = INADDR_ANY;

res = bind(soc, (struct sock_addr *)& my_addr, sizeof(my_addr));
if(res==-1) printf("Error, cannot bind");
```

## Acquisition/Envoie

### Machine A

*Etiquette de B*

```
struct sockaddr_in addr_dest;
addr_dest.sin_port = htons(7000);
addr_dest.sin_family = AF_INET;

// recuperation @IP
char* machine_name="pc5201a.lan.esiee.fr";
struct hostent* hp;
hp=gethostbyname(machine_name);
memcpy(&addr_dest.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);
```

*Envoie des données*

```
sendto(res, &temp, sizeof(temp), 0, (struct sock_addr *)& addr_dest, sizeof(addr_dest));
```

### Machine B

*Réception des données*

```
struct sockaddr_in addr_trans;

recvfrom(soc, &temp, sizeof(temp), 0, (struct sock_addr *)& addr_trans, sizeof(addr_trans));
```

## TCP

A l'instar de l'UDP, TCP est un protocole de communication qui à les mêmes spécifités qu'UDP sauf qu'il fonction en mode *connecté*. Il faut donc dabord ammorcer une connexion. Le contrôl d'erreur est aussi introduit.


### Configuration

#### Machine A

Pour se connecter au serveur il faut toujours son port et son addresse @IP;

```
int soc, temp;
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
if(soc!=0){printf("Erreur");}
connect(soc, (struct sock_addr *)& addr_server, sizeof(addr_server));
write(soc, &temp, sizeof(temp));
close(soc);

```

#### Machine B

Pour tester pusieurs client il faut fork le processus.

```
int soc, temp, stream_fd;
struct sockaddr_in addr_client;

soc=socket(AF_INET, SOCK_STREAM, 0);
if(soc!=0) printf("Error, cannot create socket");

listen(soc, 10);
stream_fd = accept(soc,(struct sock_addr *)& addr_client, sizeof(addr_client));
if(stream_fd<0) printf("Error, cannot accept client");

read(stream_fd, &temp, sizeof(temp));
close(soc);
```
