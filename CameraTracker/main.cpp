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
#include <iostream>
#include <vector>
#include <map>

// tag detection
#include <DetectChilitags.hpp>

#include "inputParsers.h"
#include "data/CTagCoords.h"

using namespace cv;
using namespace std;

int usageScenario1(int argc, char *argv[]);
void displayUsage(char* _execName);
void getTagsFromDetector( chilitags::DetectChilitags& _detector, tListOfTags& _listFoundTags );

#ifndef UNIT_TESTING

//  alternative entry point when unit testing is enabled is located in
//      unittests/CTestCase.cpp
//

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);

    return usageScenario1(argc, argv);
}

#endif


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
    map<int,ct::CTagCoords> mapWorldPoints;
    Mat cameraMatrix = Mat::eye(3,3, CV_64F);
    Mat distCoeffs = Mat::zeros(8,1,CV_64F);
    // next parameters change over time
    Mat rMat = Mat::zeros(3,3,CV_64F);
    Mat tMat = Mat::zeros(1,3,CV_64F);

    // read config file with 3D points corespondances
    load3dPoints(inFilePtsGlobalCoords, mapWorldPoints);
    loadCameraParams(inFileIntrinsicParams, cameraMatrix, distCoeffs, rMat, tMat);

    // get video file
    CvCapture* v = cvCaptureFromAVI(inFileVideoStream.c_str());
    if(!v){
        LOG(FATAL) << "Could not open video file " << inFileVideoStream;
        return 1;
    }

    // main loop
    {
        IplImage* frame = cvQueryFrame(v);
        chilitags::DetectChilitags detectChilitags(&frame);
        tListOfTags foundTags;

        // helper structures for main loop
        unsigned long frameNo = cvGetCaptureProperty(v, CV_CAP_PROP_POS_FRAMES),
                      endFrame = cvGetCaptureProperty(v, CV_CAP_PROP_FRAME_COUNT);

        while(frameNo < endFrame){
            frame = cvQueryFrame(v);
            detectChilitags.update();
            // find markers
            getTagsFromDetector(detectChilitags, foundTags);
        }
    }

    // tag detection setup

}

void getTagsFromDetector( chilitags::DetectChilitags& _detector, tListOfTags& _listFoundTags ){
    _listFoundTags.clear();

}



void displayUsage(char* _execName){
    cout << "Usage" << endl;
    cout << "\t" << _execName << " <in_tagPtsFile> <in_intrinsicFile> <in_vidoeFile> <out_cameraCoords>" << endl;
}

