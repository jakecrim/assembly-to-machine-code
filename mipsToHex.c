#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>



void handle_instruction()
{
	// *MAY DO READING AND PARSING RIGHT HERE*

	// assuming the instruciton is read in from memory and parsed at this point
	// switch(parsed_line)
}

int main(void)
{
	printf("Assembly to Machine Code Generator \n");
	// variables
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

	handle_instruction();


	return 0;
}




