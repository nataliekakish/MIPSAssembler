assembler: ASMParser.* ParseResult.* DataLookup.* RegisterLookup.*
	
	gcc -o assemble -std=c11 -Wall -W -ggdb3 ASMParser.* ParseResult.* DataLookup.* RegisterLookup.*

