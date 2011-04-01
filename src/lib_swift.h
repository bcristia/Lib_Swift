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
#define Dprintf(msg,...) printf("[%s]:%d " msg, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define Dprintf(msg,...)                /* do nothing */
#endif

// swift interface
typedef struct swift {
	int socketListener;
	int sendChannel;
	int *recvChannel;
	
	int usedChannels;
	int maxChannels;
} *Swift;

// swift_addr structure similar with in_addr 
struct swift_addr {
	unsigned short N;					// e.g. number of s_addr
	unsigned long s_addr[MAX_IPs];		// i.p. ip list
};

// swift struct similar with sock_addr
typedef struct sockSwiftaddr {
	short				sin_family;		// e.g. AF_INET
    unsigned short		sin_port;		// e.g. htons(3490)
    struct swift_addr   sin_addr;		// see struct swift_addr, below
} *SockSwiftaddr;

// list of swift_addr
struct listsockaddr {
	unsigned short N;
	struct sockaddr_in sa[MAX_IPs];
};	

// Function to create a Swift socket
Swift socketSwift(int maxChannels);

// Function to close a Swift socket
void closeSwift(Swift);

// Function to listen to a port
int listenfromSwift (Swift s, void *buf, size_t len, int flags,
                 struct sockSwiftaddr * __restrict__ from, socklen_t *fromlen);

// Function to bind a port for swift socket
int bindSwift(Swift s, const struct sockSwiftaddr *my_addr, socklen_t addrlen);

// Function to receive a message
ssize_t recvfromSwift(Swift s, void *buf, size_t len, int flags,
                 struct sockSwiftaddr *from, socklen_t fromlen);
                 
// Function to send a message
ssize_t sendToSwift(Swift s, const void *buf, size_t len, int flags, 
					const struct sockSwiftaddr *to, socklen_t tolen);


// test function -- don't commit
struct sockSwiftaddr transformFromAddrToSwift(struct listsockaddr lsa);
struct listsockaddr transformFromSwiftToAddr(struct sockSwiftaddr ssa);

#endif
