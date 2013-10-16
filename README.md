PROJECT
==================================================================

	Gina Guerrero

	CpSc6120 - Fall 2013

	Research: Rigging 3D Eyes Using Real-Time Data

	C++(Tracker's API & Custom Client)/Python (Maya Scripts)
	Modified Dikablis/Vicon Tracker 2.0/Maya 2013



DESCRIPTION
==================================================================

	Rigging 3D eyes in maya with the use of Dikablis/Vicon

	Vicon SDK 1.53.0 2013/05/23 Release for Linux 64bit
	Python Scripting in Maya
	
	Usage: viconmaya [read/write to file [nolog | logonly] [Maya IP, if nolog]


FILES
==================================================================
	viconmaya.cpp
	vicon.h
	tcp.h, tcp.cpp
	ViconData.h, ViconData.cpp
	VQueue.h, VQueue.cpp



MISC FILES
==================================================================
	README.md
	Makefile
	ViconDataStreamSDK_CPP.so, DebugServices.so (& other SO files)	


SAMPLE OUTPUT FILES
==================================================================
	From Vicon:
	Frame_Number,FramesPerSecond,Head,TranslateX,TranslateY,TranslateZ,Occluded,RotateX,RotateY,RotateZ,Occluded,[Eye-R,X,Occluded,Y,Occluded,RayX,Occluded,RayY,Occluded,RayZ,Occluded,][Eye-L,X,Occluded,Y,Occluded,RayX,Occluded,RayY,Occluded,RayZ,Occluded,][EyePG,(Eye-R)PositionX,PositionY,PositionZ,Occluded,GazeX,GazeY,GazeZ,Occluded,(Eye-L)PositionX,PositionY,PositionZ,Occluded,GazeX,GazeY,GazeZ,Occluded,]


TOTAL FILE COUNT
==================================================================
----?
