######################################################################
#	MakeFile for Borland C/C++ Compiler
#

CC  = bcc32
LN  = bcc32
RL  = brc32
RC  = brcc32

CFLAG = -c -O1 -O2 -Oc -Oi -Ov
LFLAG = -tWD -e$(EXE) -O1 -O2
RFLAG = 

EXE = delcycle.auf
OBJ = delcycle.obj
RES = delcycle.res


all: $(EXE)


$(EXE): $(OBJ) $(RES)
	$(LN) $(LFLAG) $(OBJ)
	$(RL) -fe$(EXE) $(RES)

delcycle.obj: delcycle.c filter.h
	$(CC) $(CFLAG) delcycle.c

$(RES): delcycle.rc
	$(RC) $(RFLAG) delcycle.rc
