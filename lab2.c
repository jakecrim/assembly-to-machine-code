#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

int main(void)
{
	printf("Assembly to Machine Code Generator \n");
	char test1[] = "addiu $a0, $zero, 0";
	char * pieces[5];
	char * parsedLine;
	int i = 0;
	pieces[4] = NULL;

	// Parsing the test string
	parsedLine = strtok(test1, " $,");
	while(parsedLine != NULL)
	{
		pieces[i] = parsedLine;
		parsedLine = strtok(NULL, " $,");
		i++;
	}
	// printing...
	printf("Segments of instruction: \n");
	i = 0;
	while(pieces[i] != NULL)
	{
		printf("%s \n", pieces[i]);
		i++;
	}


	return 0;
}