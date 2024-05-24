CC = gcc
CFLAGS = -g

SRC = main.c dictionary.c entity.c attribute.c

TARGET = a.out

all: ${TARGET}

${TARGET}: ${SRC}
	${CC} ${CFLAGS} ${SRC} -o ${TARGET}

clean:
	rm -f ${TARGET}
