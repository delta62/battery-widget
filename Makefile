CC	=	gcc
CFLAGS	=	-o bat
FILES	=	bat.c

debug: bat.c
	${CC} ${CFLAGS} -g ${FILES}

prod:	bat.c
	${CC} ${CFLAGS} -O2 ${FILES}

clean:
	rm bat
