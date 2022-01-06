/**
* This file is part of ORB-SLAM3
*
* Copyright (C) 2017-2020 Carlos Campos, Richard Elvira, Juan J. Gómez Rodríguez, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
* Copyright (C) 2014-2016 Raúl Mur-Artal, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
*
* ORB-SLAM3 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM3 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
* the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with ORB-SLAM3.
* If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>
#include <ctime>
#include<opencv2/core/core.hpp>
#include<opencv2/opencv_modules.hpp>
#include<System.h>
#include <blc.h>
using namespace std;



int main(int argc, char **argv){  
    if(argc < 3){
        cerr << endl << "Usage: ./mono_euroc path_to_vocabulary path_to_settings" << endl;
        return 1;
    }

    bool bFileName= (((argc-3) % 2) == 1);
    string file_name;
    if (bFileName)
    {
        file_name = string(argv[argc-1]);
        cout << "file name: " << file_name << endl;
    }

    //***** IMAGE CONNECTION *****//
    blc_array_tcp4_client client_image("192.168.0.51", "31440");
    fprintf(stderr, "Connected_image\nPress 'q' to quit\n");
    //***** COMMANDS CONNECTION *****//
     blc_array_tcp4_client client_commands("192.168.0.51", "31441");
     fprintf(stderr, "Connected°commands\nPress 'q' to quit\n");
    // ****** END OF CONNECTION DEF *****//

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM3::System SLAM( argv[1], argv[2], ORB_SLAM3::System::MONOCULAR, true);
    auto start = std::chrono::steady_clock::now();

    const int FRAME_HEIGHT = 480;
    const int FRAME_WIDTH = 640;
    cv::Mat im = cv::Mat::zeros(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC1);

    client_image.data = im.data;
    client_image.size = FRAME_HEIGHT*FRAME_WIDTH;


     double tframe;
    // Main loop

while(1){
        //add your code here
int i=0;
//cv::Mat Tcw = SLAM.TrackMonocular(im,tframe);

        for(i=0;i<5;i++){
        tframe = (chrono::steady_clock::now() - start).count()/1e9;
        cout << tframe << endl;

              // Pass the image to the SLAM system

               client_image.recv_data();
               SLAM.TrackMonocular(im,tframe);
             //  cout<<Tcw.at<double>(1,4);
             //  cout<<Tcw<<endl;

             client_commands.chars[0]=100;
             client_commands.chars[1]=100;
             client_commands.send_data();
        }
        for(i=5;i<15;i++){
        tframe = (chrono::steady_clock::now() - start).count()/1e9;
        cout << tframe << endl;

              // Pass the image to the SLAM system

               client_image.recv_data();
               SLAM.TrackMonocular(im,tframe);
               cv::Mat Tcw = SLAM.TrackMonocular(im,tframe);
             //  cout<<Tcw.at<double>(1,4);
               cout<<Tcw<<endl;

             client_commands.chars[0]=100;
             client_commands.chars[1]=100;
             client_commands.send_data();
        }
        for(i=15;i<20;i++){
        tframe = (chrono::steady_clock::now() - start).count()/1e9;
        cout << tframe << endl;

              // Pass the image to the SLAM system

               client_image.recv_data();
               SLAM.TrackMonocular(im,tframe);
               cv::Mat Tcw = SLAM.TrackMonocular(im,tframe);
             //  cout<<Tcw.at<double>(1,4);
               cout<<Tcw<<endl;

             client_commands.chars[0]=2*16-100;
             client_commands.chars[1]=100;
             client_commands.send_data();
        }

        for(i=20;i<25;i++){
        tframe = (chrono::steady_clock::now() - start).count()/1e9;
        cout << tframe << endl;

              // Pass the image to the SLAM system

               client_image.recv_data();
               SLAM.TrackMonocular(im,tframe);
               cv::Mat Tcw = SLAM.TrackMonocular(im,tframe);
             //  cout<<Tcw.at<double>(1,4);
               cout<<Tcw<<endl;

             client_commands.chars[0]=100;
             client_commands.chars[1]=2*16-100;
             client_commands.send_data();
        }
        for(i=25;i<35;i++){
        tframe = (chrono::steady_clock::now() - start).count()/1e9;
        cout << tframe << endl;

              // Pass the image to the SLAM system

               client_image.recv_data();
               SLAM.TrackMonocular(im,tframe);
               cv::Mat Tcw = SLAM.TrackMonocular(im,tframe);
             //  cout<<Tcw.at<double>(1,4);
               cout<<Tcw<<endl;

             client_commands.chars[0]=-100;
             client_commands.chars[1]=-100;
             client_commands.send_data();
        }
        for(i=35;i<40;i++){
        tframe = (chrono::steady_clock::now() - start).count()/1e9;
        cout << tframe << endl;

              // Pass the image to the SLAM system

               client_image.recv_data();
               SLAM.TrackMonocular(im,tframe);
             //  cout<<Tcw.at<double>(1,4);
             //  cout<<Tcw<<endl;

             client_commands.chars[0]=-100;
             client_commands.chars[1]=-100;
             client_commands.send_data();
        }

    }

    // Stop all threads
   SLAM.Shutdown();

   //uncommment this to return everything back to zero

    // Save camera trajectory
    if (bFileName)
    {
        const string kf_file =  "kf_" + string(argv[argc-1]) + ".txt";
        const string f_file =  "f_" + string(argv[argc-1]) + ".txt";
        SLAM.SaveTrajectoryEuRoC(f_file);
        SLAM.SaveKeyFrameTrajectoryEuRoC(kf_file);
    }
    else
    {
        SLAM.SaveTrajectoryEuRoC("CameraTrajectory.txt");
        SLAM.SaveKeyFrameTrajectoryEuRoC("KeyFrameTrajectory.txt");
        printf("trajectory saved - el mfrod y3ni - ");
    }


    return 0;
}

