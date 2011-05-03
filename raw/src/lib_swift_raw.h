/*
 *  lib_raw.h
 *  
 *
 *  Created by Bogdan Drutu on 5/3/11.
 *  Copyright 2011 UPB. All rights reserved.
 *
 */

#ifndef __LIB_RAW__

#define __LIB_RAW__

// The swift protocol number
#define IPPROTO_SWIFT		13

// The packet length
#define SWIFT_PCKT_LEN		4096

// SWIFT header's structure
struct swiftheader {
	unsigned short int udph_srcport;
	unsigned short int udph_destport;
	unsigned short int udph_len;
	unsigned short int udph_chksum;
};

void printSwiftHeader (struct swiftheader * swift);

#endif