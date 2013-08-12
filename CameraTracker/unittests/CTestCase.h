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
#ifndef CTESTCASE_H
#define CTESTCASE_H

#include <map>
#include "data/CTagCoords.h"

class CTestCase
{
public:
    CTestCase();

    void test_load3dPoints();
    void test_loadCameraParams();


    // helper debug methods
    static void displayWorldCoordinates( std::map<int,ct::CTagCoords>& _coords);
};

#endif // CTESTCASE_H
