#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include "mipsToHex.h"

#define ROWS 11
#define COLS 128

char raw[100][100];


/* HASH TABLE FUNCTIONS */
uint32_t hash(char * reg_name)
{
	int length = strnlen(reg_name, MAX_REG_NAME);
	uint32_t hashVal = 0;
	for(int i = 0; i < length; i++)
	{
		hashVal += reg_name[i];
		hashVal = ((hashVal * reg_name[i]) % TABLE_SIZE);
	}

	return hashVal;
}

bool hash_table_open()
{
	for(int i = 0; i < TABLE_SIZE; i++)
		hash_table[i] = NULL;

	return 0;
}

bool hash_table_insert(hexRegRep * current)
{
	if(current == NULL)
		return false;
	int index = hash(current->reg_name);
	for(int i = 0; i < TABLE_SIZE; i++)
	{
		int attempt = (i + index) % TABLE_SIZE;
		if(hash_table[attempt] == NULL || hash_table[attempt] == DELETED)
		{
			hash_table[attempt] = current;
			return true;
		}
	}
	return false;
}

hexRegRep * hash_table_search(char * current)
{
	int index = hash(current);
	for(int i = 0; i < TABLE_SIZE; i++)
	{
		int attempt = (i + index) % TABLE_SIZE;
		if(hash_table[attempt] == NULL)
			return false;
		if(hash_table[attempt] == DELETED)
			continue;
		if(hash_table[attempt] != NULL && strcmp(hash_table[attempt]->reg_name, current) == 0)
		{
			return hash_table[attempt];
		}
	}
	return NULL;
}

void hash_table_print()
{
	printf("Start: \n");
	for(int i = 0; i < TABLE_SIZE; i++)
	{
		if (hash_table[i] == NULL)
			printf("%i \t--- \n", i);
		else if(hash_table[i] == DELETED)
		{
			printf("DELETED \n");
		}
		else
		{
			printf("%i \t %s \n", i , hash_table[i]->reg_name);
		}
		
	}
	printf("End: \n");
}

hexRegRep * hash_table_remove(char * current)
{
	int index = hash(current);
	for(int i = 0; i < TABLE_SIZE; i++)
	{
		int attempt = (i + index) % TABLE_SIZE;
		if (hash_table[attempt] == NULL)
			return NULL;
		if (hash_table[attempt] == DELETED)
			continue;
		if(hash_table[attempt] != NULL && strcmp(hash_table[attempt]->reg_name, current) == 0)
		{
			hexRegRep * returnForFree = hash_table[attempt];
			hash_table[attempt] = DELETED;
			return returnForFree;
		}
	}
	return NULL;
	
}



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

void write_instruction()
{

	// write to output file
	FILE * outputFile;
	outputFile = fopen("output.txt", "w");
	if(outputFile == NULL)
	{
		printf("ERROR OPENING FILE \n");
		exit(1);
	}

	for(int i = 0; i < PROGRAM_LENGTH; i++)
	{
		fprintf(outputFile, "0x%08x \n", hexInfoCurrent.hexRepsArray[i]);
	}
	fclose(outputFile);

}

// Handles the instruction residing in 'parsedLine' currently this is only handling the first instruction of the text file
// 	though soon t will iterate over each line in 'raw'
void handle_instruction(int line)
{
	hexInfoCurrent.hexRep = 0x00000000; // variable for hex representation of the current instruction in its entirety
	char * parsedLine[6]; // will hold the parsed instruction line (in string form)
	char * tempParse; // used for helping parse the line
	int i = 0;
	parsedLine[5] = NULL;

	hexRegRep * current; // this is the current hexadecimal representation of the register we are working with (rs, rt, rd)


	/* PARSING INTO PIECES OF THE SINGLE INSTRUCTION LINE */	
	tempParse = strtok(raw[line], " $,()");
	while(tempParse != NULL)
	{
		parsedLine[i] = tempParse;
		tempParse = strtok(NULL, " $,");
		i++;
	}
	printf(" - Current Instruction - \n");
	i = 0;

	/* Identify The Instruction */
	// I-type
	if(strcmp(parsedLine[0], "addiu") == 0) // rt (base) , rs , offset
	{
		printf("ADDIU \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x24000000;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// immediate 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[3], NULL, 0);
	}
	else if(strcmp(parsedLine[0], "addi") == 0) // rt (base) , rs , offset
	{
		printf("ADDI \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x20000000;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// immediate 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[3], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "andi") == 0) // rt (base) , rs , offset
	{
		printf("ANDI \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x30000000;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// immediate 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[3], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "ori") == 0) // rt (base) , rs , offset
	{
		printf("ORI\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x34000000;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// immediate 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[3], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "xori") == 0) // rt (base) , rs , offset
	{
		printf("XORI\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x38000000;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// immediate 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[3], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "slti") == 0) // rt (base) , rs , offset
	{
		printf("SLTI\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x28000000;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// immediate 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[3], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "lui") == 0) // rt , imm
	{
		printf("LUI\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x3C000000;
		// rt register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// immediate 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[2], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "lw") == 0) // rt, offset(base) ****
	{
		printf("LW\n");
		printf("Parts: %s | %s | %s | %s | /n" , parsedLine[0], parsedLine[1], parsedLine[2], parsedLine[3]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x8C000000;
		// rt register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// immediate 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[2], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "sw") == 0) // rt, offset(base) ****
	{
		printf("SW\n");
		printf("Parts: %s | %s | %s | %s | \n" , parsedLine[0], parsedLine[1], parsedLine[2], parsedLine[3]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0xAC000000;
		// rt register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// immediate 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[2], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "sb") == 0) // rt (base) , rs , offset ****
	{
		printf("SB\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0xA0000000;
		// rt register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// offset
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[2], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "sh") == 0) // rt (base) , rs , offset ****
	{
		printf("SH\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0xA4000000;
		// rt register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// offset
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[2], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "beq") == 0) // rs , rt, offset
	{
		printf(" BEQ \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x10000000;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// offset 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[3], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "bgez") == 0) // SPECIAL: rs , offset
	{
		printf("BGEZ \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x04010000;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// offset 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[2], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "bgtz") == 0) // SPECIAL: rs, offset
	{
		printf("BGTZ \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x1C000000;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// offset 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[2], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "blez") == 0) // SPECIAL: rs, offset
	{
		printf("BLEZ \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x18000000;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// offset 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[2], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "bltz") == 0) // SPECIAL: rs, offset
	{
		printf("BLTZ \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x04000000;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// offset 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[2], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "bne") == 0) // rs , rt, offset
	{
		printf("BNE \n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x14000000;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// immediate 
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[3], NULL, 0);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}

	// J-type  registers handled below
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

	else if(strcmp(parsedLine[0], "add") == 0) // rd, rs, rt
	{
		printf("ADD\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000020;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rs register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[3]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "addu") == 0) // rd, rs, rt
	{
		printf("ADDU\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000021;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rs register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[3]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "sub") == 0) // rd, rs, rt
	{
		printf("SUB\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000022;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rs register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[3]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "subu") == 0) // rd, rs, rt
	{
		printf("SUBU\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000023;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rs register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[3]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "mult") == 0)	// rs, rt
	{
		printf("MULT\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000018;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "multu") == 0)	// rs, rt
	{
		printf("MULTU\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000019;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "div") == 0)	// rs, rt
	{
		printf("DIV\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x0000001A;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "divu") == 0)	// rs, rt
	{
		printf("DIVU\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000001B;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "and") == 0) // rd, rs , rt
	{
		printf("AND\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000024;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rs register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[3]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "mfhi") == 0) // rd
	{
		printf("MFHI\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000010;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "mflo") == 0) // rd
	{
		printf("MFLO\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000012;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "mthi") == 0) // rs
	{
		printf("MTHI\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000011;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "mtlo") == 0) // rs
	{
		printf("MTLO\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000013;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "or") == 0)  // rd, rs , rt
	{
		printf("OR\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000025;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rs register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[3]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "xor") == 0) // rd, rs , rt
	{
		printf("XOR\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000026;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rs register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[3]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "nor") == 0) // rd, rs , rt
	{
		printf("NOR\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000027;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rs register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[3]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "slt") == 0) // rd, rs , rt
	{
		printf("SLT\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x0000002A;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rs register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// rt register
		current = hash_table_search(parsedLine[3]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "sll") == 0) // rd, rt , sa // is already 0
	{
		printf("SLL\n");
		// hexInfoCurrent.hexRep = hexRep | 0x00000000;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// shift amount
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[3], NULL, 0) << 6;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "srl") == 0) // rd, rt , sa 
	{
		printf("SRL\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000002;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// shift amount
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[3], NULL, 0) << 6;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "sra") == 0) // rd, rt , sa 
	{
		printf("SRA\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x000000003;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rt register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		// shift amount
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[3], NULL, 0) << 6;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "jr") == 0) // rs
	{
		printf("JR\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000008;
		// rs register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		// printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}
	else if(strcmp(parsedLine[0], "jalr") == 0) // rd, rs
	{
		printf("JALR\n");
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | 0x00000009;
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		// rs register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
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
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[1], NULL, 0);
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}

	// store each hex representation into an array to be written to output file later
	printf("HexRep After: 0x%08x \n", hexInfoCurrent.hexRep);
	hexInfoCurrent.hexRepsArray[line] = hexInfoCurrent.hexRep;

}


int main(void)
{
	printf("| Assembly to Machine Code Generator | \n");

	printf("--- Handling Instructions ---\n");

	hash_table_open();
	
	// filling hash table
	hexRegRep regs0 = {.reg_name = "zero" , .hexRegRepCurrent = 0x0};
	hexRegRep regs1 = {.reg_name = "at" , .hexRegRepCurrent = 0x1};
	hexRegRep regs2 = {.reg_name = "v0" , .hexRegRepCurrent = 0x2};
	hexRegRep regs3 = {.reg_name = "v1" , .hexRegRepCurrent = 0x3};
	hexRegRep regs4 = {.reg_name = "a0" , .hexRegRepCurrent = 0x4};
	hexRegRep regs5 = {.reg_name = "a1" , .hexRegRepCurrent = 0x5};
	hexRegRep regs6 = {.reg_name = "a2" , .hexRegRepCurrent = 0x6};
	hexRegRep regs7 = {.reg_name = "a3" , .hexRegRepCurrent = 0x7};
	hexRegRep regs8 = {.reg_name = "t0" , .hexRegRepCurrent = 0x8};
	hexRegRep regs9 = {.reg_name = "t1" , .hexRegRepCurrent = 0x9};
	hexRegRep regs10 = {.reg_name = "t2" , .hexRegRepCurrent = 0xa};
	hexRegRep regs11 = {.reg_name = "t3" , .hexRegRepCurrent = 0xb};
	hexRegRep regs12 = {.reg_name = "t4" , .hexRegRepCurrent = 0xc};
	hexRegRep regs13 = {.reg_name = "t5" , .hexRegRepCurrent = 0xd};
	hexRegRep regs14 = {.reg_name = "t6" , .hexRegRepCurrent = 0xe};
	hexRegRep regs15 = {.reg_name = "t7" , .hexRegRepCurrent = 0xf};
	hexRegRep regs16 = {.reg_name = "s0" , .hexRegRepCurrent = 0x10};
	hexRegRep regs17 = {.reg_name = "s1" , .hexRegRepCurrent = 0x11};
	hexRegRep regs18 = {.reg_name = "s2" , .hexRegRepCurrent = 0x12};
	hexRegRep regs19 = {.reg_name = "s3" , .hexRegRepCurrent = 0x13};
	hexRegRep regs20 = {.reg_name = "s4" , .hexRegRepCurrent = 0x14};
	hexRegRep regs21 = {.reg_name = "s5" , .hexRegRepCurrent = 0x15};
	hexRegRep regs22 = {.reg_name = "s6" , .hexRegRepCurrent = 0x16};
	hexRegRep regs23 = {.reg_name = "s7" , .hexRegRepCurrent = 0x17};
	hexRegRep regs24 = {.reg_name = "t8" , .hexRegRepCurrent = 0x18};
	hexRegRep regs25 = {.reg_name = "t9" , .hexRegRepCurrent = 0x19};
	hexRegRep regs26 = {.reg_name = "k0" , .hexRegRepCurrent = 0x1a};
	hexRegRep regs27 = {.reg_name = "k1" , .hexRegRepCurrent = 0x1b};
	hexRegRep regs28 = {.reg_name = "gp" , .hexRegRepCurrent = 0x1c};
	hexRegRep regs29 = {.reg_name = "sp" , .hexRegRepCurrent = 0x1d};
	hexRegRep regs30 = {.reg_name = "fp" , .hexRegRepCurrent = 0x1e};
	hexRegRep regs31 = {.reg_name = "ra" , .hexRegRepCurrent = 0x1f};

	hash_table_insert(&regs0);
	hash_table_insert(&regs1);
	hash_table_insert(&regs2);
	hash_table_insert(&regs3);
	hash_table_insert(&regs4);
	hash_table_insert(&regs5);
	hash_table_insert(&regs6);
	hash_table_insert(&regs7);
	hash_table_insert(&regs8);
	hash_table_insert(&regs9);
	hash_table_insert(&regs10);
	hash_table_insert(&regs11);
	hash_table_insert(&regs12);
	hash_table_insert(&regs13);
	hash_table_insert(&regs14);
	hash_table_insert(&regs15);
	hash_table_insert(&regs16);
	hash_table_insert(&regs17);
	hash_table_insert(&regs18);
	hash_table_insert(&regs19);
	hash_table_insert(&regs20);
	hash_table_insert(&regs21);
	hash_table_insert(&regs22);
	hash_table_insert(&regs23);
	hash_table_insert(&regs24);
	hash_table_insert(&regs25);
	hash_table_insert(&regs26);
	hash_table_insert(&regs27);
	hash_table_insert(&regs28);
	hash_table_insert(&regs29);
	hash_table_insert(&regs30);
	hash_table_insert(&regs31);

	// read instructions in to raw, then go line by line the length of program (set in mipsToHex.h) and handle each instruction
	read_instruction();
	hash_table_print();
	// write_instruction(5);
	for(int line = 0; line <= PROGRAM_LENGTH; line++)
	{
		handle_instruction(line);
	}

	// write to output file
	write_instruction();

	return 0;
}




