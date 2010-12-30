#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

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

int listenfromSwift (Swift s, void *buf, size_t len, int flags,
                 struct sockSwiftaddr * __restrict__ from, socklen_t *fromlen)
{
	struct sockaddr s_other;
	struct listsockaddr lsa;
	socklen_t slen=sizeof(s_other);

	Dprintf("wait to receive messages");
	
	int rec = recvfrom(s->socketListener, buf, len, flags,
		(struct sockaddr * __restrict__)&s_other, &slen);
	
	// fill listsockaddr
	memcpy(&lsa.sa[0], &s_other, sizeof(s_other));
	lsa.N = 1;
	
	*from = transformFromAddrToSwift(lsa);
	
	return rec;
}

int bindSwift(Swift s, const struct sockSwiftaddr *my_addr, socklen_t addrlen)
{
	Dprintf("bind swift socket");
	return bind(s->socketListener, 
		(const struct sockaddr *)&s->socketListenerAddr,
		sizeof(s->socketListenerAddr));
}

Swift socketSwift()
{
	Dprintf("create swift socket");	
	Swift s = calloc(1,sizeof(*s));

	Dprintf("create swift listener");	
	CHECK(s->socketListener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));

	memset((char *) &s->socketListenerAddr, 0, sizeof(s->socketListenerAddr));
	
	s->socketListenerAddr.sin_family = AF_INET;
	s->socketListenerAddr.sin_port = htons(SWIFT_PORT);
	s->socketListenerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	return s;
}

void closeSwift(Swift s)
{
	Dprintf("close swift socket");
	close(s->socketListener);
	
	free(s);
}
