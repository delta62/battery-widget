CC	=	gcc
CFLAGS	=	-std=c11 -Wall -Wextra -Wpedantic -o bat
DFLAGS	=	-g
PFLAGS	=	-O2
FILES	=	bat.c

debug: bat.c
	${CC} ${CFLAGS} ${DFLAGS} ${FILES}

prod:	bat.c
	${CC} ${CFLAGS} ${PFLAGS} ${FILES}

clean:
	rm bat
