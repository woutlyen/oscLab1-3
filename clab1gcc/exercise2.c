#include <stdio.h>
#include <limits.h>

int main (int argc, char **argv)
{
	// int
	printf("Size of (int) = %lu bytes\n", sizeof(int));

	// float
        printf("Size of (float) = %lu bytes\n", sizeof(float));

	// double
        printf("Size of (double) = %lu bytes\n", sizeof(double));

	// void
        printf("Size of (void) = %lu bytes\n", sizeof(void));

	// pointer
        printf("Size of (pointer) = %lu bytes\n", sizeof(int*));

	// short
        printf("Size of (short) = %lu bytes\n", sizeof(short));

	// long
        printf("Size of (long) = %lu bytes\n", sizeof(long));

	// Char max
        printf("CHAR_MAX = %d\n", CHAR_MAX);

	// char min
        printf("CHAR_MIN = %d\n", CHAR_MIN);


	if(CHAR_MAX == 255) {
		printf("Unsigned Char\n");
	}
	else {
		printf("Signed Char\n");
	}

return 0;
}
