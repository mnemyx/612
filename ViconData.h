#ifndef _H_VICONDATA
#define _H_VICONDATA

#include "math.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <fstream>

#define MAX_TOKENS_PER_LINE     50
#define MAX_LINE_BUFFER         10000

class ViconData {
public:
	int frameNumber;
	double frameRate;

	int blink;

	double headPosition[3];
	int headPOccluded;
	double headRotation[3];
	int headROccluded;

	double eyeRXY[2];
	int eyeRXYOccluded[2];
	double eyeRXYZ[3];
	int eyeRXYZOccluded[3];
	double eyeLXY[2];
	int eyeLXYOccluded[2];
	double eyeLXYZ[3];
	int eyeLXYZOccluded[3];

	double eyeRPosition[3];
	int eyeRPOccluded;
	double eyeLPosition[3];
	int eyeLPOccluded;

	double eyeRGaze[3];
	int eyeRGazeOccluded;
	double eyeLGaze[3];
	int eyeLGazeOccluded;

	double eyeRAngles[2];
	double eyeLAngles[2];

	int hasEyeRAngles;
	int hasEyeLAngles;

	std::string viconMsg;

    void Reset();
	ViconData();
	const ViconData& operator=(const ViconData& source);
	bool operator==(const ViconData& two);
	ViconData(const ViconData& source);

	void PrintData();
	void ConvertEyeData();
	void ToSendToMaya(char *msgbuffer, int len);
	void SerializeData();
	void SaveToFile(std::ofstream* log);
	int getBoolean(char *tf);
	int IsEmpty();
};

#endif
