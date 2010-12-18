#ifndef _LIB_SWIFT_
#define _LIB_SWIFT_

#define SWIFT_PORT		8080
#define PACKET_SIZE		4*1024

#define DIE(s) \
	do { \
		printf("%s:%d: ", __func__, __LINE__); \
		perror(s); \
		exit(EXIT_FAILURE); \
	} while (0) \

#define CHECK(x) \
	do { \
		if (!(x)) { \
			printf("%s:%d: ", __func__, __LINE__); \
			perror(#x); \
			exit(EXIT_FAILURE); \
		} \
	} while (0) \

#ifdef DEBUG
#define Dprintf(msg,...) printf("[%s]:%d" msg, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define Dprintf(msg,...)                /* do nothing */
#endif

typedef struct swift {
	int socketListener;
	struct sockaddr_in socketListenerAddr;
} *Swift;

Swift socketSwift();
void closeSwift(Swift);


int recvfromSwift (Swift, void *, size_t, int, struct sockaddr *, socklen_t *);
int bindSwift(Swift, const struct sockaddr *, socklen_t);

#endif
