#include "vicon.h"

int viconConnect (std::string hostname, Client *myclient) {
	while(!myclient->IsConnected()->Connected) {
		
	}
}

int main( int argc, char* argv[] )
{
  
  // Make a new client
  Client MyClient;

  for(int i=0; i != 3; ++i) // repeat to check disconnecting doesn't wreck next connect
  {
    // Connect to a server
    std::cout << "Connecting to " << HostName << " ..." << std::flush;
    while( !MyClient.IsConnected().Connected )
    {
      // Direct connection

      bool ok = false;
      if(ConnectToMultiCast)
      {
        // Multicast connection
        ok = ( MyClient.ConnectToMulticast( HostName, MulticastAddress ).Result == Result::Success );

      }
      else
      {
        ok =( MyClient.Connect( HostName ).Result == Result::Success );
      }
      if(!ok)
      {
        std::cout << "Warning - connect failed..." << std::endl;
      }


      std::cout << ".";
  #ifdef WIN32
      Sleep( 200 );
  #else
      sleep(1);
  #endif
    }
    std::cout << std::endl;

    // Enable some different data types
    MyClient.EnableSegmentData();
    MyClient.EnableMarkerData();
    MyClient.EnableUnlabeledMarkerData();
    MyClient.EnableDeviceData();

    std::cout << "Segment Data Enabled: "          << Adapt( MyClient.IsSegmentDataEnabled().Enabled )         << std::endl;
    std::cout << "Marker Data Enabled: "           << Adapt( MyClient.IsMarkerDataEnabled().Enabled )          << std::endl;
    std::cout << "Unlabeled Marker Data Enabled: " << Adapt( MyClient.IsUnlabeledMarkerDataEnabled().Enabled ) << std::endl;
    std::cout << "Device Data Enabled: "           << Adapt( MyClient.IsDeviceDataEnabled().Enabled )          << std::endl;

    // Set the streaming mode
    //MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPull );
    // MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPullPreFetch );
    MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ServerPush );

    // Set the global up axis
    MyClient.SetAxisMapping( Direction::Forward, 
                             Direction::Left, 
                             Direction::Up ); // Z-up
    // MyClient.SetGlobalUpAxis( Direction::Forward, 
    //                           Direction::Up, 
    //                           Direction::Right ); // Y-up

    Output_GetAxisMapping _Output_GetAxisMapping = MyClient.GetAxisMapping();
    std::cout << "Axis Mapping: X-" << Adapt( _Output_GetAxisMapping.XAxis ) 
                           << " Y-" << Adapt( _Output_GetAxisMapping.YAxis ) 
                           << " Z-" << Adapt( _Output_GetAxisMapping.ZAxis ) << std::endl;

    // Discover the version number
    Output_GetVersion _Output_GetVersion = MyClient.GetVersion();
    std::cout << "Version: " << _Output_GetVersion.Major << "." 
                             << _Output_GetVersion.Minor << "." 
                             << _Output_GetVersion.Point << std::endl;

    if( EnableMultiCast )
    {
      assert( MyClient.IsConnected().Connected );
      MyClient.StartTransmittingMulticast( HostName, MulticastAddress );
    }

    size_t FrameRateWindow = 1000; // frames
    size_t Counter = 0;
    clock_t LastTime = clock();
    // Loop until a key is pressed
  #ifdef WIN32
    while( !Hit() )
  #else
    while( true)
  #endif
    {
      // Get a frame
      output_stream << "Waiting for new frame...";
      while( MyClient.GetFrame().Result != Result::Success )
      {
        // Sleep a little so that we don't lumber the CPU with a busy poll
        #ifdef WIN32
          Sleep( 200 );
        #else
          sleep(1);
        #endif

        output_stream << ".";
      }
      output_stream << std::endl;
      if(++Counter == FrameRateWindow)
      {
        clock_t Now = clock();
        double FrameRate = (double)(FrameRateWindow * CLOCKS_PER_SEC) / (double)(Now - LastTime);
        if(!LogFile.empty())
        {
          time_t rawtime;
          struct tm * timeinfo;
          time ( &rawtime );
          timeinfo = localtime ( &rawtime );

          ofs << "Frame rate = " << FrameRate << " at " <<  asctime (timeinfo)<< std::endl;
        }

        LastTime = Now;
        Counter = 0;
      }

      // Get the frame number
      Output_GetFrameNumber _Output_GetFrameNumber = MyClient.GetFrameNumber();
      output_stream << "Frame Number: " << _Output_GetFrameNumber.FrameNumber << std::endl;

      Output_GetFrameRate Rate = MyClient.GetFrameRate();
      std::cout << "Frame rate: "           << Rate.FrameRateHz          << std::endl;
      // Get the timecode
      Output_GetTimecode _Output_GetTimecode  = MyClient.GetTimecode();

      output_stream << "Timecode: "
                << _Output_GetTimecode.Hours               << "h "
                << _Output_GetTimecode.Minutes             << "m " 
                << _Output_GetTimecode.Seconds             << "s "
                << _Output_GetTimecode.Frames              << "f "
                << _Output_GetTimecode.SubFrame            << "sf "
                << Adapt( _Output_GetTimecode.FieldFlag ) << " " 
                << _Output_GetTimecode.Standard            << " " 
                << _Output_GetTimecode.SubFramesPerFrame   << " " 
                << _Output_GetTimecode.UserBits            << std::endl << std::endl;

      // Get the latency
      output_stream << "Latency: " << MyClient.GetLatencyTotal().Total << "s" << std::endl;
      
      for( unsigned int LatencySampleIndex = 0 ; LatencySampleIndex < MyClient.GetLatencySampleCount().Count ; ++LatencySampleIndex )
      {
        std::string SampleName  = MyClient.GetLatencySampleName( LatencySampleIndex ).Name;
        double      SampleValue = MyClient.GetLatencySampleValue( SampleName ).Value;

        output_stream << "  " << SampleName << " " << SampleValue << "s" << std::endl;
      }
      output_stream << std::endl;

      // Count the number of subjects
      unsigned int SubjectCount = MyClient.GetSubjectCount().SubjectCount;
      output_stream << "Subjects (" << SubjectCount << "):" << std::endl;
      for( unsigned int SubjectIndex = 0 ; SubjectIndex < SubjectCount ; ++SubjectIndex )
      {
        output_stream << "  Subject #" << SubjectIndex << std::endl;

        // Get the subject name
        std::string SubjectName = MyClient.GetSubjectName( SubjectIndex ).SubjectName;
        output_stream << "    Name: " << SubjectName << std::endl;

        // Get the root segment
        std::string RootSegment = MyClient.GetSubjectRootSegmentName( SubjectName ).SegmentName;
        output_stream << "    Root Segment: " << RootSegment << std::endl;

        // Count the number of segments
        unsigned int SegmentCount = MyClient.GetSegmentCount( SubjectName ).SegmentCount;
        output_stream << "    Segments (" << SegmentCount << "):" << std::endl;
        for( unsigned int SegmentIndex = 0 ; SegmentIndex < SegmentCount ; ++SegmentIndex )
        {
          output_stream << "      Segment #" << SegmentIndex << std::endl;

          // Get the segment name
          std::string SegmentName = MyClient.GetSegmentName( SubjectName, SegmentIndex ).SegmentName;
          output_stream << "        Name: " << SegmentName << std::endl;

          // Get the segment parent
          std::string SegmentParentName = MyClient.GetSegmentParentName( SubjectName, SegmentName ).SegmentName;
          output_stream << "        Parent: " << SegmentParentName << std::endl;

          // Get the segment's children
          unsigned int ChildCount = MyClient.GetSegmentChildCount( SubjectName, SegmentName ).SegmentCount;
          output_stream << "     Children (" << ChildCount << "):" << std::endl;
          for( unsigned int ChildIndex = 0 ; ChildIndex < ChildCount ; ++ChildIndex )
          {
            std::string ChildName = MyClient.GetSegmentChildName( SubjectName, SegmentName, ChildIndex ).SegmentName;
            output_stream << "       " << ChildName << std::endl;
          }

          // Get the static segment translation
          Output_GetSegmentStaticTranslation _Output_GetSegmentStaticTranslation = 
            MyClient.GetSegmentStaticTranslation( SubjectName, SegmentName );
          output_stream << "        Static Translation: (" << _Output_GetSegmentStaticTranslation.Translation[ 0 ]  << ", " 
                                                       << _Output_GetSegmentStaticTranslation.Translation[ 1 ]  << ", " 
                                                       << _Output_GetSegmentStaticTranslation.Translation[ 2 ]  << ")" << std::endl;

          // Get the static segment rotation in helical co-ordinates
          Output_GetSegmentStaticRotationHelical _Output_GetSegmentStaticRotationHelical = 
            MyClient.GetSegmentStaticRotationHelical( SubjectName, SegmentName );
          output_stream << "        Static Rotation Helical: (" << _Output_GetSegmentStaticRotationHelical.Rotation[ 0 ]     << ", " 
                                                            << _Output_GetSegmentStaticRotationHelical.Rotation[ 1 ]     << ", " 
                                                            << _Output_GetSegmentStaticRotationHelical.Rotation[ 2 ]     << ")" << std::endl;

          // Get the static segment rotation as a matrix
          Output_GetSegmentStaticRotationMatrix _Output_GetSegmentStaticRotationMatrix = 
            MyClient.GetSegmentStaticRotationMatrix( SubjectName, SegmentName );
          output_stream << "        Static Rotation Matrix: (" << _Output_GetSegmentStaticRotationMatrix.Rotation[ 0 ]     << ", " 
                                                           << _Output_GetSegmentStaticRotationMatrix.Rotation[ 1 ]     << ", " 
                                                           << _Output_GetSegmentStaticRotationMatrix.Rotation[ 2 ]     << ", " 
                                                           << _Output_GetSegmentStaticRotationMatrix.Rotation[ 3 ]     << ", " 
                                                           << _Output_GetSegmentStaticRotationMatrix.Rotation[ 4 ]     << ", " 
                                                           << _Output_GetSegmentStaticRotationMatrix.Rotation[ 5 ]     << ", " 
                                                           << _Output_GetSegmentStaticRotationMatrix.Rotation[ 6 ]     << ", " 
                                                           << _Output_GetSegmentStaticRotationMatrix.Rotation[ 7 ]     << ", " 
                                                           << _Output_GetSegmentStaticRotationMatrix.Rotation[ 8 ]     << ")" << std::endl;

          // Get the static segment rotation in quaternion co-ordinates
          Output_GetSegmentStaticRotationQuaternion _Output_GetSegmentStaticRotationQuaternion = 
            MyClient.GetSegmentStaticRotationQuaternion( SubjectName, SegmentName );
          output_stream << "        Static Rotation Quaternion: (" << _Output_GetSegmentStaticRotationQuaternion.Rotation[ 0 ]     << ", " 
                                                               << _Output_GetSegmentStaticRotationQuaternion.Rotation[ 1 ]     << ", " 
                                                               << _Output_GetSegmentStaticRotationQuaternion.Rotation[ 2 ]     << ", " 
                                                               << _Output_GetSegmentStaticRotationQuaternion.Rotation[ 3 ]     << ")" << std::endl;

          // Get the static segment rotation in EulerXYZ co-ordinates
          Output_GetSegmentStaticRotationEulerXYZ _Output_GetSegmentStaticRotationEulerXYZ = 
            MyClient.GetSegmentStaticRotationEulerXYZ( SubjectName, SegmentName );
          output_stream << "        Static Rotation EulerXYZ: (" << _Output_GetSegmentStaticRotationEulerXYZ.Rotation[ 0 ]     << ", " 
                                                             << _Output_GetSegmentStaticRotationEulerXYZ.Rotation[ 1 ]     << ", " 
                                                             << _Output_GetSegmentStaticRotationEulerXYZ.Rotation[ 2 ]     << ")" << std::endl;

          // Get the global segment translation
          Output_GetSegmentGlobalTranslation _Output_GetSegmentGlobalTranslation = 
            MyClient.GetSegmentGlobalTranslation( SubjectName, SegmentName );
          output_stream << "        Global Translation: (" << _Output_GetSegmentGlobalTranslation.Translation[ 0 ]  << ", " 
                                                       << _Output_GetSegmentGlobalTranslation.Translation[ 1 ]  << ", " 
                                                       << _Output_GetSegmentGlobalTranslation.Translation[ 2 ]  << ") " 
                                                       << Adapt( _Output_GetSegmentGlobalTranslation.Occluded ) << std::endl;

          // Get the global segment rotation in helical co-ordinates
          Output_GetSegmentGlobalRotationHelical _Output_GetSegmentGlobalRotationHelical = 
            MyClient.GetSegmentGlobalRotationHelical( SubjectName, SegmentName );
          output_stream << "        Global Rotation Helical: (" << _Output_GetSegmentGlobalRotationHelical.Rotation[ 0 ]     << ", " 
                                                            << _Output_GetSegmentGlobalRotationHelical.Rotation[ 1 ]     << ", " 
                                                            << _Output_GetSegmentGlobalRotationHelical.Rotation[ 2 ]     << ") " 
                                                            << Adapt( _Output_GetSegmentGlobalRotationHelical.Occluded ) << std::endl;

          // Get the global segment rotation as a matrix
          Output_GetSegmentGlobalRotationMatrix _Output_GetSegmentGlobalRotationMatrix = 
            MyClient.GetSegmentGlobalRotationMatrix( SubjectName, SegmentName );
          output_stream << "        Global Rotation Matrix: (" << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 0 ]     << ", " 
                                                           << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 1 ]     << ", " 
                                                           << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 2 ]     << ", " 
                                                           << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 3 ]     << ", " 
                                                           << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 4 ]     << ", " 
                                                           << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 5 ]     << ", " 
                                                           << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 6 ]     << ", " 
                                                           << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 7 ]     << ", " 
                                                           << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 8 ]     << ") " 
                                                           << Adapt( _Output_GetSegmentGlobalRotationMatrix.Occluded ) << std::endl;

          // Get the global segment rotation in quaternion co-ordinates
          Output_GetSegmentGlobalRotationQuaternion _Output_GetSegmentGlobalRotationQuaternion = 
            MyClient.GetSegmentGlobalRotationQuaternion( SubjectName, SegmentName );
          output_stream << "        Global Rotation Quaternion: (" << _Output_GetSegmentGlobalRotationQuaternion.Rotation[ 0 ]     << ", " 
                                                               << _Output_GetSegmentGlobalRotationQuaternion.Rotation[ 1 ]     << ", " 
                                                               << _Output_GetSegmentGlobalRotationQuaternion.Rotation[ 2 ]     << ", " 
                                                               << _Output_GetSegmentGlobalRotationQuaternion.Rotation[ 3 ]     << ") " 
                                                               << Adapt( _Output_GetSegmentGlobalRotationQuaternion.Occluded ) << std::endl;

          // Get the global segment rotation in EulerXYZ co-ordinates
          Output_GetSegmentGlobalRotationEulerXYZ _Output_GetSegmentGlobalRotationEulerXYZ = 
            MyClient.GetSegmentGlobalRotationEulerXYZ( SubjectName, SegmentName );
          output_stream << "        Global Rotation EulerXYZ: (" << _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[ 0 ]     << ", " 
                                                             << _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[ 1 ]     << ", " 
                                                             << _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[ 2 ]     << ") " 
                                                             << Adapt( _Output_GetSegmentGlobalRotationEulerXYZ.Occluded ) << std::endl;

          // Get the local segment translation
          Output_GetSegmentLocalTranslation _Output_GetSegmentLocalTranslation = 
            MyClient.GetSegmentLocalTranslation( SubjectName, SegmentName );
          output_stream << "        Local Translation: (" << _Output_GetSegmentLocalTranslation.Translation[ 0 ]  << ", " 
                                                      << _Output_GetSegmentLocalTranslation.Translation[ 1 ]  << ", " 
                                                      << _Output_GetSegmentLocalTranslation.Translation[ 2 ]  << ") " 
                                                      << Adapt( _Output_GetSegmentLocalTranslation.Occluded ) << std::endl;

          // Get the local segment rotation in helical co-ordinates
          Output_GetSegmentLocalRotationHelical _Output_GetSegmentLocalRotationHelical = 
            MyClient.GetSegmentLocalRotationHelical( SubjectName, SegmentName );
          output_stream << "        Local Rotation Helical: (" << _Output_GetSegmentLocalRotationHelical.Rotation[ 0 ]     << ", " 
                                                           << _Output_GetSegmentLocalRotationHelical.Rotation[ 1 ]     << ", " 
                                                           << _Output_GetSegmentLocalRotationHelical.Rotation[ 2 ]     << ") " 
                                                           << Adapt( _Output_GetSegmentLocalRotationHelical.Occluded ) << std::endl;

          // Get the local segment rotation as a matrix
          Output_GetSegmentLocalRotationMatrix _Output_GetSegmentLocalRotationMatrix = 
            MyClient.GetSegmentLocalRotationMatrix( SubjectName, SegmentName );
          output_stream << "        Local Rotation Matrix: (" << _Output_GetSegmentLocalRotationMatrix.Rotation[ 0 ]     << ", " 
                                                          << _Output_GetSegmentLocalRotationMatrix.Rotation[ 1 ]     << ", " 
                                                          << _Output_GetSegmentLocalRotationMatrix.Rotation[ 2 ]     << ", " 
                                                          << _Output_GetSegmentLocalRotationMatrix.Rotation[ 3 ]     << ", " 
                                                          << _Output_GetSegmentLocalRotationMatrix.Rotation[ 4 ]     << ", " 
                                                          << _Output_GetSegmentLocalRotationMatrix.Rotation[ 5 ]     << ", " 
                                                          << _Output_GetSegmentLocalRotationMatrix.Rotation[ 6 ]     << ", " 
                                                          << _Output_GetSegmentLocalRotationMatrix.Rotation[ 7 ]     << ", " 
                                                          << _Output_GetSegmentLocalRotationMatrix.Rotation[ 8 ]     << ") " 
                                                          << Adapt( _Output_GetSegmentLocalRotationMatrix.Occluded ) << std::endl;

          // Get the local segment rotation in quaternion co-ordinates
          Output_GetSegmentLocalRotationQuaternion _Output_GetSegmentLocalRotationQuaternion = 
            MyClient.GetSegmentLocalRotationQuaternion( SubjectName, SegmentName );
          output_stream << "        Local Rotation Quaternion: (" << _Output_GetSegmentLocalRotationQuaternion.Rotation[ 0 ]     << ", " 
                                                              << _Output_GetSegmentLocalRotationQuaternion.Rotation[ 1 ]     << ", " 
                                                              << _Output_GetSegmentLocalRotationQuaternion.Rotation[ 2 ]     << ", " 
                                                              << _Output_GetSegmentLocalRotationQuaternion.Rotation[ 3 ]     << ") " 
                                                              << Adapt( _Output_GetSegmentLocalRotationQuaternion.Occluded ) << std::endl;

          // Get the local segment rotation in EulerXYZ co-ordinates
          Output_GetSegmentLocalRotationEulerXYZ _Output_GetSegmentLocalRotationEulerXYZ = 
            MyClient.GetSegmentLocalRotationEulerXYZ( SubjectName, SegmentName );
          output_stream << "        Local Rotation EulerXYZ: (" << _Output_GetSegmentLocalRotationEulerXYZ.Rotation[ 0 ]     << ", " 
                                                            << _Output_GetSegmentLocalRotationEulerXYZ.Rotation[ 1 ]     << ", " 
                                                            << _Output_GetSegmentLocalRotationEulerXYZ.Rotation[ 2 ]     << ") " 
                                                            << Adapt( _Output_GetSegmentLocalRotationEulerXYZ.Occluded ) << std::endl;
        }

        // Count the number of markers
        unsigned int MarkerCount = MyClient.GetMarkerCount( SubjectName ).MarkerCount;
        output_stream << "    Markers (" << MarkerCount << "):" << std::endl;
        for( unsigned int MarkerIndex = 0 ; MarkerIndex < MarkerCount ; ++MarkerIndex )
        {
          // Get the marker name
          std::string MarkerName = MyClient.GetMarkerName( SubjectName, MarkerIndex ).MarkerName;

          // Get the marker parent
          std::string MarkerParentName = MyClient.GetMarkerParentName( SubjectName, MarkerName ).SegmentName;

          // Get the global marker translation
          Output_GetMarkerGlobalTranslation _Output_GetMarkerGlobalTranslation =
            MyClient.GetMarkerGlobalTranslation( SubjectName, MarkerName );

          output_stream << "      Marker #" << MarkerIndex            << ": "
                                        << MarkerName             << " ("
                                        << _Output_GetMarkerGlobalTranslation.Translation[ 0 ]  << ", "
                                        << _Output_GetMarkerGlobalTranslation.Translation[ 1 ]  << ", "
                                        << _Output_GetMarkerGlobalTranslation.Translation[ 2 ]  << ") "
                                        << Adapt( _Output_GetMarkerGlobalTranslation.Occluded ) << std::endl;
        }
      }

      // Get the unlabeled markers
      unsigned int UnlabeledMarkerCount = MyClient.GetUnlabeledMarkerCount().MarkerCount;
      output_stream << "    Unlabeled Markers (" << UnlabeledMarkerCount << "):" << std::endl;
      for( unsigned int UnlabeledMarkerIndex = 0 ; UnlabeledMarkerIndex < UnlabeledMarkerCount ; ++UnlabeledMarkerIndex )
      { 
        // Get the global marker translation
        Output_GetUnlabeledMarkerGlobalTranslation _Output_GetUnlabeledMarkerGlobalTranslation =
          MyClient.GetUnlabeledMarkerGlobalTranslation( UnlabeledMarkerIndex );

        output_stream << "      Marker #" << UnlabeledMarkerIndex   << ": ("
                                      << _Output_GetUnlabeledMarkerGlobalTranslation.Translation[ 0 ] << ", "
                                      << _Output_GetUnlabeledMarkerGlobalTranslation.Translation[ 1 ] << ", "
                                      << _Output_GetUnlabeledMarkerGlobalTranslation.Translation[ 2 ] << ")" << std::endl;
      }

      // Count the number of devices
      unsigned int DeviceCount = MyClient.GetDeviceCount().DeviceCount;
      output_stream << "  Devices (" << DeviceCount << "):" << std::endl;
      for( unsigned int DeviceIndex = 0 ; DeviceIndex < DeviceCount ; ++DeviceIndex )
      {
        output_stream << "    Device #" << DeviceIndex << ":" << std::endl;

        // Get the device name and type
        Output_GetDeviceName _Output_GetDeviceName = MyClient.GetDeviceName( DeviceIndex );
        output_stream << "      Name: " << _Output_GetDeviceName.DeviceName << std::endl;
        output_stream << "      Type: " << Adapt( _Output_GetDeviceName.DeviceType ) << std::endl;

        // Count the number of device outputs
        unsigned int DeviceOutputCount = MyClient.GetDeviceOutputCount( _Output_GetDeviceName.DeviceName ).DeviceOutputCount;
        output_stream << "      Device Outputs (" << DeviceOutputCount << "):" << std::endl;
        for( unsigned int DeviceOutputIndex = 0 ; DeviceOutputIndex < DeviceOutputCount ; ++DeviceOutputIndex )
        {
          // Get the device output name and unit
          Output_GetDeviceOutputName _Output_GetDeviceOutputName = 
            MyClient.GetDeviceOutputName( _Output_GetDeviceName.DeviceName, DeviceOutputIndex );

          unsigned int DeviceOutputSubsamples = 
                         MyClient.GetDeviceOutputSubsamples( _Output_GetDeviceName.DeviceName, 
                                                             _Output_GetDeviceOutputName.DeviceOutputName ).DeviceOutputSubsamples;

          output_stream << "      Device Output #" << DeviceOutputIndex << ":" << std::endl;
          output_stream << "      Samples (" << DeviceOutputSubsamples << "):" << std::endl;

          for( unsigned int DeviceOutputSubsample = 0; DeviceOutputSubsample < DeviceOutputSubsamples; ++DeviceOutputSubsample )
          {
            output_stream << "        Sample #" << DeviceOutputSubsample << ":" << std::endl;

            // Get the device output value
            Output_GetDeviceOutputValue _Output_GetDeviceOutputValue = 
              MyClient.GetDeviceOutputValue( _Output_GetDeviceName.DeviceName, 
                                             _Output_GetDeviceOutputName.DeviceOutputName, 
                                             DeviceOutputSubsample );

            output_stream << "          '" << _Output_GetDeviceOutputName.DeviceOutputName          << "' "
                                          << _Output_GetDeviceOutputValue.Value                    << " " 
                                          << Adapt( _Output_GetDeviceOutputName.DeviceOutputUnit ) << " " 
                                          << Adapt( _Output_GetDeviceOutputValue.Occluded )        << std::endl;
          }
        }
      }

      // Output the force plate information.
      unsigned int ForcePlateCount = MyClient.GetForcePlateCount().ForcePlateCount;
      output_stream << "  Force Plates: (" << ForcePlateCount << ")" << std::endl;

      for( unsigned int ForcePlateIndex = 0 ; ForcePlateIndex < ForcePlateCount ; ++ForcePlateIndex )
      {
        output_stream << "    Force Plate #" << ForcePlateIndex << ":" << std::endl;

        unsigned int ForcePlateSubsamples = MyClient.GetForcePlateSubsamples( ForcePlateIndex ).ForcePlateSubsamples;

		output_stream << "    Samples (" << ForcePlateSubsamples << "):" << std::endl;

        for( unsigned int ForcePlateSubsample = 0; ForcePlateSubsample < ForcePlateSubsamples; ++ForcePlateSubsample )
        {
          output_stream << "      Sample #" << ForcePlateSubsample << ":" << std::endl;

          Output_GetGlobalForceVector _Output_GetForceVector = MyClient.GetGlobalForceVector( ForcePlateIndex, ForcePlateSubsample );
          output_stream << "        Force (" << _Output_GetForceVector.ForceVector[ 0 ] << ", ";
          output_stream << _Output_GetForceVector.ForceVector[ 1 ] << ", ";
          output_stream << _Output_GetForceVector.ForceVector[ 2 ] << ")" << std::endl;

          Output_GetGlobalMomentVector _Output_GetMomentVector = 
                                         MyClient.GetGlobalMomentVector( ForcePlateIndex, ForcePlateSubsample );
          output_stream << "        Moment (" << _Output_GetMomentVector.MomentVector[ 0 ] << ", ";
          output_stream << _Output_GetMomentVector.MomentVector[ 1 ] << ", ";
          output_stream << _Output_GetMomentVector.MomentVector[ 2 ] << ")" << std::endl;

          Output_GetGlobalCentreOfPressure _Output_GetCentreOfPressure = 
                                             MyClient.GetGlobalCentreOfPressure( ForcePlateIndex, ForcePlateSubsample );
          output_stream << "        CoP (" << _Output_GetCentreOfPressure.CentreOfPressure[ 0 ] << ", ";
          output_stream << _Output_GetCentreOfPressure.CentreOfPressure[ 1 ] << ", ";
          output_stream << _Output_GetCentreOfPressure.CentreOfPressure[ 2 ] << ")" << std::endl;
        }
      }

      // Output eye tracker information.
      unsigned int EyeTrackerCount = MyClient.GetEyeTrackerCount().EyeTrackerCount;
      output_stream << "  Eye Trackers: (" << EyeTrackerCount << ")" << std::endl;

      for( unsigned int EyeTrackerIndex = 0 ; EyeTrackerIndex < EyeTrackerCount ; ++EyeTrackerIndex )
      {
        output_stream << "    Eye Tracker #" << EyeTrackerIndex << ":" << std::endl;

        Output_GetEyeTrackerGlobalPosition _Output_GetEyeTrackerGlobalPosition = MyClient.GetEyeTrackerGlobalPosition( EyeTrackerIndex );

        output_stream << "      Position (" << _Output_GetEyeTrackerGlobalPosition.Position[ 0 ] << ", ";
        output_stream << _Output_GetEyeTrackerGlobalPosition.Position[ 1 ] << ", ";
        output_stream << _Output_GetEyeTrackerGlobalPosition.Position[ 2 ] << ") ";
        output_stream << Adapt( _Output_GetEyeTrackerGlobalPosition.Occluded ) << std::endl;

        Output_GetEyeTrackerGlobalGazeVector _Output_GetEyeTrackerGlobalGazeVector = MyClient.GetEyeTrackerGlobalGazeVector( EyeTrackerIndex );

        output_stream << "      Gaze (" << _Output_GetEyeTrackerGlobalGazeVector.GazeVector[ 0 ] << ", ";
        output_stream << _Output_GetEyeTrackerGlobalGazeVector.GazeVector[ 1 ] << ", ";
        output_stream << _Output_GetEyeTrackerGlobalGazeVector.GazeVector[ 2 ] << ") ";
        output_stream << Adapt( _Output_GetEyeTrackerGlobalGazeVector.Occluded ) << std::endl;
      }
    }

    if( EnableMultiCast )
    {
      MyClient.StopTransmittingMulticast();
    }
    MyClient.DisableSegmentData();
    MyClient.DisableMarkerData();
    MyClient.DisableUnlabeledMarkerData();
    MyClient.DisableDeviceData();

    // Disconnect and dispose
    int t = clock();
    std::cout << " Disconnecting..." << std::endl;
    MyClient.Disconnect();
    int dt = clock() - t;
    double secs = (double) (dt)/(double)CLOCKS_PER_SEC;
    std::cout << " Disconnect time = " << secs << " secs" << std::endl;

  }
}
