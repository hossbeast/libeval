#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>

int main(int argc, char ** argv)
{
	char * mem = mmap(0, 2048, PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if(mem == MAP_FAILED)
		printf("wtf\n");

	memset(mem, 0, 2048);
#include "bytes"
	
	int (*add)(int a, int b) = (void*)mem;

	int a = 3;
	int b = 75;
	int c = add(a, b);
printf("add(%d,%d)=%d\n", a, b, c);

	return 0;
}
