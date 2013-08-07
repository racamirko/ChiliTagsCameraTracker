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
#include <sstream>

#define BUFFER_SIZE 1500

using namespace std;
using namespace cv;

void load3dPoints(std::string _inFilePtsGlobalCoords, std::map<int, ct::CTagCoords>& _mapWorldPoints){
    LOG(INFO) << "load3dPoints";
    char buffer[BUFFER_SIZE];
    char bufferSubString[BUFFER_SIZE];
    char bufferOneNum[BUFFER_SIZE]; // a bit excesive for a 10char float number...

    _mapWorldPoints.clear();

    ifstream inFile(_inFilePtsGlobalCoords);
    while(!inFile.eof()){
        inFile.getline(buffer, BUFFER_SIZE);
        if(buffer[0] == '#' || buffer[0] == '\n')
            continue;
        // cycle variables
        ct::CTagCoords coords;
        istringstream rawLine(buffer);

        // line sub-section delimited by ";"
        rawLine.getline(bufferSubString, BUFFER_SIZE, ';');
        coords.setId( atoi(bufferSubString) );

        // get 4 corners of the string
        for( int cornerIdx = ct::TC_UPLEFT; cornerIdx <= ct::TC_DOWNLEFT; cornerIdx++ ){
            ct::enumCorners curCorner = static_cast<ct::enumCorners>(cornerIdx);
            Point3f newPoint;
            rawLine.getline(bufferSubString, BUFFER_SIZE, ';');
            // individual points being read
            istringstream numberStream(bufferSubString);
            int coordIndex = 0;
            do {
                numberStream.getline(bufferOneNum, BUFFER_SIZE, ',');
                if( strlen(bufferOneNum) > 0 ){
                    float value = atof(bufferOneNum);
                    switch( coordIndex ){
                        case 0:
                            newPoint.x = value;
                            break;
                        case 1:
                            newPoint.y = value;
                            break;
                        case 2:
                            newPoint.z = value;
                            break;
                    }
                }
                coordIndex++;
            } while ( strlen(bufferOneNum) > 0 );
            coords.setCorner(curCorner, newPoint);
        }
        _mapWorldPoints[coords.getId()] = coords;
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

