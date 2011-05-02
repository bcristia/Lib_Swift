#ifndef __UTILS__

#define __UTILS__

#define DIE(s) \
  do \
  { \
    printf("%s:%d: ", __func__, __LINE__); \
    perror(s); \
    exit(EXIT_FAILURE); \
  } while (0) \

#define CHECK(x) \
  do \
  { \
    if (!(x)) \
    { \
      printf("%s:%d: ", __func__, __LINE__); \
      perror(#x); \
      exit(EXIT_FAILURE); \
    } \
  } while (0) \

#ifdef DEBUG
#define Dprintf(msg,...) printf("[%s]:%d " msg, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define Dprintf(msg,...)                /* do nothing */
#endif

#endif
