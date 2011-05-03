/*
 *  lib_swift_raw.c
 *  
 *
 *  Created by Bogdan Drutu on 5/3/11.
 *  Copyright 2011 UPB. All rights reserved.
 *
 */

#include <stdio.h>
#include <netinet/in.h>

#include "lib_swift_raw.h"

void printSwiftHeader (struct swiftheader *swift) {
	printf("Receive packet: ");
	printf("src_port = %d ", ntohl(swift->udph_srcport));
	printf("dest_port = %d ", ntohl(swift->udph_destport));
	printf("len = %d\n", ntohl(swift->udph_len));
}
