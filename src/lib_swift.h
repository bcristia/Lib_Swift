#ifndef _LIB_SWIFT_
#define _LIB_SWIFT_

#define SWIFT_PORT		8080
#define PACKET_SIZE		4*1024
#define MAX_IPs			10

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

struct swift_addr {
	unsigned short N;					// e.g. number of s_addr
	unsigned long s_addr[MAX_IPs];		// i.p. ip list
};

typedef struct sockSwiftaddr {
	short				sin_family;		// e.g. AF_INET
    unsigned short		sin_port;		// e.g. htons(3490)
    struct swift_addr   sin_addr;		// see struct swift_addr, below
} *SockSwiftaddr;

struct listsockaddr {
	unsigned short N;
	struct sockaddr_in sa[MAX_IPs];
};	

Swift socketSwift();
void closeSwift(Swift);


int recvfromSwift (Swift, void *, size_t, int, struct sockSwiftaddr *, socklen_t *);
int bindSwift(Swift, const struct sockSwiftaddr *, socklen_t);

#endif
