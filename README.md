# Real-time-ORBSLAM3-algorithm-between-RPI-Thymio-PC
A.0.2 Install and Run ORBSLAM on PC

In order to run our code for ORBSLAM, which receives the camera feed from the RPI and sends commands to RPI to send them to Thymio-II you need to follow the following steps;

(1) Install Dependencies :

sudo apt-get install build-essential

sudo apt-get install cmake git libgtk2.0-dev

sudo apt-get install pkg-config libavcodec-dev libavformat-dev libswscale-dev

sudo apt-get install python-dev python-numpy libtbb2

sudo apt-get install libtbb-dev libjpeg-dev libpng-dev libtiff-dev 

sudo apt-get install libjasper-dev libdc1394-22-dev

(2) Install OPenCV3 

git clone https://github.com/opencv/opencv.git

git chekcout 3.3.1

mkdir build

cd build

cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..

make -j3

make install

(3) Install Eigen3

sudo apt install libeigen3-dev 

(4) Install Pangolin

git clone https://github.com/stevenlovegrove/Pangolin.git

cd Pangolin

mkdir build

cd build

cmake .. -DCMAKE_BUILD_TYPE=Release

make -j 3

make install

(5) Now, go to the ORBSLAM3 directory and build the project (./build.sh ).

(6) To run the project, go to ( steps )

Please note that the files ( Optimizer.cc , monoeuroc.cc ) have been edited to achieve the purpose of the project. The rest is the same as the original ORBSLAM3 code here (https://github.com/UZ-SLAMLab/ORB_SLAM3)


