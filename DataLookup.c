#include "DataLookup.h"
#include <inttypes.h>
#include <stdio.h>

#define NUMINSTRUCTIONS 27


static DataLookup DataTable[NUMINSTRUCTIONS] = {
	{"add", "000000", "100000"},
	{"and", "000000", "100100"},
	{"sub", "000000", "100010"},
	{"nor", "000000", "100111"},
	{"slti", "001010", NULL},
	{"syscall", "000000", "001100"},
	{"addi", "001000", NULL},
	{"andi", "001100", NULL},
	{"lui", "001111", NULL},
	{"lw", "100011", NULL},
	{"la", "001000"},
	{"mul", "011100", "000010"},
	{"beq", "000100"},
	{"bne", "000101"},
	
	{"sw", "101011"},//done
	{"addu", "000000", "100001"},//done 
	{"addiu", "001001"},//done
	{"sll", "000000", "000000"},
	{"slt", "000000", "101010"},//done
	{"sra", "000000", "000011"},
	{"srav", "000000", "000111"},//done
	
	{"blez", "000110"},//done
	{"bgtz", "000111"},//done
	
	{"j", "000010"},//done
	
	{"move", "000000", "100001"}, //addu rd, $zero, rs //done
	{"blt", "000101"}, //slt at, rs, rt -----> bne at, $zero, offset
	{"li", "001001"} //addiu rt, $zero, imm16 //done
	
};

/**
 * looks for mnemonic info
 */
const DataLookup* FindMnem(const char* const mnem){
	for(int i = 0; i < NUMINSTRUCTIONS; i++){
		if(strcmp(DataTable[i].Mnem, mnem) == 0){
			return &DataTable[i];
		}
	}
	return NULL;	
}
	
