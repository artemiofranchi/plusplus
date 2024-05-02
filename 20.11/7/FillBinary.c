#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	FILE* f = fopen("7.bin", "w+");
	int n = 1;
	while (n != 0) {
		scanf("%d", &n);
		fwrite(&n, sizeof(int), 1, f);
	}
	fclose(f);
	return 0;
}
