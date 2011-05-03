/*
 *  utils.h
 *  
 *
 *  Created by Bogdan Drutu on 5/3/11.
 *  Copyright 2011 UPB. All rights reserved.
 *
 */

#ifndef __UTILS__

#define __UTILS__

#define DIE(s) \
  do \
  { \
    printf("%s:%d: ", __func__, __LINE__); \
    perror(s); \
    exit(EXIT_FAILURE); \
  } while (0) \

#define CHECK(x, s) \
  do \
  { \
    if (x) \
    { \
		DIE(s); \
    } \
  } while (0) \

#ifdef DEBUG
#define Dprintf(msg,...) printf("[%s]:%d " msg, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define Dprintf(msg,...)                /* do nothing */
#endif

#endif
