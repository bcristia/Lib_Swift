/*
 *  client.c
 *  
 *
 *  Created by Bogdan Drutu on 5/3/11.
 *  Copyright 2011 UPB. All rights reserved.
 *
 */


// Must be run by root lol! Just datagram, no payload/data
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

// Source IP, source port, target IP, target port from the command line arguments
int main(int argc, char *argv[])
{
	int socket_raw;
	
	// No data/payload just datagram
	char buffer[SWIFT_PCKT_LEN];
	
	// Our own headers' structures
	struct swiftheader *udp = (struct swiftheader *) (buffer);// + sizeof(struct ipheader));
	
	// Source addresses: IP and port
	struct sockaddr_in sin;

	memset(buffer, 0, SWIFT_PCKT_LEN);
	
	if(argc != 5)
	{
		fprintf(stderr,"- Usage %s <source hostname/IP> <source port> <target hostname/IP> <target port>\n", argv[0]);
		DIE("Invalid parameters!!!\n");
	}
	
	// Create a raw socket with UDP protocol
	socket_raw = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
	if(socket_raw < 0)
	CHECK(socket_raw < 0, "socket() error");
	
	// The source is redundant, may be used later if needed
	// The address family
	sin.sin_family = AF_INET;
	
	// Port numbers
	sin.sin_port = htons(atoi(argv[2]));
	
	// IP addresses
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	
	// Fabricate the UDP header. Source port number, redundant
	udp->udph_srcport = htons(atoi(argv[2]));
	// Destination port number
	udp->udph_destport = htons(atoi(argv[4]));
	udp->udph_len = htons(sizeof(struct swiftheader));

	// Send loop, send for every 2 second for 100 count
	printf("Trying...\n");
	printf("Using raw socket and UDP protocol\n");
	printf("Using Source IP: %s port: %u, Target IP: %s port: %u.\n", argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
	
	int count;
	for(count = 1; count <=20; count++)
	{
		Dprintf("SIZEOF udp = %lu\n", sizeof(struct swiftheader));
		
		CHECK(sendto(socket_raw, buffer, sizeof(struct swiftheader), 0, (struct sockaddr *)&sin, sizeof(sin)) < 0 , "sendto() error");
		
		printf("Count #%u - sendto() is OK.\n", count);
		sleep(2);
	}
	
	close(socket_raw);
	return 0;
}
