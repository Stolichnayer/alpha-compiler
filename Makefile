PHASE4_SOURCE = parser.c scanner.c quad.c targetCode.c
PHASE5_SOURCE = avm.c
PHASE4_OUT	  = calc4
PHASE5_OUT	  = calc5
CC            = gcc

all: Phase4 Phase5

Phase4:
	bison --yacc --defines --output=parser.c parser.y
	flex --outfile=scanner.c al.l
	gcc -g -o $(PHASE4_OUT) $(PHASE4_SOURCE) -lm

Phase5:
	gcc -g -o $(PHASE5_OUT) $(PHASE5_SOURCE) -lm

clean:	
		rm -rf $(PHASE4_OUT) $(PHASE5_OUT) parser.c parser.h scanner.c scanner.h code.bin
