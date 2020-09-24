#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>



void handle_instruction(char ** parsedLine)
{
	uint32_t hexRep = 0x00000000; // hex representation
	// *MAY DO READING AND PARSING RIGHT HERE*

	// *Assuming the instruciton is read in from memory and parsed at this point *

	/* Identify The Instruction */

	// I-type
	if(strcmp(parsedLine[0], "addiu") == 0)
	{
		printf("ADDIU \n");
		hexRep = hexRep | 0x24000000;
		// printf("HexRep: 0x%08X \n", hexRep);
	}
	else if(strcmp(parsedLine[0], "addi") == 0)
	{
		printf("ADDI \n");
		hexRep = hexRep | 0x20000000;
		// printf("HexRep: 0x%08X \n", hexRep);
	}
	else if(strcmp(parsedLine[0], "andi") == 0)
	{
		printf("ANDI \n");
		hexRep = hexRep | 0x30000000;
		// printf("HexRep: 0x%08X \n", hexRep);
	}
	else if(strcmp(parsedLine[0], "") == 0)
	{
		printf(" \n");
		hexRep = hexRep | 0x00000000;
		// printf("HexRep: 0x%08X \n", hexRep);
	}

	// J-type

	// R-type

	/* Identify rt register */

	/* Identify rs register */

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

	printf("--- Handling Instructions ---\n");
	handle_instruction(pieces);


	return 0;
}




