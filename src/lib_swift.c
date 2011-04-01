#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define DEBUG
#include "lib_swift.h"

struct sockSwiftaddr transformFromAddrToSwift(struct listsockaddr lsa)
{
	struct sockSwiftaddr ssa;
	int i;
	
	for (i = 0; i < lsa.N; i++)
	{
		ssa.sin_addr.s_addr[i] = lsa.sa[i].sin_addr.s_addr;
	}
	
	return ssa;
}

struct listsockaddr transformFromSwiftToAddr(struct sockSwiftaddr ssa)
{
	struct listsockaddr lsa;
	int i;
	
	lsa.N = ssa.sin_addr.N;
	
	for ( i = 0; i < lsa.N; i++) {
		lsa.sa[i].sin_family = ssa.sin_family;
		lsa.sa[i].sin_port = ssa.sin_port;
		lsa.sa[i].sin_addr.s_addr = ssa.sin_addr.s_addr[i];
	}
	
	return lsa;
}

// Function to receive a message
ssize_t recvfromSwift(Swift s, void *buf, size_t len, int flags,
					  struct sockSwiftaddr *from, socklen_t fromlen)
{
	struct sockaddr s_other;
	socklen_t slen=sizeof(s_other);	
	ssize_t rec = -1, send;
	char *command = "test";
	struct listsockaddr lsa =  transformFromSwiftToAddr(*from);
	int i, channel;
	
	Dprintf("create recv channel\n");	
	// TODO make pool
	if (s->usedChannels < s->maxChannels) 
	{
		channel = s->usedChannels++;
		CHECK(s->recvChannel[channel] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));

		for ( i = 0 ; i < lsa.N ; i++) 
		{
			Dprintf("send information to %s\n", inet_ntoa(lsa.sa[0].sin_addr));			
			send = sendto(s->recvChannel[channel], command, strlen(command), 0, (const struct sockaddr *)&lsa.sa[i], sizeof(lsa.sa[i]));
		}
		
		Dprintf("receive data\n");
		// I'm waiting for response
		rec = recvfrom(s->recvChannel[channel], buf, len, flags,  (struct sockaddr * __restrict__)&s_other, &slen);		
	
		close(s->recvChannel[channel]);
		s->usedChannels--;
	}
	return rec;
}

// Function to send a message
ssize_t sendToSwift(Swift s, const void *buf, size_t len, int flags, 
					const struct sockSwiftaddr *to, socklen_t tolen) 
{
	struct listsockaddr lsa =  transformFromSwiftToAddr(*to);
	int i;
	ssize_t send = -1;
	
	Dprintf("send data\n");
	for ( i = 0 ; i < lsa.N ; i++) 
	{
		send = sendto(s->sendChannel, buf, len, flags, (const struct sockaddr *)&lsa.sa[i], sizeof(lsa.sa[i]));
	}
	
	return send;
}
// Function to listen to a port
int listenfromSwift (Swift s, void *buf, size_t len, int flags,
					 struct sockSwiftaddr * __restrict__ from, socklen_t *fromlen) 
{
	struct sockaddr s_other;
	struct listsockaddr lsa;
	socklen_t slen=sizeof(s_other);	
	ssize_t rec;
	
	Dprintf("wait to receive messages\n");
	
	rec = recvfrom(s->socketListener, buf, len, flags,  (struct sockaddr * __restrict__)&s_other, &slen);
	
	// fill listsockaddr
	memcpy(&lsa.sa[0], &s_other, sizeof(s_other));
	lsa.N = 1;
	
	*from = transformFromAddrToSwift(lsa);
	
	return rec;
}

// Function to bind a port for swift socket
int bindSwift(Swift s, const struct sockSwiftaddr *my_addr, socklen_t addrlen)
{
	Dprintf("bind swift socket\n");
	struct listsockaddr lsa = transformFromSwiftToAddr(*my_addr);
		
	return bind(s->socketListener, (const struct sockaddr *)&lsa.sa[0], sizeof(lsa.sa[0]));
}

// Function to create a Swift socket
Swift socketSwift(int maxChannels)
{
	Dprintf("create struct swift\n");	
	Swift s = calloc(1,sizeof(*s));

	Dprintf("create swift socket listener\n");	
	CHECK(s->socketListener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));

	Dprintf("create swift send channel\n");	
	CHECK(s->sendChannel = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));

	return s;
}


void closeSwift(Swift s)
{
	Dprintf("close swift socket\n");
	close(s->socketListener);
	close(s->sendChannel);
	
	
	free(s);
}


