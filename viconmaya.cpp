/**
    C++ client example using sockets
    http://www.binarytides.com/code-a-simple-socket-client-class-in-c/

    Modified - GBG 9/2013 - CpSc6120
**/

#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <unistd.h>
#include <pthread.h>

#include "tcp.h"
#include "ViconData.h"
#include "VQueue.cpp"
#include "vicon.h"

#define MAX_TOKENS_PER_LINE     50
#define MAX_LINE_BUFFER         10000

using namespace std;

typedef VQueue<ViconData> Buffer;

string ViconHost = "130.127.48.127:801";
//string MayaHost = "192.168.2.19";
string MayaHost;
int MayaPort = 7777;

extern "C" void* readFrames(void *arg) {
    Buffer* buffer = (Buffer*) arg;

    ifstream log;
    string line;
    ViconData temp, nullvd;
    int n;

    log.open(buffer->file());

    if(!log.good()) {
        fprintf(stderr, "error opening parameter file %s\n", buffer->file());
        exit(1);
    }

    while(!log.eof()) {
        temp.Reset();

        char buf[MAX_LINE_BUFFER];
        log.getline(buf, MAX_LINE_BUFFER);

        temp.viconMsg = (string)buf;
        temp.SerializeData();

        if(log.eof()) break;

        if(!(temp==nullvd)) {
            //cout << "PRODUCER: " << endl;
            //temp.PrintData();
            buffer->add(temp);
        }
        //sleep(1);
    }

    return 0;
}

extern "C" void* sendFrames(void *arg) {
    Buffer* buffer = (Buffer*) arg;
    char msg[MAX_LINE_BUFFER];
    ViconData frame;
    tcp_client c;
    ofstream log;

    log.open (buffer->file(), ofstream::out | ofstream::app);

    c.conn(MayaHost, MayaPort);

    frame = buffer->get();
    while(!frame.IsEmpty()) {
        if(frame.frameNumber != 0) {
            frame.SaveToFile(&log);
            frame.ToSendToMaya(msg, MAX_LINE_BUFFER);
            c.send_data((string)msg);
        }

        frame = buffer->get();
    }

    log.close();

    return 0;
}

extern "C" void* sendFramesNoLog(void *arg) {
    Buffer* buffer = (Buffer*) arg;
    char msg[MAX_LINE_BUFFER];
    ViconData frame;
    tcp_client c;

    c.conn(MayaHost, MayaPort);

    frame = buffer->get();
    while(!frame.IsEmpty()) {
        if(frame.frameNumber != 0) {
            frame.ToSendToMaya(msg, MAX_LINE_BUFFER);
            c.send_data((string)msg);

            sleep(1);
        }

        frame = buffer->get();
    }

    return 0;
}

extern "C" void* sendFramesLogOnly(void *arg) {
    Buffer* buffer = (Buffer*) arg;
    char msg[MAX_LINE_BUFFER];
    ViconData frame;
    ofstream log;

    log.open (buffer->file(), ofstream::out | ofstream::app);


    frame = buffer->get();
    while(!frame.IsEmpty()) {
        if(frame.frameNumber != 0 ) {
            frame.SaveToFile(&log);
        }

        frame = buffer->get();
    }

    log.close();

    return 0;
}

extern "C" void* vicon (void *arg) {
    Buffer* buffer = (Buffer*) arg;
    Client MyClient;
    ViconData temp;

    for (int i=0; i!=3; ++i) {

        while(!MyClient.IsConnected().Connected) {
            bool ok = false;

            ok = (MyClient.Connect(ViconHost).Result == Result::Success);

            if(ok) { cout << "Connected to Vicon" << endl; }

            sleep(1);
        }

        // Go ahead and enable everything
        MyClient.EnableSegmentData();
        MyClient.EnableMarkerData();
        MyClient.EnableUnlabeledMarkerData();
        MyClient.EnableDeviceData();

        // Set the streaming mode
        // MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPull );
        // MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPullPreFetch );
        // ---- NOTE: Server push seems to be the most stable.
        MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ServerPush );

        // Set axis - want y-up for the sake in Maya
        MyClient.SetAxisMapping( Direction::Forward,
                                  Direction::Up,
                                  Direction::Right );


        size_t FrameRateWindow = 1000; // frames
        size_t Counter = 0;
        clock_t LastTime = clock();

        // Loop until a key is pressed
        while(true) {
          // Get a frame
          //logofs << "Waiting for new frame...";
          while(MyClient.GetFrame().Result != Result::Success) {
            // Sleep a little so that we don't lumber the CPU with a busy poll
            //sleep(1);
          }

          if(++Counter == FrameRateWindow) {
            clock_t Now = clock();
            double FrameRate = (double)(FrameRateWindow * CLOCKS_PER_SEC) / (double)(Now - LastTime);

            time_t rawtime;
            struct tm * timeinfo;
            time ( &rawtime );
            timeinfo = localtime ( &rawtime );

            //logofs << "Frame rate = " << FrameRate << " at " <<  asctime (timeinfo)<< std::endl;

            LastTime = Now;
            Counter = 0;
          }

          stringstream datastring;

            // get frame info
            Output_GetFrameNumber _Output_GetFrameNumber = MyClient.GetFrameNumber();
            datastring << _Output_GetFrameNumber.FrameNumber << ",";

            Output_GetFrameRate Rate = MyClient.GetFrameRate();
            datastring << Rate.FrameRateHz << ",";

            unsigned int SubjectCount = MyClient.GetSubjectCount().SubjectCount;
            for( unsigned int SubjectIndex = 0 ; SubjectIndex < SubjectCount ; ++SubjectIndex ) {
                string SubjectName = MyClient.GetSubjectName( SubjectIndex ).SubjectName;
                if(!SubjectName.compare("Head")) {
                    datastring << SubjectName << ",";

                    unsigned int SegmentCount = MyClient.GetSegmentCount( SubjectName ).SegmentCount;
                    for( unsigned int SegmentIndex = 0 ; SegmentIndex < SegmentCount ; ++SegmentIndex ){

                        string SegmentName = MyClient.GetSegmentName( SubjectName, SegmentIndex ).SegmentName;

                        Output_GetSegmentGlobalTranslation _Output_GetSegmentGlobalTranslation =
                            MyClient.GetSegmentGlobalTranslation( SubjectName, SegmentName );
                        Output_GetSegmentGlobalRotationEulerXYZ _Output_GetSegmentGlobalRotationEulerXYZ =
                            MyClient.GetSegmentGlobalRotationEulerXYZ( SubjectName, SegmentName );

                        datastring << _Output_GetSegmentGlobalTranslation.Translation[0] << "," <<  _Output_GetSegmentGlobalTranslation.Translation[1] << "," << _Output_GetSegmentGlobalTranslation.Translation[2] << ",";
                        datastring << Adapt( _Output_GetSegmentGlobalTranslation.Occluded ) << ",";

                        datastring << _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[0] << "," << _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[1] << "," << _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[2] << ",";
                        datastring << Adapt( _Output_GetSegmentGlobalRotationEulerXYZ.Occluded ) << ",";
                    }
                }
            }

            unsigned int DeviceCount = MyClient.GetDeviceCount().DeviceCount;
            for( unsigned int DeviceIndex = 0 ; DeviceIndex < DeviceCount ; ++DeviceIndex ) {

                // Get the device name and type
                Output_GetDeviceName _Output_GetDeviceName = MyClient.GetDeviceName( DeviceIndex );
                datastring << _Output_GetDeviceName.DeviceName << ",";

                // Count the number of device outputs
                unsigned int DeviceOutputCount = MyClient.GetDeviceOutputCount( _Output_GetDeviceName.DeviceName ).DeviceOutputCount;
                for( unsigned int DeviceOutputIndex = 0 ; DeviceOutputIndex < DeviceOutputCount ; ++DeviceOutputIndex ) {
                  // Get the device output name and unit
                    Output_GetDeviceOutputName _Output_GetDeviceOutputName =
                        MyClient.GetDeviceOutputName( _Output_GetDeviceName.DeviceName, DeviceOutputIndex );

                    unsigned int DeviceOutputSubsamples =
                                 MyClient.GetDeviceOutputSubsamples( _Output_GetDeviceName.DeviceName,
                                                                     _Output_GetDeviceOutputName.DeviceOutputName ).DeviceOutputSubsamples;

                    for( unsigned int DeviceOutputSubsample = 0; DeviceOutputSubsample < DeviceOutputSubsamples; ++DeviceOutputSubsample ) {
                        // Get the device output value
                        Output_GetDeviceOutputValue _Output_GetDeviceOutputValue =
                          MyClient.GetDeviceOutputValue( _Output_GetDeviceName.DeviceName,
                                                         _Output_GetDeviceOutputName.DeviceOutputName,
                                                         DeviceOutputSubsample );

                        //datastring << _Output_GetDeviceOutputName.DeviceOutputName << ",";
                        datastring << _Output_GetDeviceOutputValue.Value << ",";
                        //datastring << Adapt( _Output_GetDeviceOutputName.DeviceOutputUnit ) << " "
                        datastring << Adapt( _Output_GetDeviceOutputValue.Occluded )  << ",";

                    }
                }
            }

            datastring << "EyePG,";

            // Output eye tracker information.
            unsigned int EyeTrackerCount = MyClient.GetEyeTrackerCount().EyeTrackerCount;
            for( unsigned int EyeTrackerIndex = 0 ; EyeTrackerIndex < EyeTrackerCount ; ++EyeTrackerIndex ) {

                Output_GetEyeTrackerGlobalPosition _Output_GetEyeTrackerGlobalPosition = MyClient.GetEyeTrackerGlobalPosition( EyeTrackerIndex );

                datastring << _Output_GetEyeTrackerGlobalPosition.Position[0] << "," << _Output_GetEyeTrackerGlobalPosition.Position[1] << "," << _Output_GetEyeTrackerGlobalPosition.Position[2] << ",";
                datastring << Adapt( _Output_GetEyeTrackerGlobalPosition.Occluded ) << ",";

                Output_GetEyeTrackerGlobalGazeVector _Output_GetEyeTrackerGlobalGazeVector = MyClient.GetEyeTrackerGlobalGazeVector( EyeTrackerIndex );

                datastring << _Output_GetEyeTrackerGlobalGazeVector.GazeVector[0] << "," << _Output_GetEyeTrackerGlobalGazeVector.GazeVector[1] << "," << _Output_GetEyeTrackerGlobalGazeVector.GazeVector[2] << ",";
                datastring << Adapt( _Output_GetEyeTrackerGlobalGazeVector.Occluded ) << ",";
            }

            temp.viconMsg = datastring.str();
            temp.SerializeData();

            buffer->add(temp);

        }

        MyClient.DisableSegmentData();
        MyClient.DisableMarkerData();
        MyClient.DisableUnlabeledMarkerData();
        MyClient.DisableDeviceData();

        // Disconnect and dispose
        int t = clock();
        MyClient.Disconnect();
        int dt = clock() - t;
        double secs = (double) (dt)/(double)CLOCKS_PER_SEC;
    }

    return 0;
}


int main(int argc , char *argv[]) {
    if(argc > 4 || argc < 2) { cout << "usage: ./viconmaya [logname to write to | logname to read from] [logonly | nolog] [maya address]" << endl; exit(1); }

    cout << "Hit CTRL+C to end program" << endl;
    string arg = argv[2];

    VQueue<ViconData> buffer(20000, argv[1]);

    if(argc == 4) MayaHost = argv[3];
    else MayaHost = "";

    pthread_t prod;
    pthread_t cons;

    if(argc == 2) {
        pthread_create(&prod, 0, vicon, &buffer);
        pthread_create(&cons, 0, sendFrames, &buffer);
    } else if( arg == "nolog" && argc == 4 ) {
        pthread_create(&prod, 0, readFrames, &buffer);
        pthread_create(&cons, 0, sendFramesNoLog, &buffer);
    } else if( arg == "logonly" && argc == 3)  {
        pthread_create(&prod, 0, vicon, &buffer);
        pthread_create(&cons, 0, sendFramesLogOnly, &buffer);
    }

    void* resultp;
    pthread_join(prod, &resultp);
    pthread_join(cons, &resultp);



    return 0;
}
