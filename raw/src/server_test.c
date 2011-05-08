#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <features.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/ip.h>
#include <netinet/in.h>

int CreateRawSocket(int protocol_to_sniff)
{
	int rawsock;

	if((rawsock = socket(PF_PACKET, SOCK_RAW, htons(protocol_to_sniff)))== -1)
	{
		perror("Error creating raw socket: ");
		exit(-1);
	}

	return rawsock;
}

int BindRawSocketToInterface(char *device, int rawsock, int protocol)
{
	
	struct sockaddr_ll sll;
	struct ifreq ifr;

	bzero(&sll, sizeof(sll));
	bzero(&ifr, sizeof(ifr));
	
	/* First Get the Interface Index  */


	strncpy((char *)ifr.ifr_name, device, IFNAMSIZ);
	if((ioctl(rawsock, SIOCGIFINDEX, &ifr)) == -1)
	{
		printf("Error getting Interface index !\n");
		exit(-1);
	}

	/* Bind our raw socket to this interface */

	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(protocol); 


	if((bind(rawsock, (struct sockaddr *)&sll, sizeof(sll)))== -1)
	{
		perror("Error binding raw socket to interface\n");
		exit(-1);
	}

	return 1;
	
}

void PrintPacketInHex(unsigned char *packet, int len)
{
	unsigned char *p = packet;

	printf("\n\n---------Packet---Starts----\n\n");
	
	while(len--)
	{
		printf("%.2x ", *p);
		p++;
	}

	printf("\n\n--------Packet---Ends-----\n\n");

}


int PrintInHex(char *mesg, unsigned char *p, int len)
{
	printf(mesg);

	while(len--)
	{
		printf("%.2X ", *p);
		p++;
	}

}


void ParseEthernetHeader(unsigned char *packet, int len)
{
	struct ethhdr *ethernet_header;

	if(len > sizeof(struct ethhdr))
	{
		ethernet_header = (struct ethhdr *)packet;

		/* First set of 6 bytes are Destination MAC */

		PrintInHex("Destination MAC: ", ethernet_header->h_dest, 6);
		printf("\n");
		
		/* Second set of 6 bytes are Source MAC */

		PrintInHex("Source MAC: ", ethernet_header->h_source, 6);
		printf("\n");

		/* Last 2 bytes in the Ethernet header are the protocol it carries */

		PrintInHex("Protocol: ",(void *)&ethernet_header->h_proto, 2);
		printf("\n");

		
	}
	else
	{
		printf("Packet size too small !\n");
	}
}

void ParseIpHeader(unsigned char *packet, int len)
{
	struct ethhdr *ethernet_header;
	struct iphdr *ip_header;

	/* First Check if the packet contains an IP header using
	   the Ethernet header                                */

	ethernet_header = (struct ethhdr *)packet;

	if(ntohs(ethernet_header->h_proto) == ETH_P_IP)
	{
		/* The IP header is after the Ethernet header  */
		
		if(len >= (sizeof(struct ethhdr) + sizeof(struct iphdr)))
		{
			ip_header = (struct iphdr*)(packet + sizeof(struct ethhdr));
			
			/* print the Source and Destination IP address */

			printf("Dest IP address: %s\n", inet_ntoa(ip_header->daddr));
			printf("Source IP address: %s\n", inet_ntoa(ip_header->saddr));
			printf("TTL = %d\n", ip_header->ttl);	

		}
		else
		{
			printf("IP packet does not have full header\n");
		}

	}
	else
	{
		/* Not an IP packet */

	}
}


int main(int argc, char **argv)
{
	int raw;
	unsigned char packet_buffer[2048]; 
	int len;
	int packets_to_sniff;
	struct sockaddr_ll packet_info;
	int packet_info_size = sizeof(packet_info_size);

	/* create the raw socket */

	raw = CreateRawSocket(ETH_P_IP);

	/* Bind socket to interface */

	BindRawSocketToInterface(argv[1], raw, ETH_P_IP);

	/* Get number of packets to sniff from user */

	packets_to_sniff = atoi(argv[2]);

	/* Start Sniffing and print Hex of every packet */
	
	while(packets_to_sniff--)
	{
		if((len = recvfrom(raw, packet_buffer, 2048, 0, (struct sockaddr*)&packet_info, &packet_info_size)) == -1)
		{
			perror("Recv from returned -1: ");
			exit(-1);
		}
		else
		{
			/* Packet has been received successfully !! */

			PrintPacketInHex(packet_buffer, len);

			/* Parse Ethernet Header */
			
			ParseEthernetHeader(packet_buffer, len);
			
			/* Parse IP Header */

			ParseIpHeader(packet_buffer, len);
		}
	}
	
	return 0;
}


