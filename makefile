
EXE = glversion 
OBJS = glversion.o 
FILES = glversion.c 
INCLUDES = /usr/X11/include
LIB = /usr/X11/lib 
LFLAGS = -lGL -lX11
CC = cc

.SUFFIXES: .c
prog: ${EXE}

#${EXE}: ${OBJS}
#	${CC} -L${LIB} -o $@ ${OBJS} ${LFLAGS}
#
#${OBJS}:${FILES}
#	 ${CC} -I${INCLUDES} -c ${FILES}
#another way to do it

.c.o:
	 ${CC} -I${INCLUDES} -c $<

${EXE}: ${OBJS}
	${CC} -L${LIB} -o $@ ${OBJS} ${LFLAGS}

clean:
	rm -f ${OBJS} ${EXE}
	make
