// On my honor:
// - I have not discussed the C language code in my program with
// anyone other than my instructor or the teaching assistants
// assigned to this course.
//
// - I have not used C language code obtained from another student,
// or any other unauthorized source, including the Internet,
// either modified or unmodified.
//
// - If any C language code or documentation used in my program
// was obtained from an allowed source, such as a text book or course
// notes, that has been clearly noted with a proper citation in
// the comments of my program.
//
// Natalie Kakish nataliekakish

#include <inttypes.h>  // I used functions from these Standard modules in
#include <string.h>    // my solution; you may or may not need all of them,
#include <stdlib.h>    // and you might need additional ones, depending on
#include <stdio.h>     // your approach to the problem.
#include <assert.h>
#include "DataLookup.h"
#include "RegisterLookup.h"
#include "ASMParser.h"
#include "Symbols.h"
#include <stdbool.h>
#define MAX_INSTRUCTIONS 100

/**
 * parses assembly file
 */
int main(int argc, char* argv []){
	
	FILE *input = fopen(argv[1], "r");
	FILE *out = fopen(argv[2], "w");

	
	if(argc == 4) {

		
		int offset = 0x2000;
		char instr[MAX_INSTRUCTIONS];
		//getting first line
		fgets(instr, MAX_INSTRUCTIONS, input);
		
		//skipping comments and new lines
		while (strncmp(instr, "\n", 2) == 0 || strncmp(instr, "#", 1) == 0) {
			fgets(instr, MAX_INSTRUCTIONS, input);	 
		}
		//skip .data line
		fgets(instr, MAX_INSTRUCTIONS, input);
		
		//while there's data in .data
		while (strstr(instr, ":") != NULL && instr != NULL) {
			
			char* tok;
			//if it's a word
			if(strstr(instr, ".word") != NULL) {
				
				if(strstr(instr, ",") != NULL) {
					tok = strtok(instr, " .\t:,");
					fprintf(out, "0x%08X:  ", offset);		
					fprintf(out, tok);			
					fprintf(out, "\n");
					tok = strtok(NULL, " .\t:,");
					tok = strtok(NULL, " .\t:,");
					while (tok != NULL) {
						offset+= 4;
						tok = strtok(NULL, " .\t:,");
					}
				}
			
				else {
					tok = strtok(instr, " .\t:");
					fprintf(out, "0x%08X:  ", offset);
					fprintf(out, tok);
					fprintf(out, "\n");
					tok = strtok(NULL, " .\t:");
					tok = strtok(NULL, " .\t:");
					tok = strtok(NULL, " .\t:");
					int ct;
					if (tok != NULL) {
						ct = atoi(tok);
					}
					else {
						ct = 1;
					}					
					offset = offset + 4*ct;	
				}	
			}
			else if (strstr(instr, ".asciiz") != NULL) {
				tok = strtok(instr, ":");
				fprintf(out, "0x%08X:  ", offset);
				fprintf(out, tok);
				
				fprintf(out, "\n");
				tok = strtok(NULL, "\"");
				tok = strtok(NULL, "\"");
				
				//3
				int len = strlen(tok) + 1; // with null terminator
				
				if(len <= 4) {
					offset += 4;
				}
				else{
					int leftover = len % 4;
					if(leftover != 0) {
						offset += 4;
					}
					int lines = len/4 * 4;
					
					offset += lines;
				}
			}
			//next line
			fgets(instr, MAX_INSTRUCTIONS, input);	
			
		}

		offset = 0x0;
		//next line
		fgets(instr, MAX_INSTRUCTIONS, input);
		while(fgets(instr, MAX_INSTRUCTIONS, input) != NULL) {
			char* temp = trim_space(instr);
			if (strstr(instr, ":") != NULL) {
				char* tok = strtok(instr, ":");
				fprintf(out, "0x%08X:  ", offset);
				fprintf(out, tok);
				
				fprintf(out, "\n");
			}
		
			//if not comments or new lines
			else if (instr[0] != '\n' &&
				temp[0] != '#') {
					
					if(strstr(instr, "blt") != NULL) {
						offset+=8;
					}
					else {
						offset+=4;		
					}
			}
		}		
	}
	else {
		
	//handling symbols
	Symbol** symbolz = malloc(1000*sizeof(Symbol*));
	int symbolzSz = 0;
	
	int offset = 0x2000;
	char instr[MAX_INSTRUCTIONS];
	//getting first line
	fgets(instr, MAX_INSTRUCTIONS, input);
	
	//skipping comments and new lines
	while (strncmp(instr, "\n", 2) == 0 || strncmp(instr, "#", 1) == 0) {
		fgets(instr, MAX_INSTRUCTIONS, input);	 
	}
	//skip .data line
	fgets(instr, MAX_INSTRUCTIONS, input);
	
	//while there's data in .data
	while (strstr(instr, ":") != NULL && instr != NULL) {
		
		char* tok;
		//if it's a word
		if(strstr(instr, ".word") != NULL) {
			
			if(strstr(instr, ",") != NULL) {
				tok = strtok(instr, " .\t:,");
				//fprintf(symbols, "0x%08X:  ", offset);		
				//fprintf(symbols, tok);
				Symbol* symbol = malloc(sizeof(Symbol));
				symbol->symbol=calloc(strlen(tok), sizeof(char));
				strcpy(symbol->symbol, tok);
				symbol->off=offset;
				*(symbolz + symbolzSz) = symbol;
				symbolzSz+=sizeof(Symbol*);
				//fprintf(symbols, "\n");
				tok = strtok(NULL, " .\t:,");
				tok = strtok(NULL, " .\t:,");
				while (tok != NULL) {
					offset+= 4;
					tok = strtok(NULL, " .\t:,");
				}
			}
		
			else {
				tok = strtok(instr, " .\t:");
				//fprintf(symbols, "0x%08X:  ", offset);
				//fprintf(symbols, tok);
				Symbol* symbol = malloc(sizeof(Symbol));
				symbol->symbol=calloc(strlen(tok) + 1, sizeof(char));
				strcpy(symbol->symbol, tok);
				symbol->off=offset;
				*(symbolz + symbolzSz) = symbol;
				symbolzSz+=sizeof(Symbol*);
				//fprintf(symbols, "\n");
				tok = strtok(NULL, " .\t:");
				tok = strtok(NULL, " .\t:");
				tok = strtok(NULL, " .\t:");
				int ct;
				if (tok != NULL) {
					ct = atoi(tok);
				}
				else {
					ct = 1;
				}					
				offset = offset + 4*ct;	
			}	
		}
		else if (strstr(instr, ".asciiz") != NULL) {
			tok = strtok(instr, ":");
			//fprintf(symbols, "0x%08X:  ", offset);
			//fprintf(symbols, tok);
			Symbol* symbol = malloc(sizeof(Symbol));
			symbol->symbol=calloc(strlen(tok) + 1, sizeof(char));
			strcpy(symbol->symbol, tok);
			symbol->off=offset;
			*(symbolz + symbolzSz) = symbol;
			symbolzSz+=sizeof(Symbol*);
			//fprintf(symbols, "\n");
			tok = strtok(NULL, "\"");
			tok = strtok(NULL, "\"");
			
			//3
			int len = strlen(tok) + 1; // with null terminator
			
			if(len <= 4) {
				offset += 4;
			}
			else{
				int leftover = len % 4;
				if(leftover != 0) {
					offset += 4;
				}
				int lines = len/4 * 4;
				
				offset += lines;
			}
		}
		//next line
		fgets(instr, MAX_INSTRUCTIONS, input);	
		
	}

	offset = 0x0;
	//next line
	fgets(instr, MAX_INSTRUCTIONS, input);
	while(fgets(instr, MAX_INSTRUCTIONS, input) != NULL) {
		char* temp = trim_space(instr);
		if (strstr(instr, ":") != NULL) {
			char* tok = strtok(instr, ":");
			//fprintf(symbols, "0x%08x:  ", offset);
			//fprintf(symbols, tok);
			Symbol* symbol = malloc(sizeof(Symbol));
			symbol->symbol= calloc(strlen(tok) + 1, sizeof(char));
			strcpy(symbol->symbol, tok);
			symbol->off=offset;
			*(symbolz + symbolzSz) = symbol;
			symbolzSz+=sizeof(Symbol*);
			//fprintf(symbols, "\n");
		}
	
		//if not comments or new lines
		else if (instr[0] != '\n' &&
			temp[0] != '#') {
				
				if(strstr(instr, "blt") != NULL) {
					offset+=8;
				}
				else {
					offset+=4;		
				}
		}
	}
	//rewind
	fseek(input, 0, SEEK_SET);
	//fclose(symbols);
	
	int count = 0;
	char text[MAX_INSTRUCTIONS];
	ParseResult* res;
	
	
	while (fgets(text, MAX_INSTRUCTIONS, input) != NULL) {
		//while no syscall, no $, no j, or if there's a colon
		while ((strstr(text, "syscall\n") == NULL 
			&& strstr(text, "$") == NULL
			&& strstr(text, "j") == NULL)
			|| strstr(text, ":") != NULL){
		
			if(strstr(text, "nop") != NULL){
				fprintf(out, "%s", "00000000000000000000000000000000\n");
				count++;
			
			}
			fgets(text, MAX_INSTRUCTIONS, input);
			if(strncmp(text, "# That's all...", 15) == 0) {
				break;
			}
			
		}
		
		if(strstr(text, "blt") != NULL) {
			// blt rs, rt, offset
			// slt at, rs, rt
			// bne at, zero, offset
			char* tok = strtok(text, " \t"); //  mnemonic
			
			tok = strtok(NULL, " \t");
			char* rs = tok;
			
			tok = strtok(NULL, " \t");
			char* rt = tok;
			
			tok = strtok(NULL, " \t");
			char* off = tok;
			
			char* sltInstr = calloc(18, sizeof(char));
			strcpy(sltInstr, "slt ");
			strcat(sltInstr, "$at, ");
			strcat(sltInstr, rs);
			strcat(sltInstr, " ");
			char* first = strncat(sltInstr, rt, 3);
			res = parseASM(first, count, symbolz, symbolzSz);
			printResult(out, res);
			count++;
			free(res);
			
			char* bneInstr = calloc(17 + strlen(off), sizeof(char));
			strcpy(bneInstr, "bne ");
			strcat(bneInstr, "$at, ");
			strcat(bneInstr, "$zero, ");
			char* second = strcat(bneInstr, off);
			res = parseASM(second, count, symbolz, symbolzSz);
			printResult(out, res);
			count++;
			free(res);
			
			
			
		}
		else {
		
			res = parseASM(text, count, symbolz, symbolzSz);
			if(res != NULL){
			
				printResult(out, res);
				count++;
			
			clearResult(res);
			free(res);
			}
		}
		
		
	}
	
	
	
	//rewind
	fseek(input, 0, SEEK_SET);
	fgets(text, MAX_INSTRUCTIONS, input);
	
	//skip
	while (text[0] == '#' ||
	       text[0] == '\n') {		   
			fgets(text, MAX_INSTRUCTIONS, input);
	}

	fgets(text, MAX_INSTRUCTIONS, input);
	fprintf(out, "\n");
	
	int totAsciiLen = 0;
	int lastAsciiLen = 0;
	while (strstr(text, ":") != NULL) {
		
		
		bool prevWasAscii = false;
		if(strstr(text, ".word") != NULL) {
					
			if (strstr(text, ",") != NULL) {
				char* tok = strtok(text, " .\t:,");
				tok = strtok(NULL, " .\t:,");
				tok = strtok(NULL, " .\t:,");
				while (tok != NULL) {
					int num = atoi(tok);
					char* numB = toBin(num, 32); //to 32 bit bin
					fprintf(out, numB);
					fprintf(out,"\n");
					free(numB);
					tok = strtok(NULL, " .\t:,");					
				}
			}
			else {
				char* tok = strtok(text, " \t:.");
				tok = strtok(NULL, " \t:.");
				tok = strtok(NULL, " \t:.");
				char* num = calloc(33, sizeof(char));
				strcpy(num, tok);
				tok = strtok(NULL, " \t:.");
				int count;
				if (tok != NULL) {
					count = atoi(tok);
				}
				else {
					count = 1;
				}
				char* bin = toBin(atoi(num), 32);
				for (int i = 0; i < count; i++) {
					fprintf(out, bin);
					fprintf(out, "\n");
		
				}
				free(num);
				free(bin);			
		    }
		}
		
		else if(strstr(text, ".asciiz") != NULL){
			
			char* tok = strtok(text, "\"");
			tok = strtok(NULL, "\"");//this is the word

			lastAsciiLen = asciiToBin(tok, out, totAsciiLen);
			totAsciiLen += lastAsciiLen;
			prevWasAscii = true;
			
			tok = strtok(text, "\"");
			tok = strtok(NULL, "\"");//this is the word		
		}	
		
		
		fgets(text, MAX_INSTRUCTIONS, input);
		if(lastAsciiLen % 4 != 0) {
			if(prevWasAscii && strstr(text, ".asciiz") == NULL) { //if next isnt ascii
				//add padding after last ascii
				char* padding = "00000000";
				for(int i = 0; i < 4 - (lastAsciiLen % 4); i++) {
					fprintf(out, padding);
					totAsciiLen++;
				}
				fprintf(out, "\n");	
				prevWasAscii = false;
			}
		}
	}
	
	clearSymbolz(symbolz, symbolzSz);
	free(symbolz);
	}
	
    fclose(input);
    fclose(out);
}







/**
 * parses each assembly instruction
 */
ParseResult* parseASM(const char* const pASM, int ctr, Symbol** symbolz, int symbolzSz) {
	
   ParseResult* result = calloc(1, sizeof(ParseResult));
   char* copy = pASM;
   result->ASMInstruction = calloc(strlen(pASM) + 1, sizeof(char));
   strcpy(result->ASMInstruction, pASM);//instruction

   result->Opcode = calloc(7, sizeof(char));
   result->RS = calloc(6, sizeof(char));
   result->rsName = calloc(4, sizeof(char));
   result->RT = calloc(6, sizeof(char));
   result->rtName = calloc(4, sizeof(char));
 
   
   char* tok = strtok(copy, " \t"); //  mnemonic
   char* mnemonic = tok;
   result->Mnemonic = calloc(strlen(tok) + 1, sizeof(char));
   strcpy(result->Mnemonic, tok); 


   if (strcmp(mnemonic, "add") == 0 
   || strcmp(mnemonic, "and") == 0 
   || strcmp(mnemonic, "sub") == 0
   || strcmp(mnemonic, "nor") == 0
   || strcmp(mnemonic, "mul") == 0
   || strcmp(mnemonic, "addu") == 0
   || strcmp(mnemonic, "slt") == 0
   || strcmp(mnemonic, "srav") == 0) {

      //R1
      tok = strtok(NULL, " \t");
      result->rdName = calloc(4, sizeof(char));
      strncpy(result->rdName, tok, 3);
      result->rd = FindReg(result->rdName)->RNumber;
      result->RD = calloc(6, sizeof(char));
      strcpy(result->RD, FindReg(result->rdName)->RBinary);
      //R2
      tok = strtok(NULL, " \t");
      result->rsName = calloc(4, sizeof(char));
      strncpy(result->rsName, tok, 3);
      result->rs = FindReg(result->rsName)->RNumber;
      strcpy(result->RS, FindReg(result->rsName)->RBinary);
      //R3
      tok = strtok(NULL, " \t");
      result->rtName = calloc(4, sizeof(char));
      strncpy(result->rtName, tok, 3);
      result->rt = FindReg(result->rtName)->RNumber;
      strcpy(result->RT, FindReg(result->rtName)->RBinary);
      //op
      strcpy(result->Opcode, FindMnem(mnemonic)->Op);
	  //func
      result->Funct = calloc(7, sizeof(char));
      strcpy(result->Funct, FindMnem(result->Mnemonic)->Func);
      //defaults
      result->Imm = 0;
      result->IMM = NULL;
      
      return result;
   }
   
   else if(strcmp(mnemonic, "sra") == 0 
		|| strcmp(mnemonic, "sll") == 0) {
			
	  //R1
      tok = strtok(NULL, " \t");
      result->rdName = calloc(4, sizeof(char));
      strncpy(result->rdName, tok, 3);
      result->rd = FindReg(result->rdName)->RNumber;
      result->RD = calloc(6, sizeof(char));
      strcpy(result->RD, FindReg(result->rdName)->RBinary);
      
      
      //R3
      tok = strtok(NULL, " \t");
      result->rtName = calloc(4, sizeof(char));
      strncpy(result->rtName, tok, 3);
      result->rt = FindReg(result->rtName)->RNumber;
      strcpy(result->RT, FindReg(result->rtName)->RBinary);
      //op
      strcpy(result->Opcode, FindMnem(mnemonic)->Op);
	  //func
      result->Funct = calloc(7, sizeof(char));
      strcpy(result->Funct, FindMnem(result->Mnemonic)->Func);
      //defaults
      result->Imm = 0;
      result->IMM = NULL;
      
      
      tok = strtok(NULL, " \t");
      char* shiftAmt = toBin(atoi(tok), 5);
	  //R2

      strcpy(result->RS, shiftAmt);
      free(shiftAmt);
	  return result;
		
		
	}

   else if (strcmp(mnemonic, "addi") == 0 
    || strcmp(mnemonic, "addiu") == 0
	|| strcmp(mnemonic, "andi") == 0
	|| strcmp(mnemonic, "slti") == 0
	|| strcmp(mnemonic, "sll") == 0) {
		
		//op
		
	  strcpy(result->Opcode, FindMnem(mnemonic)->Op);
		//R1
      tok = strtok(NULL, " \t");
      strncpy(result->rtName, tok, 3);
      result->rt = FindReg(result->rtName)->RNumber;
      strcpy(result->RT, FindReg(result->rtName)->RBinary);
        //R2 
      tok = strtok(NULL, " \t");
      strncpy(result->rsName, tok, 3);
      result->rs = FindReg(result->rsName)->RNumber;
      strcpy(result->RS, FindReg(result->rsName)->RBinary);
      
		//imm
      tok = strtok(NULL, " \t");
      result->Imm = atoi(tok);

      result->IMM = calloc(17, sizeof(char));
      strcpy(result->IMM, toBin(result->Imm, 16));
      
 
      result->rdName = NULL;
      result->rd = 255;
      result->RD = NULL;
      result->Funct = NULL;
     
      return result;
	}
	
   else if(strcmp(mnemonic, "j") == 0) {
	   strcpy(result->Opcode, FindMnem(mnemonic)->Op);
	
	   // print 10 0s
	   tok = strtok(NULL, " \t"); //label
	   result->IMM = calloc(17, sizeof(char));
	   
	   //change for j 
	   int symOff = findSymbol(symbolz, tok, symbolzSz);
	   strcpy(result->IMM, toBin((symOff / 4), 16));
	   return result;
	   
	   
	   
   }
   
   else if(strcmp(mnemonic, "move") == 0) {
	  //R1
      tok = strtok(NULL, " \t");
      result->rdName = calloc(4, sizeof(char));
      strncpy(result->rdName, tok, 3);
      result->rd = FindReg(result->rdName)->RNumber;
      result->RD = calloc(6, sizeof(char));
      strcpy(result->RD, FindReg(result->rdName)->RBinary);
      //$zero
      result->rsName = calloc(6, sizeof(char));
      strcpy(result->rsName, "$zero");
      result->rs = FindReg(result->rsName)->RNumber;
      strcpy(result->RS, FindReg(result->rsName)->RBinary);
      //R3
      tok = strtok(NULL, " \t");
      result->rtName = calloc(4, sizeof(char));
      strncpy(result->rtName, tok, 3);
      result->rt = FindReg(result->rtName)->RNumber;
      strcpy(result->RT, FindReg(result->rtName)->RBinary);
      //op
      strcpy(result->Opcode, FindMnem(mnemonic)->Op);
	  //func
      result->Funct = calloc(7, sizeof(char));
      strcpy(result->Funct, FindMnem(result->Mnemonic)->Func);
      //defaults
      result->Imm = 0;
      result->IMM = NULL;
      
      return result;
	   
	   
   }
	
   else if (strcmp(mnemonic, "bne") == 0
			|| strcmp(mnemonic, "beq") == 0
			|| strcmp(mnemonic, "blez") == 0
			|| strcmp(mnemonic, "bgtz") == 0){
				
		tok = strtok(NULL, " \t");

		strcpy(result->Opcode, FindMnem(mnemonic)->Op);
		strcpy(result->rsName, tok);
		strcpy(result->RS, FindReg(result->rsName)->RBinary);
				
		if(strcmp(mnemonic, "blez") == 0
		|| strcmp(mnemonic, "bgtz") == 0){
		
			strcpy(result->RT, "00000");
			result->IMM = calloc(17, sizeof(char));	
			tok = strtok(NULL, " \t"); //symbol
			int symOff = findSymbol(symbolz, tok, symbolzSz);
			strcpy(result->IMM, toBin((symOff / 4) - ctr - 1, 16));
		}
		else {
			tok = strtok(NULL, " \t");
			strcpy(result->rtName, tok);
			strcpy(result->RT, FindReg(result->rtName)->RBinary);
			
			result->IMM = calloc(17, sizeof(char));
			
			tok = strtok(NULL, " \t"); //symbol
			int symOff = findSymbol(symbolz, tok, symbolzSz);
			strcpy(result->IMM, toBin((symOff / 4) - ctr - 1, 16));
		}
		
		return result;
							
	}
	
	//addi rd, $zero, label
   else if (strcmp(mnemonic, "la") == 0
			|| strcmp(mnemonic, "li") == 0) {
				
		strcpy(result->Opcode, FindMnem(mnemonic)->Op);//op
	    free(result->rsName);
	    result->rsName = calloc(6, sizeof(char));
	    strcpy(result->rsName, "$zero");
		tok = strtok(NULL, " \t");
		free(result->rtName);
		result->rtName = calloc(5, sizeof(char));
		strcpy(result->rtName, tok);
		strcpy(result->RS, FindReg(result->rsName)->RBinary);
		strcpy(result->RT, FindReg(result->rtName)->RBinary);				
		result->IMM = calloc(17, sizeof(char));
		tok = strtok(NULL, " \t");
		
		if(strcmp(mnemonic, "li") == 0) {
			strcpy(result->IMM, toBin(atoi(tok), 16));
		}
		else{	
			int symOff = findSymbol(symbolz, tok, symbolzSz);
			char* b = toBin(symOff, 16);
			strcpy(result->IMM, b);
			free(b);
			
		} 
		return result;
   }

   else if (strcmp(mnemonic, "lui") ==0) {
	
      tok = strtok(NULL, " \t");
      strncpy(result->rtName, tok, 3);
      result->rt = FindReg(result->rtName)->RNumber;
		//imm
      tok  = strtok(NULL, " \t");
      result->Imm = atoi(tok);
      
      strcpy(result->Opcode, FindMnem(mnemonic)->Op);//op
      strcpy(result->RT, FindReg(result->rtName)->RBinary);//bin
      result->IMM = calloc(17, sizeof(char));
      strcpy(result->IMM, toBin(result->Imm, 16));

		//defaultz
	  strcpy(result->RS, "00000\0");
      result->rsName = NULL;
      result->rdName = NULL;
      result->rs = 0;
      result->rd = 255;
      result->Funct = NULL;
      result->RD = NULL;
      
      return result;
   }

   else if (strcmp(mnemonic, "lw")== 0
			|| strcmp(mnemonic, "sw")== 0) {
				
				
	 
	  if(strstr(result->ASMInstruction, "(") != NULL) {  
		  
		  tok = strtok(NULL, " \t");
		  strncpy(result->rtName, tok, 3);
		  result->rt = FindReg(result->rtName)->RNumber;
		  strcpy(result->RT, FindReg(result->rtName)->RBinary);
		  strcpy(result->Opcode, FindMnem(mnemonic)->Op);
		  
		  
		  tok = strtok(NULL, "(");
		  result->Imm = atoi(tok);
		  result->IMM = calloc(17, sizeof(char));
		  char* b = toBin(result->Imm, 16);
		  strcpy(result->IMM, b);
		  free(b);
		  
		  tok = strtok(NULL, " \t");
		  strncpy(result->rsName, tok, 3);
		  result->rs = FindReg(result->rsName)->RNumber;
		  strcpy(result->RS, FindReg(result->rsName)->RBinary);
		  
		  
		  //defaults
		  result->rdName = NULL;
		  result->rd = 255;
		  result->Funct = NULL;
		  result->RD = NULL;
	}
	else {
		tok = strtok(NULL, " \t");
	    strncpy(result->rtName, tok, 3);
	    result->rt = FindReg(result->rtName)->RNumber;
	    strcpy(result->RT, FindReg(result->rtName)->RBinary);
	    strcpy(result->Opcode, FindMnem(mnemonic)->Op);
	   
	    tok = strtok(NULL, " \t");
	    int symOff = findSymbol(symbolz, tok, symbolzSz);
	    result->IMM = calloc(17, sizeof(char));
		strcpy(result->IMM, toBin(symOff, 16));
		
		
		strcpy(result->RS,  FindReg("$zero")->RBinary);
		
	}
     
      return result;
   }
   else if (strcmp(mnemonic, "syscall\n") == 0) {
	   //R1
      result->RD = calloc(21, sizeof(char));
      strcpy(result->RD, "00000000000000000000");
   
      //op
      strcpy(result->Opcode, FindMnem("syscall")->Op);
	  //func
      result->Funct = calloc(7, sizeof(char));
      strcpy(result->Funct, FindMnem("syscall")->Func);
      //defaults
      result->Imm = 0;
      result->IMM = NULL;
     
      return result;
	   
	   
   }
 
   else {	
	          
       free(result->ASMInstruction);
       free(result->Opcode);
       free(result->RS);
       free(result->rsName);
       free(result->RT);
       free(result->rtName);
       free(result);
	   return NULL;
   }											
}


char* toBin(int imm, int bits) {
	int b;
	char* bin = calloc(bits+1, sizeof(char));

	for (int i = bits-1; i >= 0; i--) {
		b = imm >> i;
		if(b & 1) {
			bin[bits - 1 - i] = '1';
		}
		else {
			bin[bits - 1 - i] = '0';
		}
	}
	return bin;
}

int findSymbol(const Symbol** const symbols, char* sym, int sz) {
	
	int symbolLen = strlen(sym);
	if(strstr(sym, "\n") != NULL) {
		symbolLen--;
	}
	
	for(int i = 0; i < sz; i+=sizeof(Symbol*)){
		
		if(strncmp(symbols[i]->symbol, sym, symbolLen) == 0){
			return symbols[i]->off;
		}
	}
	return NULL;
	
	
	
}

/**
 * converts ascii characters to binary
 */
int asciiToBin(char* ascii, FILE* out, int lastAsciiLen) {
	
	
	int count = 0;
	int secondCount = lastAsciiLen;
	int len = strlen(ascii);
	for(int i = 0;i<len; i++){

		
		int temp = (int)ascii[i]; //current character
		//8 bit
		int8_t curr = temp;
		char* b = toBin(curr, 8);
		fprintf(out, b);
		count++;
		secondCount++;
		
		if(i!=0 && count % 4 == 0) {
			fprintf(out, "\n");
		}
		else if(secondCount % 4 == 0) {
				fprintf(out, "\n");	
		}
		free(b);
	}
	
	fprintf(out, "00000000");//null terminator
	count++;
	secondCount++;
	if(count % 4 == 0 || secondCount %4 == 0) {
			fprintf(out, "\n");
	}
	
	return count;
	
	
}

char* trim_space(char *str) {
    char *end;
    /* skip leading whitespace */
    while (isspace(*str)) {
        str = str + 1;
    }
    /* remove trailing whitespace */
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end = end - 1;
    }
    /* write null character */
    *(end+1) = '\0';
    return str;
}

void clearSymbolz(Symbol** symbolz, int symSz) {
	for(int i = 0; i < symSz; i+=sizeof(Symbol*)) {
		free(symbolz[i]->symbol);
		free(symbolz[i]);	
	
	}
}

