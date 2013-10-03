#ifndef _H_VICONDATA
#define _H_VICONDATA

#include "math.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>

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

    void Reset();
	ViconData();
	const ViconData& operator=(const ViconData& source);
	bool operator==(const ViconData& two);
	ViconData(const ViconData& source);

	void PrintData();
	void ConvertEyeData();
};

#endif
