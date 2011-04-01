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

void transformFromAddrToSwift(struct sockSwiftaddr *ssa, struct listsockaddr lsa)
{
	int i;
	
	Dprintf("transform in %d address\n", lsa.N);	
	ssa->sin_addr.N = lsa.N;
	if (lsa.N > 0) {
		ssa->sin_family = lsa.sa[0].sin_family;
		ssa->sin_port = lsa.sa[0].sin_port;
	}
	
	for (i = 0; i < lsa.N; i++)
	{
		ssa->sin_addr.s_addr[i] = lsa.sa[i].sin_addr.s_addr;
	}
}

void transformFromSwiftToAddr(struct listsockaddr *lsa, struct sockSwiftaddr ssa)
{
	int i;
	
	Dprintf("transform %d address\n", ssa.sin_addr.N);
	lsa->N = ssa.sin_addr.N;
	
	for ( i = 0; i < lsa->N; i++) {
		lsa->sa[i].sin_family = ssa.sin_family;
		lsa->sa[i].sin_port = ssa.sin_port;
		lsa->sa[i].sin_addr.s_addr = ssa.sin_addr.s_addr[i];
	}
}

// Function to receive a message
ssize_t recvfromSwift(Swift s, void *buf, size_t len, int flags,
					  struct sockSwiftaddr *from, socklen_t fromlen)
{
	struct sockaddr s_other;
	socklen_t slen=sizeof(s_other);	
	ssize_t rec = -1, send;
	char *command = "test";
	struct listsockaddr lsa;
	int i, channel;
	
	transformFromSwiftToAddr(&lsa, *from);
	Dprintf("create recv channel\n");	
	// TODO make pool
	if (s->usedChannels < s->maxChannels) 
	{
		channel = s->usedChannels++;
		CHECK(s->recvChannel[channel] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));

		for ( i = 0 ; i < lsa.N ; i++) 
		{
			Dprintf("send information to %s:%d\n", inet_ntoa(lsa.sa[i].sin_addr), ntohs(lsa.sa[i].sin_port));			
			send = sendto(s->recvChannel[channel], command, 4, 0, (const struct sockaddr *)&lsa.sa[i], sizeof(lsa.sa[i]));
		}
		
		Dprintf("receive data\n");
		// TODO make a non-blocant select
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
	struct listsockaddr lsa;
	int i;
	ssize_t send = -1;
	
	transformFromSwiftToAddr(&lsa, *to);
	
	for ( i = 0 ; i < lsa.N ; i++) 
	{
		Dprintf("send data to %s:%d\n", inet_ntoa(lsa.sa[i].sin_addr), ntohs(lsa.sa[i].sin_port));
		send = sendto(s->sendChannel, buf, len, flags, (const struct sockaddr *)&lsa.sa[i], sizeof(lsa.sa[i]));
	}
	
	return send;
}
// Function to listen to a port
int listenfromSwift (Swift s, void *buf, size_t len, int flags,
					 struct sockSwiftaddr * from, socklen_t *fromlen) 
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
	
	transformFromAddrToSwift(from, lsa);
	
	return rec;
}

// Function to bind a port for swift socket
int bindSwift(Swift s, const struct sockSwiftaddr *my_addr, socklen_t addrlen)
{
	Dprintf("bind swift socket\n");
	struct listsockaddr lsa;
	transformFromSwiftToAddr(&lsa, *my_addr);
		
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

	Dprintf("create swift send channel\n");	
	CHECK(s->recvChannel = calloc(100,sizeof(SOCKET)));
	
	s->usedChannels = 0;
	s->maxChannels = maxChannels;
	
	return s;
}


void closeSwift(Swift s)
{
	Dprintf("close swift socket\n");
	close(s->socketListener);
	close(s->sendChannel);
	
	
	free(s);
}


