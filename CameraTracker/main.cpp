/*******************************************************************************
*   Copyright 2013 EPFL                                                        *
*                                                                              *
*   This file is part of ChiliTagsCameraTracker.                               *
*                                                                              *
*   Chilitags is free software: you can redistribute it and/or modify          *
*   it under the terms of the Lesser GNU General Public License as             *
*   published by the Free Software Foundation, either version 3 of the         *
*   License, or (at your option) any later version.                            *
*                                                                              *
*   Chilitags is distributed in the hope that it will be useful,               *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of             *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
*   GNU Lesser General Public License for more details.                        *
*                                                                              *
*   You should have received a copy of the GNU Lesser General Public License   *
*   along with Chilitags.  If not, see <http://www.gnu.org/licenses/>.         *
*                                                                              *
*   Author: Mirko Raca <mirko.raca@epfl.ch>                                    *
*                                                                              *
*******************************************************************************/

#include "globalInclude.h"
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

using namespace cv;
using namespace std;

int usageScenario1(int argc, char *argv[]);
void loadParams(std::string _inFilename, cv::Mat& _camMatrix, cv::Mat& _distCoef, cv::Mat& _rMat, cv::Mat& _tMat);
void load3dPoints(std::string _inFilePtsGlobalCoords, std::map<int, cv::Point3f> mapWorldPoints);
void displayUsage(char* _execName);

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);

    return usageScenario1(argc, argv);
}


int usageScenario1(int argc, char *argv[]){
    LOG(INFO) << "usageScenario1";
    string inFilePtsGlobalCoords, inFileIntrinsicParams, inFileVideoStream;
    string outFileCameraPosition;

    if(argc < 5){
        LOG(FATAL) << "Not enough parametrs, need 4 got " << argc-1;
        cout << "Not enough params" << endl;
        displayUsage(argv[0]);
    }

    inFilePtsGlobalCoords = string(argv[1]);
    inFileIntrinsicParams = string(argv[2]);
    inFileVideoStream = string(argv[3]);
    outFileCameraPosition = string(argv[4]);

    // global variables
    map<int,Point3f> mapWorldPoints;

    // read config file with 3D points corespondances
    load3dPoints(inFilePtsGlobalCoords, mapWorldPoints);
    // read intrinsic parameters file
    // get video file
    // process scene by scene
    // make viewer in processing Video + 3d projection of scene + projection of view
}

void displayUsage(char* _execName){
    cout << "Usage" << endl;
    cout << "\t" << _execName << " <in_tagPtsFile> <in_intrinsicFile> <in_vidoeFile> <out_cameraCoords>" << endl;
}

void load3dPoints(std::string _inFilePtsGlobalCoords, std::map<int, cv::Point3f> mapWorldPoints){
    LOG(INFO) << "load3dPoints";
    ifstream inFile(_inFilePtsGlobalCoords);
    while(!inFile.eof()){

    }
}

void loadParams(std::string _inFilename, cv::Mat& _camMatrix, cv::Mat& _distCoef, cv::Mat& _rMat, cv::Mat& _tMat){
    cv::FileStorage fs;

    if( !fs.open(_inFilename, cv::FileStorage::READ) ){
        LOG(FATAL) << "Could not read from: " << _inFilename;
        return;
    }

    fs["CamMatrix"] >> _camMatrix;
    fs["DistCoef"] >> _distCoef;
    fs["RotMat"] >> _rMat;
    fs["TransMat"] >> _tMat;

    fs.release();
}
