#include <stdint.h>

// *** SET PROGRAM LENGTH ***
<<<<<<< HEAD
#define PROGRAM_LENGTH 44
// #define PROGRAM_LENGTH 31
=======
 #define PROGRAM_LENGTH 44
//#define PROGRAM_LENGTH 31
>>>>>>> ceadfd93eed7298552557b670a7b1aa0e5abb077

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
	uint32_t hexRepsArray[PROGRAM_LENGTH + 1];

}hexInfo;

hexInfo hexInfoCurrent;
hexRegRep * hash_table[TABLE_SIZE];


