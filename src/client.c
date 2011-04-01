#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "lib_swift.h"

int main()
{
	Swift s = socketSwift(1);
	struct sockSwiftaddr my_addr;
	char buf[100];
	
	// populate sockSwiftaddr
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SWIFT_PORT);
	my_addr.sin_addr.N = 1;	
	my_addr.sin_addr.s_addr[0] = htonl(INADDR_LOOPBACK);	
	
	recvfromSwift(s, buf, 100, 0, &my_addr, sizeof(my_addr));
	
	struct listsockaddr lsa =  transformFromSwiftToAddr(my_addr);
	printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(lsa.sa[0].sin_addr), ntohs(my_addr.sin_port), buf);
	
	closeSwift(s);
	return 0;	
}
