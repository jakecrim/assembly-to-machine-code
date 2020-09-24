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

// #include <iostream>
// using namespace std;

// // To execute C++, please define "int main()"


// class Pokemon
// {
//     public:
//         int lifePoints;
//         int attackDmg;
//         int defense;
    
     
//         void attack()
//         {
//             attack = rand(25,40);
//         }
    
//         void defend(int incomingAttack)
//         {
//             defense = rand(1,3);
//             if(defense == 1)
//             {
//                 return;
//             }
//             lifePoints -= incomingAttack;
//         }
         
//         // Constructor
//         Pokemon()
//         {
//             attackDmg = 0;
//             lifePoints = rand(50,100); 
//         }
    
//         // destructor 
//         ~Pokemon();
    
// }


// class Trainer
// {
//     Pokemon partner;
//     string name;
// }

// class pikachu 
// {
       
// };

// class squirtle
// {
       
// };

// int main() 
// {
//     // pokemon stuff!
//     Pokemon pikachu;
    
    
    
//     cout << "Hello World" << endl;
    
//     int * gpsel0 = (int *)0x40000000;
//     printf("0x%08X \n", gpsel0); 
    
//     for(int i = 0; i < 10; i++)
//     {
//         cout << "testing bruh" << endl;
        
//     }
//     return 0;
// }


