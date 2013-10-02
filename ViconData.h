#ifndef _H_VICONDATA
#define _H_VICONDATA

class ViconData {
public:
	int frameNumber;
	double frameRate;
	double headPosition[3];
	int headPOccluded;
	double headRotation[3];
	int headROccluded;
	double eyeRXY[2];
	int eyeRXYOccluded;
	double eyeRXYZ[3];
	int eyeRXYZOccluded;
	double eyeLXY[2];
	int eyeLXYOccluded;
	double eyeLXYZ[3];
	int eyeLXYZOccluded;
	double eyeRPosition[3];
	int eyeRPOccluded;
	double eyeLPosition[3];
	int eyeLPOccluded;
	double eyeRGaze[3];
	int eyeRGazeOccluded;
	double eyeLGaze[3];
	int eyeLGazeOccluded;
	
	ViconData();
};

#endif
