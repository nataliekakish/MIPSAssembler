#ifndef ASMPARSER_H
#define ASMPARSER_H
#include "ParseResult.h"
#include "Symbols.h"
/**  Breaks up given MIPS32 assembly instruction and creates a proper 
 *   ParseResult object storing information about that instruction.
 * 
 *   Pre:  pASM points to an array holding the bits (as chars) of a
 *         syntactically valid assembly instruction, whose mnemonic is
 *         one of the following:
 *             add  addi  and  andi  lui  lw  or  ori  sub
 * 
 *   Returns:
 *         A pointer to a proper ParseResult object whose fields have been
 *         correctly initialized to correspond to the target of pASM.
 */
ParseResult* parseASM(const char* const pASM, int ctr, Symbol** symbolz, int SymbolzSz);
char* toBin(int number, int bits);
char* trim_space(char *str);
int findSymbol(const Symbol** const symbols, char* sym, int sz);
ParseResult* parseASM(const char* const pASM, int ctr, Symbol** symbolz, int SymbolzSz);
int asciiToBin(char* ascii, FILE* out, int lastAsciiLen);
void clearSymbolz(Symbol** symbolz, int symSz);
#endif
