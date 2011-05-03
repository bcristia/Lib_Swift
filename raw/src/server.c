/*
 *  client.c
 *  
 *
 *  Created by Bogdan Drutu on 5/3/11.
 *  Copyright 2011 UPB. All rights reserved.
 *
 */


// Must be run by root lol! Just datagram, no payload/data#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#include "utils.h"
#include "lib_swift_raw.h"

int main(int argc, char *argv[])
{
	int socket_raw;
	
	// No data/payload just datagram
	char buffer[SWIFT_PCKT_LEN];
	
	// Destination addresses: IP and port
	struct sockaddr_in din;
	// Destination length
	socklen_t len;
		
	// Create a raw socket with UDP protocol
	socket_raw = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
	CHECK(socket_raw < 0, "socket() error");
	
	while (1)
	{
		memset(buffer, 0, SWIFT_PCKT_LEN);
		
		if(recvfrom(socket_raw, buffer, SWIFT_PCKT_LEN, 0, (struct sockaddr *)&din, &len) < 0)
		// Verify
		{
			DIE("sendto() error");
		}
		else
		{
			printSwiftHeader((struct swiftheader *) buffer);
		}
	}
	
	close(socket_raw);
	return 0;
}
