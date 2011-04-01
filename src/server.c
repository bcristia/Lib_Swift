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
	struct sockSwiftaddr my_addr, from;
	char buf[100];
	socklen_t slen;
	struct listsockaddr lsa;
	
	// populate sockSwiftaddr
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SWIFT_PORT);
	my_addr.sin_addr.N = 1;	
	my_addr.sin_addr.s_addr[0] = htonl(INADDR_ANY);	
	
	//recvfromSwift(s, buf, 100, 0, (struct sockaddr *)&si_other, &slen);
	
	bindSwift(s, &my_addr, sizeof(my_addr));
	listenfromSwift(s, buf, 100, 0, &from, &slen);
	
	lsa =  transformFromSwiftToAddr(from);
	printf("Received packet from %s:%d with data: %s\n", inet_ntoa(lsa.sa[0].sin_addr), ntohs(lsa.sa[0].sin_port), buf);
	
	
	closeSwift(s);
	return 0;
}
