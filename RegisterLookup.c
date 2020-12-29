#include "RegisterLookup.h"
#define NUMREGS 29

static RegLookup RegTable[NUMREGS] = {
	{0, "00000", "$zero"},
	{1, "00001", "$at"},
	{2, "00010", "$v0"},
	{3, "00011", "$v1"},
	
	
	{8, "01000", "$t0"},
	{9, "01001", "$t1"},
	{10, "01010", "$t2"},
	{11, "01011", "$t3"},
	
	{12, "01100", "$t4"},
	{13, "01101", "$t5"},
	{14, "01110", "$t6"},
	{15, "01111", "$t7"},
	{24, "11000", "$t8"},
	{25, "11001", "$t9"},
	
	
	{4, "00100", "$a0"},
	{5, "00101", "$a1"},
	{6, "00110", "$a2"},
	{7, "00111", "$a3"},
	
	{11, "01011", "$t3"},
	
	{26, "11010", "$k0"},
	{27, "11011", "$k1"},
	
	
	
	{16, "10000", "$s0"},
	{17, "10001", "$s1"},
	{18, "10010", "$s2"},
	{19, "10011", "$s3"},
	{20, "10100", "$s4"},
	{21, "10101", "$s5"},
	{22, "10110", "$s6"},
	{23, "10111", "$s7"}
};

/**
 * looks for register info
 */
const RegLookup* FindReg(const char* const regName){
	for(int i = 0; i < NUMREGS; i++){
		if(strncmp(regName, "$zero", 5) == 0){
			return &RegTable[0];
		}
		if(strncmp(RegTable[i].RName, regName, 3) == 0){
			return &RegTable[i];
		}
	}
	return NULL;
}


