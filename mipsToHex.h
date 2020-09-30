#include <stdint.h>

#define MAX_REG_NAME 4
#define TABLE_SIZE 32

typedef struct hexRegRep{
	char reg_name[MAX_REG_NAME];
	uint32_t hexRegRepCurrent;
}hexRegRep;

#define DELETED (hexRegRep*)(0xFFFFFFFF)

typedef struct hexInfo{
	uint32_t hexRep;
	char instructionType;

}hexInfo;

hexInfo hexInfoCurrent;
hexRegRep * hash_table[TABLE_SIZE];


