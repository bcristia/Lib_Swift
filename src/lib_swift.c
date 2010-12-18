#include <stdio.h>
#include "lib_swift.h"


#ifdef DEBUG
#define Dprintf(msg,...) printf("[%s]:%d" msg, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define Dprintf(msg,...)                /* do nothing */
#endif


void f() 
{
	Dprintf("TESTARE in PLM");
	
	printf("Current file name %s\n", __FILE__);
	printf("Current line %d in file %s\n", __LINE__, __FILE__);
}
