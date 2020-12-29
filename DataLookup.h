#ifndef DATALOOKUP_H
#define DATALOOKUP_H
#include <inttypes.h>
#include <stdio.h>

/**
 * Mnem is the mnemonic
 * Op is the opcode
 * Func is funct
 * 		func is null when mnemonic not one of: add, sub, and.
 */ 
struct _DataLookup{
	char* Mnem;
	char* Op;
	char* Func;
};

typedef struct _DataLookup DataLookup;

const DataLookup* FindMnem(const char* const mnem);

#endif
