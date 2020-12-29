#include "ParseResult.h"
#include <stdlib.h>

/**  Frees the dynamic content of a ParseResult object.
 * 
 *   Pre:  pPR points to a proper ParseResult object.
 *   Post: All of the dynamically-allocated arrays in *pPR have been
 *         deallocated; pointers are NULL, static fields are reset to
 *         default values.
 * 
 *   Comments:
 *     -  The function has no information about whether *pPR has been
 *        allocated dynamically, so it cannot risk attempting to 
 *        deallocate *pPR.
 *     -  The function is intended to provide the user with a simple
 *        way to free memory; the user may or may not reuse *pPR.  So,
 *        the function does set the pointers in *pPR to NULL.
 */
void clearResult(ParseResult* const pPR) {
	
	free(pPR->ASMInstruction);
	pPR->ASMInstruction = NULL;
	free(pPR->Mnemonic);
	pPR->Mnemonic = NULL;
	free(pPR->rdName);
	pPR->rdName = NULL;
	free(pPR->rsName);
	pPR->rsName = NULL;
	free(pPR->rtName);
	pPR->rtName = NULL;
	free(pPR->Opcode);
	pPR->Opcode = NULL;
	free(pPR->Funct);
	pPR->Funct = NULL;
	free(pPR->RD);
	pPR->RD = NULL;
	free(pPR->RS);
	pPR->RS = NULL;
	free(pPR->RT);
	pPR->RT = NULL;
	free(pPR->IMM);
	pPR->IMM = NULL;
	pPR->rd = 255;
	pPR->rs = 255;
	pPR->rt = 255;
	pPR->Imm = 0;

    
}

/**  Prints the contents of a ParseResult object.
 * 
 *   Pre:  Log is open on an output file.
 *         pPR points to a proper ParseResult object.
 */
void printResult(FILE* out, const ParseResult* const result) {
   
      if(result != NULL) {		
			char* mnemonic = result->Mnemonic;				
			//clearResult(res);
			if (strcmp(mnemonic, "add") == 0 
				|| strcmp(mnemonic, "and") == 0 
			    || strcmp(mnemonic, "sub") == 0
		   	    || strcmp(mnemonic, "nor") == 0
		   	    || strcmp(mnemonic, "mul") == 0
		   	    || strcmp(mnemonic, "addu") == 0
				|| strcmp(mnemonic, "slt") == 0
				|| strcmp(mnemonic, "move") == 0) {
					
					fprintf(out, "%s%s%s%s%s%s\n", result->Opcode, result->RS, 
					  result->RT, result->RD, "00000", result->Funct);   
			}
			else if (strcmp(mnemonic, "addi") == 0 
					|| strcmp(mnemonic, "addiu") == 0 
					|| strcmp(mnemonic, "andi") == 0
					|| strcmp(mnemonic, "slti") == 0
					|| strcmp(mnemonic, "bne") == 0
					|| strcmp(mnemonic, "beq") == 0
					|| strcmp(mnemonic, "lui") == 0
					|| strcmp(mnemonic, "lw") == 0
					|| strcmp(mnemonic, "la") == 0
					|| strcmp(mnemonic, "blez") == 0
					|| strcmp(mnemonic, "bgtz") == 0
					|| strcmp(mnemonic, "sw") == 0
					|| strcmp(mnemonic, "li") == 0) {
						
				fprintf(out, "%s%s%s%s\n", result->Opcode, result->RS, result->RT, result->IMM);						
			}
			
			else if (strncmp(mnemonic, "srav", 4) == 0) {
				fprintf(out, "%s%s%s%s%s%s\n", result->Opcode, result->RT, 
					  result->RS, result->RD, "00000", result->Funct);  				
			}
			
			else if (strcmp(mnemonic, "sra") == 0
					|| strcmp(mnemonic, "sll") == 0) {
						
				fprintf(out, "%s%s%s%s%s%s\n", result->Opcode, "00000",result->RT, result->RD, result->RS, result->Funct);						
			}
			else if(strcmp(mnemonic, "nop") == 0){
				fprintf(out, "%s", "00000000000000000000000000000000");
				
			}
			
			//else if (strcmp(mnemonic, "sll") == 0){
				//fprintf(out, "%s%s%s%s%s%s\n", result->Opcode, "00000",result->RT, result->RD, result->RS, result->Funct);						

				
			//}
			
			else if (strcmp(mnemonic, "j") == 0) {
				fprintf(out, "%s%s%s\n", result->Opcode, "0000000000",result->IMM);						

				
			}

			else if (strcmp(mnemonic, "syscall\n")== 0) {
				fprintf(out, "%s%s%s%s%s\n", result->Opcode, result->RS, result->RT, result->RD, result->Funct);
			}
	}		

}
