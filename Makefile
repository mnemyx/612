#----------------------------------------------
#  Makefile for CpSc6120 - Vicon to Maya Client
#----------------------------------------------
#
#  Gina Guerrero
#  Fall 2013
#

CC      = g++
C	= cpp
H	= h
CFLAGS 	= -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -Wall
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -Wall
  endif
endif

HFILES 	= tcp.${H} 
OFILES 	= tcp.o 
PROJECT = viconmaya

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

tcp.o: tcp.${C} tcp.${H}
	${CC} $(CFLAGS) -c tcp.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm *.o *~ ${PROJECT}
