#include "ViconData.h"

using namespace std;

void ViconData::Reset() {
    int i;

    frameNumber = blink = hasEyeLAngles = hasEyeRAngles = 0;
    frameRate = 0.0;

    viconMsg = "";

    headPOccluded = headROccluded =  eyeRPOccluded = eyeLPOccluded = eyeRGazeOccluded = eyeLGazeOccluded = 0;

    for(i = 0; i < 3; i++) {
        headPosition[i] = headRotation[i] = eyeRXYZ[i] = eyeLXYZ[i] = eyeRPosition[i] = eyeLPosition[i] = eyeRGaze[i] = eyeLGaze[i] = eyeLXYOccluded[i] = eyeLXYZOccluded[i] = 0.0;

        if(i < 2) {
            eyeRXY[i] = eyeLXY[i] = eyeRAngles[i] = eyeLAngles[i] = eyeRXYOccluded[i] = eyeRXYZOccluded[i] =  0.0;
        }
    }
}

ViconData::ViconData() {
    Reset();
}

ViconData::ViconData(const ViconData& o) {
    int i;

    frameNumber = o.frameNumber;
    frameRate = o.frameRate;
    blink = o.blink;

    headPOccluded = o.headPOccluded;
    headROccluded = o.headROccluded;
    eyeRPOccluded = o.eyeRPOccluded;
    eyeLPOccluded = o.eyeLPOccluded;
    eyeRGazeOccluded = o.eyeRGazeOccluded;
    eyeLGazeOccluded = o.eyeLGazeOccluded;

    hasEyeLAngles = o.hasEyeLAngles;
    hasEyeRAngles = o.hasEyeRAngles;

    viconMsg = o.viconMsg;

    for(i = 0; i < 3; i++) {
        headPosition[i] = o.headPosition[i];
        headRotation[i] = o.headRotation[i];
        eyeRXYZ[i] = o.eyeRXYZ[i];
        eyeLXYZ[i] = o.eyeLXYZ[i];
        eyeRPosition[i] = o.eyeRPosition[i];
        eyeLPosition[i] = o.eyeLPosition[i];
        eyeRGaze[i] = o.eyeRGaze[i];
        eyeLGaze[i] = o.eyeLGaze[i];
        eyeLXYOccluded[i] = o.eyeLXYOccluded[i];
        eyeLXYZOccluded[i] = o.eyeLXYZOccluded[i];

        if(i < 2) {
            eyeRXY[i] = o.eyeRXY[i];
            eyeLXY[i] = o.eyeLXY[i];
            eyeRAngles[i] = o.eyeRAngles[i];
            eyeLAngles[i] = o.eyeLAngles[i];
            eyeRXYOccluded[i] = o.eyeRXYOccluded[i];
            eyeRXYZOccluded[i] =  o.eyeRXYZOccluded[i];
        }
    }
}

const ViconData& ViconData::operator=(const ViconData& o) {
    ViconData temp(o);

    int i;

    frameNumber = temp.frameNumber;
    frameRate = temp.frameRate;
    blink = temp.blink;

    headPOccluded = temp.headPOccluded;
    headROccluded = temp.headROccluded;
    eyeRPOccluded = temp.eyeRPOccluded;
    eyeLPOccluded = temp.eyeLPOccluded;
    eyeRGazeOccluded = temp.eyeRGazeOccluded;
    eyeLGazeOccluded = temp.eyeLGazeOccluded;

    hasEyeLAngles = temp.hasEyeLAngles;
    hasEyeRAngles = temp.hasEyeRAngles;

    viconMsg = o.viconMsg;

    for(i = 0; i < 3; i++) {
        headPosition[i] = temp.headPosition[i];
        headRotation[i] = temp.headRotation[i];
        eyeRXYZ[i] = temp.eyeRXYZ[i];
        eyeLXYZ[i] = temp.eyeLXYZ[i];
        eyeRPosition[i] = temp.eyeRPosition[i];
        eyeLPosition[i] = temp.eyeLPosition[i];
        eyeRGaze[i] = temp.eyeRGaze[i];
        eyeLGaze[i] = temp.eyeLGaze[i];
        eyeLXYOccluded[i] = temp.eyeLXYOccluded[i];
        eyeLXYZOccluded[i] = temp.eyeLXYZOccluded[i];

        if(i < 2) {
            eyeRXY[i] = temp.eyeRXY[i];
            eyeLXY[i] = temp.eyeLXY[i];
            eyeRAngles[i] = temp.eyeRAngles[i];
            eyeLAngles[i] = temp.eyeLAngles[i];
            eyeRXYOccluded[i] = temp.eyeRXYOccluded[i];
            eyeRXYZOccluded[i] =  temp.eyeRXYZOccluded[i];
        }
    }

    return *this;
}

bool ViconData::operator==(const ViconData& two) {
    return ((frameNumber == two.frameNumber) && (frameRate == two.frameRate) && (blink == two.blink)
            && (headPOccluded == two.headPOccluded) && (headPOccluded == two.headPOccluded) && (headPOccluded == two.headPOccluded)
            && (headROccluded == two.headROccluded) && (eyeRPOccluded == two.eyeRPOccluded) && (eyeLPOccluded == two.eyeLPOccluded)
            && (eyeRGazeOccluded == two.eyeRGazeOccluded) && (eyeLGazeOccluded == two.eyeLGazeOccluded) && (headPosition[0] == two.headPosition[0])
            && (headRotation[0] == two.headRotation[0]) && (eyeRXYZ[0] == two.eyeRXYZ[0]) && (eyeLXYZ[0] == two.eyeLXYZ[0])
            && (eyeRPosition[0] == two.eyeRPosition[0]) && (eyeLPosition[0] == two.eyeLPosition[0]) && (eyeRGaze[0] == two.eyeRGaze[0])
            && (eyeLGaze[0] == two.eyeLGaze[0]) && (eyeLXYOccluded[0] == two.eyeLXYOccluded[0]) && (eyeLXYZOccluded[0] == two.eyeLXYZOccluded[0])
            && (headPosition[1] == two.headPosition[1]) && (headRotation[1] == two.headRotation[1]) && (eyeRXYZ[1] == two.eyeRXYZ[1])
            && (eyeLXYZ[1] == two.eyeLXYZ[1]) && (eyeRPosition[1] == two.eyeRPosition[1]) && (eyeLPosition[1] == two.eyeLPosition[1])
            && (eyeRGaze[1] == two.eyeRGaze[1]) && (eyeLGaze[1] == two.eyeLGaze[1]) && (eyeLXYOccluded[1] == two.eyeLXYOccluded[1])
            && (eyeLXYZOccluded[1] == two.eyeLXYZOccluded[1]) && (headPosition[2] == two.headPosition[2]) && (headRotation[2] == two.headRotation[2])
            && (eyeRXYZ[2] == two.eyeRXYZ[2]) && (eyeLXYZ[2] == two.eyeLXYZ[2]) && (eyeRPosition[2] == two.eyeRPosition[2])
            && (eyeLPosition[2] == two.eyeLPosition[2]) && (eyeRGaze[2] == two.eyeRGaze[2]) && (eyeLGaze[2] == two.eyeLGaze[2])
            && (eyeLXYOccluded[2] == two.eyeLXYOccluded[2]) && (eyeLXYZOccluded[2] == two.eyeLXYZOccluded[2]) && (eyeRXY[0] == two.eyeRXY[0])
            && (eyeLXY[0] == two.eyeLXY[0]) && (eyeRAngles[0] == two.eyeRAngles[0]) && (eyeLAngles[0] == two.eyeLAngles[0])
            && (eyeRXYOccluded[0] == two.eyeRXYOccluded[0]) && (eyeRXYZOccluded[0] ==  two.eyeRXYZOccluded[0]) && (eyeRXY[1] == two.eyeRXY[1])
            && (eyeLXY[1] == two.eyeLXY[1]) && (eyeRAngles[1] == two.eyeRAngles[1]) && (eyeLAngles[1] == two.eyeLAngles[1])
            && (eyeRXYOccluded[1] == two.eyeRXYOccluded[1]) && (eyeRXYZOccluded[1] == two.eyeRXYZOccluded[1])
            && (hasEyeRAngles == two.hasEyeRAngles) && (hasEyeLAngles == two.hasEyeLAngles && (viconMsg == two.viconMsg) && (blink == two.blink)));
}

void ViconData::PrintData() {

    cout << "Frame Number: " << frameNumber << endl << "Frame Rate: "<< frameRate << endl;
    cout << "Blink? " << blink << endl;
    cout << "Vicon Head Position: " << headPOccluded << " (" << headPosition[0] << ", " << headPosition[1] << ", " << headPosition[2] << ")" << endl;
    cout << "Vicon Head Rotaton: " << headROccluded << " (" << headRotation[0] << ", " << headRotation[1] << ", " << headRotation[2] << ")" << endl;
    cout << "Dikablis Eye-R XY: " << "[" << eyeRXYOccluded[0] << ", " << eyeRXYOccluded[1] << "] " << "(" << eyeRXY[0] << ", " << eyeRXY[1] << ")" << endl;
    cout << "Dikablis Eye-R XYZ: " << "[" << eyeRXYZOccluded[0] << ", " << eyeRXYZOccluded[1] << ", " << eyeRXYZOccluded[2] << "] " << "(" << eyeRXYZ[0] << ", " << eyeRXYZ[1] << ", " << eyeRXYZ[2] << ")" << endl;
    cout << "Dikablis Eye-L XY: " << "[" << eyeLXYOccluded[0] << ", " << eyeLXYOccluded[1] << "] " << " (" << eyeLXY[0] << ", " << eyeLXY[1] << ")" << endl;
    cout << "Dikablis Eye-L XYZ: " << "[" << eyeLXYZOccluded[0] << ", " << eyeLXYZOccluded[1] << ", " << eyeLXYZOccluded[2] << "] " << "(" << eyeLXYZ[0] << ", " << eyeLXYZ[1] << ", " << eyeLXYZ[2] << ")" << endl;
    cout << "Vicon Eye-R Position: " << eyeRPOccluded << " (" << eyeRPosition[0] << ", " << eyeRPosition[1] << ", " << eyeRPosition[2] << ")" << endl;
    cout << "Vicon Eye-R Gaze: " << eyeLGazeOccluded << " (" << eyeRGaze[0] << ", " << eyeRGaze[1] << ", " << eyeRGaze[2] << ")" << endl;
    cout << "Vicon Eye-L Position: " << eyeLPOccluded << " (" << eyeLPosition[0] << ", " << eyeLPosition[1] << ", " << eyeLPosition[2] << ")" << endl;
    cout << "Vicon Eye-L Gaze: " << eyeLGazeOccluded << " (" << eyeLGaze[0] << ", " << eyeLGaze[1] << ", " << eyeLGaze[2] << ")" << endl;
    cout << "Calc Eye-R Rotation: " << hasEyeRAngles << " (" << eyeRAngles[0] << ", " << eyeRAngles[1] << ")" << endl;
    cout << "Calc Eye-L Rotation: " << hasEyeLAngles << " (" << eyeLAngles[0] << ", " << eyeLAngles[1] << ")" << endl;
    cout << "Vicon Msg: " << viconMsg << endl << endl;
}

void ViconData::ConvertEyeData() {

    if(!eyeRXYZOccluded[0] && !eyeRXYZOccluded[1] && !eyeRXYZOccluded[2] && eyeRXYZ[2] != 0) {
        eyeRAngles[0] = tan(eyeRXYZ[0] / eyeRXYZ[2]);
        eyeRAngles[1] = tan(eyeRXYZ[1] / eyeRXYZ[2]);
        hasEyeLAngles = 1;
    } else { eyeRAngles[0] = 0; eyeRAngles[1] = 0; hasEyeRAngles = 0; }

    if(!eyeLXYZOccluded[0] && !eyeLXYZOccluded[1] && !eyeLXYZOccluded[2] && eyeLXYZ[2] != 0) {
        eyeLAngles[0] = tan(eyeLXYZ[0] / eyeLXYZ[2]);
        eyeLAngles[1] = tan(eyeLXYZ[1] / eyeLXYZ[2]);
        hasEyeLAngles = 1;
    } else { eyeLAngles[0] = 0; eyeLAngles[1] = 0;  hasEyeLAngles = 0;}

}

void ViconData::ToSendToMaya(char *msgbuffer, int len) {

    char msg[len];

    sprintf(msg, "Frame,%d,Head,%d,%lf,%lf,%lf,Blink,%d,EyeR,%d,%lf,%lf,EyeL,%d,%lf,%lf",
            frameNumber, headROccluded,headRotation[0],headRotation[1], headRotation[2], blink,
            hasEyeRAngles,eyeRAngles[0],eyeRAngles[1],
            hasEyeLAngles,eyeLAngles[0],eyeLAngles[1]);

    strcpy(msgbuffer,msg);
}

int ViconData::IsEmpty() {
    ViconData temp;

    if(*this == temp) return true;
    else return false;
}

void ViconData::SerializeData() {
    char *buf= new char[viconMsg.length() + 1];
    strcpy(buf, viconMsg.c_str());
    // do stuff
    int n = 0;
    int i = 0;
    char *token[MAX_TOKENS_PER_LINE];

    token[0] = strtok(buf, ",");

    // trim out the dummy frames we seem to get
    if(token[0] && atoi(token[0]) < 1000000) {
        for( n = 1; n < MAX_TOKENS_PER_LINE; n++) {
            token[n] = strtok(0, ",");
            if(!token[n]) break;
        }

        if(i < n) {
            // parse data as best as possible.
            frameNumber = atoi(token[i++]);
            frameRate = atoi(token[i++]);

            if(i < n) {
                if(strcmp(token[i],"Head") == 0 && strcmp(token[i+1], "Eye-R") != 0) {
                    headPosition[0] = atof(token[++i]);
                    headPosition[1] = atof(token[++i]);
                    headPosition[2] = atof(token[++i]);

                    headPOccluded = getBoolean(token[++i]);

                    headRotation[0] = atof(token[++i]);
                    headRotation[1] = atof(token[++i]);
                    headRotation[2] = atof(token[++i]);

                    headROccluded = getBoolean(token[++i]);
                }

                if(++i < n) {
                    if(strcmp(token[i],"Eye-R") == 0 && strcmp(token[i+1], "Eye-L") != 0) {
                        eyeRXY[0] = atof(token[++i]);
                        eyeRXYOccluded[0] = getBoolean(token[++i]);
                        eyeRXY[1] = atof(token[++i]);
                        eyeRXYOccluded[1] = getBoolean(token[++i]);

                        eyeRXYZ[0] = atof(token[++i]);
                        eyeRXYZOccluded[0] = getBoolean(token[++i]);
                        eyeRXYZ[1] = atof(token[++i]);
                        eyeRXYZOccluded[1] = getBoolean(token[++i]);
                        eyeRXYZ[2] = atof(token[++i]);
                        eyeRXYZOccluded[2] = getBoolean(token[++i]);
                    } else { blink = true; }
                    if(++i < n) {
                        if(strcmp(token[i],"Eye-L") == 0 && strcmp(token[i+1], "EyePG") != 0) {
                            eyeLXY[0] = atof(token[++i]);
                            eyeLXYOccluded[0] = getBoolean(token[++i]);
                            eyeLXY[1] = atof(token[++i]);
                            eyeLXYOccluded[1] = getBoolean(token[++i]);

                            eyeLXYZ[0] = atof(token[++i]);
                            eyeLXYZOccluded[0] = getBoolean(token[++i]);
                            eyeLXYZ[1] = atof(token[++i]);
                            eyeLXYZOccluded[1] = getBoolean(token[++i]);
                            eyeLXYZ[2] = atof(token[++i]);
                            eyeLXYZOccluded[2] = getBoolean(token[++i]);
                        } else { blink = true; }

                        if(++i < n) {
                            if(strcmp(token[i],"EyePG") == 0 && token[i+1]) {
                                eyeRPosition[0] = atof(token[++i]);
                                eyeRPosition[1] = atof(token[++i]);
                                eyeRPosition[2] = atof(token[++i]);

                                eyeRPOccluded = getBoolean(token[++i]);

                                eyeRGaze[0] = atof(token[++i]);
                                eyeRGaze[1] = atof(token[++i]);
                                eyeRGaze[2] = atof(token[++i]);

                                eyeRGazeOccluded = getBoolean(token[++i]);

                                eyeLPosition[0] = atof(token[++i]);
                                eyeLPosition[1] = atof(token[++i]);
                                eyeLPosition[2] = atof(token[++i]);

                                eyeLPOccluded = getBoolean(token[++i]);

                                eyeLGaze[0] = atof(token[++i]);
                                eyeLGaze[1] = atof(token[++i]);
                                eyeLGaze[2] = atof(token[++i]);

                                eyeLGazeOccluded = getBoolean(token[++i]);
                            }
                        }
                    }
                }
            }
        }
    }

    ConvertEyeData();
    delete [] buf;
}

void ViconData::SaveToFile(ofstream* log) {
    *log << viconMsg << endl;
}


int ViconData::getBoolean(char *tf) {
    if(strcmp(tf, "True") == 0) return 1;
    return 0;
}
