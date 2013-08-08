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
#include <fstream>
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
void getTagsFromDetector( tListOfTags& _listFoundTags, int _maxTagScope = 1024 );
void calcCorespondance(tListOfTags& _foundTags,map<int,ct::CTagCoords>& _mapWorldPoints, cv::Mat& _camMat, cv::Mat& _distMat, cv::Mat& _initR, cv::Mat& _initT, cv::Mat& _rMat, cv::Mat& _tMat, bool _bReturnRotVec );

// output values
void initOutput(std::string _outputFilename);
void writeData( unsigned long frameNo, cv::Mat& _rMat, cv::Mat& _tMat );
void finalizeOutput();
std::ofstream gOutputHnd; // Global output file handler

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

    // fixed parameters (for now)
    const int maxNumTags = 4;

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

        // prepare output
        initOutput(outFileCameraPosition);

        // helper structures for main loop
        unsigned long frameNo = cvGetCaptureProperty(v, CV_CAP_PROP_POS_FRAMES),
                      endFrame = cvGetCaptureProperty(v, CV_CAP_PROP_FRAME_COUNT);

        while(frameNo < endFrame){
            frame = cvQueryFrame(v);
            frameNo = cvGetCaptureProperty(v, CV_CAP_PROP_POS_FRAMES);
            detectChilitags.update();
            // find markers
            getTagsFromDetector(foundTags, maxNumTags);

            Mat newR = Mat::zeros(3,3,CV_64F);
            Mat newT = Mat::zeros(1,3,CV_64F);
            calcCorespondance(foundTags, mapWorldPoints, cameraMatrix, distCoeffs,
                              rMat, tMat, newR, newT, false);
            // store values
            writeData( frameNo, newR, newT );
            // update loop values
            rMat = newR; tMat = newT;
        }

        finalizeOutput();
    }

}

void getTagsFromDetector( tListOfTags& _listFoundTags, int _maxTagScope ){
    _listFoundTags.clear();

    if( _maxTagScope > 1024 ){
        LOG(ERROR) << "Maximum tag scope incorrect, max corrected to 1024";
        _maxTagScope = 1024;
    }

    for (int tagId = 0; tagId < _maxTagScope; ++tagId) {
        chilitags::Chilitag curTag(tagId);

        if (curTag.isPresent()) {
            DLOG(INFO) << "Tag [" << tagId << "] found!";
            _listFoundTags.push_back(pair<int, chilitags::Quad>(tagId, curTag.getCorners()));
        }
    }
}

void calcCorespondance(tListOfTags& _foundTags,map<int,ct::CTagCoords>& _mapWorldPoints,
                       cv::Mat& _camMat, cv::Mat& _distMat,
                       cv::Mat& _initR, cv::Mat& _initT,
                       cv::Mat& _rMat, cv::Mat& _tMat,
                       bool _bReturnRotVec )
{
    vector<Point3f> objPts;
    vector<Point2f> imgPts;
    Mat tmpRvec;

    _tMat = _initT.clone();

    if( _foundTags.size() == 0 ){
        LOG(INFO) << "No tags found, replicating previous findings";
        _rMat = _initR.clone();
        return;
    }

    objPts.reserve(_foundTags.size());
    imgPts.reserve(_foundTags.size());

    // fill object points & img points
    for( pair<int, chilitags::Quad> curTag : _foundTags ){
        for( int cornerIdx = 0; cornerIdx < 4; ++cornerIdx ){
            ct::enumCorners cornerLabel = static_cast<ct::enumCorners>(cornerIdx);
            imgPts.push_back( curTag.second[cornerIdx] );
            objPts.push_back( _mapWorldPoints[curTag.first].getCorner(cornerLabel) );
        }
    }

    bool useInitialGuess = false;

    if(sum(_initT)[0] == 0){
        useInitialGuess = true;
    }

    if( useInitialGuess ){
        if( !_bReturnRotVec){
            Rodrigues(_initR, tmpRvec);
        } else {
            tmpRvec = _initR.clone();
        }
    }

    // solve equation
    solvePnP(objPts, imgPts, _camMat, _distMat, tmpRvec, _tMat, useInitialGuess);

    // TODO: this should be made more memory-friendly
    if( !_bReturnRotVec )
        Rodrigues(tmpRvec, _rMat); // vector -> matrix
    else
        _rMat = tmpRvec.clone();
}

void writeData( unsigned long frameNo, cv::Mat& _rMat, cv::Mat& _tMat ){
    if( !gOutputHnd.is_open() ){
        LOG(FATAL) << "Output stream not open for writing";
        throw Exception();
    }
    gOutputHnd << frameNo << ", ";
    for( auto iterMat = _rMat.begin<double>(); iterMat != _rMat.end<double>(); ++iterMat ){
        gOutputHnd << *iterMat << ", ";
    }
    for( auto iterMat = _tMat.begin<double>(); iterMat != _tMat.end<double>(); ++iterMat ){
        gOutputHnd << *iterMat;
        if( iterMat + 1 != _tMat.end() )
            gOutputHnd << ", ";
        else
            gOutputHnd << endl;
    }
}

void initOutput(std::string _outputFilename){
    gOutputHnd.open( _outputFilename.c_str() );
    if( !gOutputHnd.is_open() ){
        LOG(FATAL) << "Unable to open " << _outputFilename;
        throw Exception();
    }
    gOutputHnd << "# frameNo, [9 fields of rot matrix], [3 fields of trans matrix]" << endl;
}

void finalizeOutput(){
    if(gOutputHnd.is_open())
        gOutputHnd.close();
}

void displayUsage(char* _execName){
    cout << "Usage" << endl;
    cout << "\t" << _execName << " <in_tagPtsFile> <in_intrinsicFile> <in_vidoeFile> <out_cameraCoords>" << endl;
}

