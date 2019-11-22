**Configuration**

*Machine A*

'''
int soc, res;
soc=socket(AF_INET, SOCK_DGRAM, 0);
if(soc!=0){...};
struct sock_addr_in myAddr;
myAddr.sin_family = AF_INET;
myAddr.sin_port = htons(6500);
myAddr.sin_addr.s_addr = IN_ADDR_ANY;
res = bind(soc, (struct sock_addr *)& myAddr, sizeof(myAddr));
if(res==-1){...};
'''

*Machine B*

'''
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

**Acquisition**

*Machine A*

--etiquette de B

```
struct sock_addr_in add_dest;
add_dest.sin_port = htons(7000);
add_dest.sin_addr_s_add = @IP;
add_dest.sin_family = AF_INET;


int temp;
temp = Temp();
sendto(res, &temp, sizeof(temp), 0, (struct sock_addr *)& addr_dest, sizeof(add_dest));
```

*Machine B*

``̀ 
int temp;

struct sock_addr_in add_emet;
add_emet.sin_port = htons(7000);
add_emet.sin_addr_s_add = @IP;
add_emet.sin_family = AF_INET;

recv_from(soc, &temp, sizeof(temp), 0, (struct sock_addr *) addr_emet);
```
