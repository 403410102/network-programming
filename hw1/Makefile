include ../Make.defines

all:	tcpservfork tcpservselect

tcpservfork:	tcpservfork.o sigchldwaitpid.o
		${CC} ${CFLAGS} -o $@ tcpservfork.o sigchldwaitpid.o ${LIBS}

tcpservselect:	tcpservselect.o
		${CC} ${CFLAGS} -o $@ tcpservselect.o ${LIBS}

clean:
		rm -f tcpservfork tcpservselect ${CLEANFILES}
