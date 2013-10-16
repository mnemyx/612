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
LDFLAGS = -pthread -L. -lViconDataStreamSDK_CPP -lDebugServices

HFILES 	= vicon.${H} VQueue.${H} ViconData.${H} tcp.${H}
OFILES 	= VQueue.o ViconData.o tcp.o
PROJECT = viconmaya

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

VQueue.o: VQueue.${C} VQueue.${H}
	${CC} $(CFLAGS) -c VQueue.${C}

tcp.o: tcp.${C} tcp.${H}
	${CC} $(CFLAGS) -c tcp.${C}

ViconData.o: ViconData.${C} ViconData.${H}
	${CC} $(CFLAGS) -c ViconData.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm *.o *~ ${PROJECT}
