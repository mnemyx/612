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
#LDFLAGS 	= -L. -lViconDataStreamSDK_CPP -lDebugServices

HFILES 	= ViconData.${H} tcp.${H} 
OFILES 	= ViconData.o tcp.o 
PROJECT = viconmaya

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

tcp.o: tcp.${C} tcp.${H}
	${CC} $(CFLAGS) -c tcp.${C}

ViconData.o: ViconData.${C} ViconData.${H}
	${CC} $(CFLAGS) -c ViconData.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm *.o *~ ${PROJECT}
