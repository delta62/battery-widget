CC	=	gcc
CFLAGS	=	-o bat
FILES	=	bat.c

test:	bat.c
	${CC} ${CFLAGS} ${FILES}

clean:
	rm bat
