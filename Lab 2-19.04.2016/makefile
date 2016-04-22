
EXE = cube
OBJS = main.o makeCube.o 
FILES = main.c makeCube.c 
INCLUDES = /usr/X11R6/include
LIB = /usr/X11R6/lib 
LFLAGS = -lGL -lGLU -lX11
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
