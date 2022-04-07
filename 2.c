#include <stdio.h>

int main()
{
	FILE *sourceFile;
	char symb;
	sourceFile = fopen("3.txt", "r");
	if (sourceFile == NULL)
	{
		printf("Can't open file!\n");
	}
	while (!feof(sourceFile))
	{
		symb = fgetc(sourceFile);
		putc(symb, stdout);
	}
	return 0;
}
