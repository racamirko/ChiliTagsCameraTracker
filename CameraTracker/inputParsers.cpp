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

#include "inputParsers.h"
#include "globalInclude.h"

#include <fstream>

using namespace std;
using namespace cv;

void load3dPoints(std::string _inFilePtsGlobalCoords, std::map<int, cv::Point3f> _mapWorldPoints){
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

