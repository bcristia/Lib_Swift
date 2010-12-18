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
	Swift s = socketSwift();
	struct sockaddr_in si_other;
	socklen_t slen = sizeof(si_other);
	
	char buf[100];
	
	recvfromSwift(s,buf,100,0,(struct sockaddr *)&si_other,&slen);
	
	printf("Received packet from %s:%d\nData: %s\n\n", 
		inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
	
	return 0;
}
