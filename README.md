#UDP

UDP est un protocole de communication qui permet la transmission de donnée entre deux entités définies par une @IP et un numéro de port. 

## Configuration

### Machine A

```
int soc, res;
soc=socket(AF_INET, SOCK_DGRAM, 0);
if(soc!=0){...};

struct sock_addr_in myAddr;
myAddr.sin_family = AF_INET;
myAddr.sin_port = htons(6500);
myAddr.sin_addr.s_addr = IN_ADDR_ANY;

res = bind(soc, (struct sock_addr *)& myAddr, sizeof(myAddr));
if(res==-1){...};
```

### Machine B


```
int soc, res;
soc=socket(AF_INET, SOCK_DGRAM, 0);
if(soc!=0){...};

struct sock_addr_in myAddr;
myAddr.sin_family = AF_INET;
myAddr.sin_port = htons(7000);
myAddr.sin_addr.s_addr = IN_ADDR_ANY;

res = bind(soc, (struct sock_addr *)& myAddr, sizeof(myAddr));
if(res==-1){...};
```

# Acquisition

## Machine A

*Etiquette de B*

```
struct sock_addr_in add_dest;
add_dest.sin_port = htons(7000);
add_dest.sin_addr_s_add = @IP;
add_dest.sin_family = AF_INET;
```

*Envoie des données*

```
int temp;
temp = Temp();
sendto(res, &temp, sizeof(temp), 0, (struct sock_addr *)& addr_dest, sizeof(add_dest));
```

## Machine B

```
int temp;

struct sock_addr_in addr_emet;

recv_from(soc, &temp, sizeof(temp), 0, (struct sock_addr *) addr_emet);
```

#TCP

A l'instar de l'UDP, TCP est un protocole de communication qui à les mêmes spécifités qu'UDP sauf qu'il fonction en mode *connecté*. Il faut donc dabord ammorcer une connexion. Le contrôl d'erreur est aussi introduit.


## Configuration

### Machine A

Pour se connecter au serveur il faut toujours son port et son addresse @IP;

```
int soc, res, temp;
temp=Temp();

struct sock_addr_in add_server;
add_emet.sin_port = htons(7000);
add_emet.sin_addr_s_add = @IP;
add_emet.sin_family = AF_INET;

soc=socket(AF_INET, SOCK_STREAM, 0);
if(soc!=0){...};
connect(soc, (struct sock_addr *)& addr_server, sizeof(addr_server));
write(soc, &temp, sizeof(temp));
close(soc);
```

### Machine B

Pour tester pusieurs client il faut fork le processus.

```
int soc, res;
soc=socket(AF_INET, SOCK_STREAM, 0);
if(soc!=0){...};

struct sock_addr_in myAddr;
myAddr.sin_family = AF_INET;
myAddr.sin_port = htons(7000);
myAddr.sin_addr.s_addr = IN_ADDR_ANY;

listen(soc, 10); // -> file d attente de demande de connexion
s2 = accept(soc, ...);
read(s2, &temp, sizeof(temp));
print("Temp = %d", temp);
```

