#include <stdint.h>

// *** SET PROGRAM LENGTH ***
#define PROGRAM_LENGTH 40 
// *** SET PROGRAM LENGTH ***

#define MAX_REG_NAME 4
#define TABLE_SIZE 32

// Struct for register hex representation
typedef struct hexRegRep{
	char reg_name[MAX_REG_NAME];
	uint32_t hexRegRepCurrent;
}hexRegRep;

// used for deleted nodes in hashing
#define DELETED (hexRegRep*)(0xFFFFFFFF)

// struct for overall instruction hex representation
typedef struct hexInfo{
	uint32_t hexRep;
	char instructionType;
	uint32_t hexRepsArray[PROGRAM_LENGTH];

}hexInfo;

hexInfo hexInfoCurrent;
hexRegRep * hash_table[TABLE_SIZE];


