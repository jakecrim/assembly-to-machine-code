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
		printf("%s \n", parsedLine[i]);
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
		hash_table_print();
		printf("%s \n", parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (uint32_t)strtol(parsedLine[1], NULL, 0);
		printf("HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
	}

	// handle R - type parsing
	if(hexInfoCurrent.instructionType == 'r')
	{
		printf("BEFORE: HexRep: 0x%08X \n", hexInfoCurrent.hexRep);
		hexRegRep * current;
		
		/* Retrieve the hex values for each register name and put it into the correct place in the hex representation */
		// rd register
		current = hash_table_search(parsedLine[1]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 11;
		printf("HexRep After: 0x%08x \n", hexInfoCurrent.hexRep);
		// rs register
		current = hash_table_search(parsedLine[2]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 21;
		printf("HexRep After: 0x%08x \n", hexInfoCurrent.hexRep);
		// rt register
		current = hash_table_search(parsedLine[3]);
		hexInfoCurrent.hexRep = hexInfoCurrent.hexRep | (current->hexRegRepCurrent) << 16;
		printf("HexRep After: 0x%08x \n", hexInfoCurrent.hexRep);
	}


	// hash_table_print();
	// hexRegRep * current = hash_table_search("s5");
	// printf("Found Value 0x%X \n", current->hexRegRepCurrent);
	// hash_table_remove("v0");
	// hash_table_print()
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

	// hash_table_print();
	// hexRegRep * current = hash_table_search("s5");
	// printf("Found Value 0x%X \n", current->hexRegRepCurrent);
	// hash_table_remove("v0");
	// hash_table_print();

	read_instruction();
	handle_instruction(pieces);


	return 0;
}




