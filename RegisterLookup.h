#ifndef REGISTERLOOKUP_H
#define REGISTERLOOKUP_H
#include <inttypes.h>
#include <stdio.h>

/**
 * RegNumber is the register integer number
 * RBinary is binary representation of register
 * RName is the register's name
 */ 
struct _RegisterLookup{
	int8_t RNumber;
	char* RBinary;
	char* RName;
};

typedef struct _RegisterLookup RegLookup;
const RegLookup* FindReg(const char* const reg);

#endif
