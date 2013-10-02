#include "vicon.h"

struct viconData {
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
};

viconData *buffer = NULL;


void vicon (std::string hostname) {
    Client MyClient;

    for (int i=0; i!=3; ++i) {

        //logofs << "Connecting to " << hostname << " ..." << std::flush;

        while(!MyClient.IsConnected().Connected) {
            bool ok = false;

            ok = (MyClient.Connect(hostname).Result == Result::Success);

            //if(!ok)
                //logofs << "Warning - connect failed..." << std::endl;

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

          //logofs << std::endl;

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

          std::stringstream datastring;

            // get frame info
            Output_GetFrameNumber _Output_GetFrameNumber = MyClient.GetFrameNumber();
            datastring << _Output_GetFrameNumber.FrameNumber << ",";
            
            Output_GetFrameRate Rate = MyClient.GetFrameRate();
            datastring << Rate.FrameRateHz << ",";

            unsigned int SubjectCount = MyClient.GetSubjectCount().SubjectCount;
            for( unsigned int SubjectIndex = 0 ; SubjectIndex < SubjectCount ; ++SubjectIndex ) {
                std::string SubjectName = MyClient.GetSubjectName( SubjectIndex ).SubjectName;
                if(!SubjectName.compare("Head")) {
                    datastring << SubjectName << ",";

                    unsigned int SegmentCount = MyClient.GetSegmentCount( SubjectName ).SegmentCount;
                    for( unsigned int SegmentIndex = 0 ; SegmentIndex < SegmentCount ; ++SegmentIndex ){

                        std::string SegmentName = MyClient.GetSegmentName( SubjectName, SegmentIndex ).SegmentName;

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

            // Output eye tracker information.
            unsigned int EyeTrackerCount = MyClient.GetEyeTrackerCount().EyeTrackerCount;
            for( unsigned int EyeTrackerIndex = 0 ; EyeTrackerIndex < EyeTrackerCount ; ++EyeTrackerIndex ) {

                Output_GetEyeTrackerGlobalPosition _Output_GetEyeTrackerGlobalPosition = MyClient.GetEyeTrackerGlobalPosition( EyeTrackerIndex );

                datastring << _Output_GetEyeTrackerGlobalPosition.Position[0] << "," << _Output_GetEyeTrackerGlobalPosition.Position[1] << "," << _Output_GetEyeTrackerGlobalPosition.Position[2] << ",";
                datastring << Adapt( _Output_GetEyeTrackerGlobalPosition.Occluded ) << ",";

                Output_GetEyeTrackerGlobalGazeVector _Output_GetEyeTrackerGlobalGazeVector = MyClient.GetEyeTrackerGlobalGazeVector( EyeTrackerIndex );

                datastring << _Output_GetEyeTrackerGlobalGazeVector.GazeVector[0] << "," << _Output_GetEyeTrackerGlobalGazeVector.GazeVector[1] << "," << _Output_GetEyeTrackerGlobalGazeVector.GazeVector[2] << ",";
                datastring << Adapt( _Output_GetEyeTrackerGlobalGazeVector.Occluded );
            }

            std::cout << datastring.str() << std::endl;
        }

        MyClient.DisableSegmentData();
        MyClient.DisableMarkerData();
        MyClient.DisableUnlabeledMarkerData();
        MyClient.DisableDeviceData();

        // Disconnect and dispose
        int t = clock();
        //std::cout << " Disconnecting..." << std::endl;
        MyClient.Disconnect();
        int dt = clock() - t;
        double secs = (double) (dt)/(double)CLOCKS_PER_SEC;
        //std::cout << " Disconnect time = " << secs << " secs" << std::endl;
    }
}


int main( int argc, char* argv[] )
{
    std::filebuf buf;
    buf.open("S014_ONEFOURTHSPEED", std::ios::out);
    std::streambuf* oldbuf = std::cout.rdbuf( &buf ) ;

    vicon("130.127.48.127:801");

    std::cout.rdbuf(oldbuf);

}
