#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "lib_swift.h"

ssize_t recvfrom(int s, void *buf, size_t len, int flags,
                 struct sockaddr *from, socklen_t *fromlen);

int recvfromSwift (Swift s, void *buf, size_t len, int flags,
                 struct sockaddr * __restrict__ from, socklen_t *fromlen)
{
	struct sockaddr_in si_other;
	socklen_t slen=sizeof(si_other);

	Dprintf("wait to receive messages");
	return recvfrom(s->socketListener, buf, len, flags,
		from, &slen);
}

int bindSwift(Swift s, const struct sockaddr *my_addr, socklen_t addrlen)
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
