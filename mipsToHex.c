#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "mipsToHex.h"

#define ROWS 11
#define COLS 128

char raw[100][100];

// Reads all instructions into 'raw'
char * read_instruction()
{
	FILE * fp = NULL;
	// char raw[100][100];
	char filename[25];
	int currentLength = 0;

	printf("Input Filename for reading: \n");
	scanf("%s", filename);

	fp = fopen(filename, "r");
	while(fgets(raw[currentLength], COLS + 1, fp))
	{
		raw[currentLength][strlen(raw[currentLength]) - 1 ] = '\0';
		currentLength++;
	}
	
	return 0;
}

// Handles the instruction residing in 'parsedLine' currently this is only handling the first instruction of the text file
// 	though soon t will iterate over each line in 'raw'
void handle_instruction(char ** parsedLine2)
{
	hexInfoCurrent.hexRep = 0x00000000; // hex representation
	char * parsedLine[6];
	char * tempParse;
	int i = 0;
	parsedLine[5] = NULL;


	/* PARSING INTO PIECES OF THE SINGLE INSTRUCTION LINE */	
	tempParse = strtok(raw[0], " $,");
	while(tempParse != NULL)
	{
		parsedLine[i] = tempParse;
		tempParse = strtok(NULL, " $,");
		i++;
	}
	// printf("Segments of instruction: \n");
	i = 0;
	while(parsedLine[i] != NULL)
	{
		// printf("%s \n", parsedLine[i]);
		i++;
	}

	/* Identify The Instruction */
	// I-type
	if(strcmp(parsedLine[0], "addiu") == 0)
	{
		printf("ADDIU \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x24000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "addi") == 0)
	{
		printf("ADDI \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x20000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "andi") == 0)
	{
		printf("ANDI \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x30000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "ori") == 0)
	{
		printf("ORI\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x34000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "xori") == 0)
	{
		printf("XORI\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x38000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "slti") == 0)
	{
		printf("SLTI\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x28000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "lui") == 0)
	{
		printf("LUI\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x3C000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "sw") == 0)
	{
		printf("SW\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0xAC000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "sb") == 0)
	{
		printf("SB\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0xA0000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "sh") == 0)
	{
		printf("SH\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0xA4000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "beq") == 0)
	{
		printf(" BEQ \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x10000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "bgez") == 0)
	{
		printf("BGEZ \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x04010000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "bgtz") == 0)
	{
		printf("BGTZ \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x1C000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "blez") == 0)
	{
		printf("BLEZ \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x18000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "bltz") == 0)
	{
		printf("BLTZ \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x04000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "bne") == 0)
	{
		printf("BNE \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x14000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}

	// J-type
	else if(strcmp(parsedLine[0], "j") == 0)
	{
		printf("J\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x08000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "jal") == 0)
	{
		printf("JAL\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x0C000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}

	// R-type

	else if(strcmp(parsedLine[0], "add") == 0)
	{
		printf("ADD\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000020;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "addu") == 0)
	{
		printf("ADDU\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000021;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "sub") == 0)
	{
		printf("SUB\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000022;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "subu") == 0)
	{
		printf("SUBU\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000023;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "mult") == 0)
	{
		printf("MULT\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000018;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "multu") == 0)
	{
		printf("MULTU\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000019;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "div") == 0)
	{
		printf("DIV\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x0000001A;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "divu") == 0)
	{
		printf("DIVU\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000001B;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "and") == 0)
	{
		printf("AND\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000024;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "mfhi") == 0)
	{
		printf("MFHI\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000010;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "mflo") == 0)
	{
		printf("MFLO\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000012;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "mthi") == 0)
	{
		printf("MTHI\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000011;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "mtlo") == 0)
	{
		printf("MTLO\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000013;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "or") == 0)
	{
		printf("OR\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000025;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "xor") == 0)
	{
		printf("XOR\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000026;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "nor") == 0)
	{
		printf("NOR\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000027;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "slt") == 0)
	{
		printf("SLT\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x0000002A;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "sll") == 0) // is already 0
	{
		printf("SLL\n");
		// hexInfoCurrent.hexRep = hexRep | 0x00000000;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "srl") == 0)
	{
		printf("SRL\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000002;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "sra") == 0)
	{
		printf("SRA\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x000000003;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "jr") == 0)
	{
		printf("JR\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000008;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "jalr") == 0)
	{
		printf("JALR\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000009;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}

	/* Identify Instruction Type */
	if((0xFC000000 & hexInfoCurrent.hexRep) >> 26 == 0)
		hexInfoCurrent.instructionType = 'r';
	else if((0xFC000000 & hexInfoCurrent.hexRep) >> 26 <= 3)
		hexInfoCurrent.instructionType = 'j';
	else if((0xFC000000 & hexInfoCurrent.hexRep) >> 26 > 3)
		hexInfoCurrent.instructionType = 'i';
	printf("Instruction Type: %c \n", hexInfoCurrent.instructionType);

	// handle J - type parsing
	if(hexInfoCurrent.instructionType == 'j')
	{
		printf("%s \n", parsedLine[1]);
		// uint32_t hexy = (uint32_t)strtol(parsedLine[1], NULL, 0);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[1], NULL, 0);
		// printf("HexRep: 0x%08x \n", hexy);
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}


	/* Identify rt register */

	/* Identify rs register */

}

int main(void)
{
	printf("| Assembly to Machine Code Generator | \n");
	// variables
	char * pieces[5];
	/* TEST STUFF IM SAVING FOR LATER BUT IS CURRENTLY UNUSED */
	// char test1[] = "addiu $a0, $zero, 0";
	// char * parsedLine;
	// int i = 0;

	// pieces[4] = NULL;

	// // Parsing the test string
	// parsedLine = strtok(test1, " $,");
	// while(parsedLine != NULL)
	// {
	// 	pieces[i] = parsedLine;
	// 	parsedLine = strtok(NULL, " $,");
	// 	i++;
	// }
	// // printing...
	// printf("Segments of instruction: \n");
	// i = 0;
	// while(pieces[i] != NULL)
	// {
	// 	printf("%s \n", pieces[i]);
	// 	i++;
	// }

	printf("--- Handling Instructions ---\n");
	read_instruction();
	handle_instruction(pieces);


	return 0;
}




