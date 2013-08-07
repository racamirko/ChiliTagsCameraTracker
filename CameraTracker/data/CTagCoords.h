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

#ifndef CTAGCOORDS_H
#define CTAGCOORDS_H

#include <opencv2/core/core.hpp>
#include <map>

namespace ct {

    typedef enum { TC_UPLEFT = 0, TC_UPRIGHT, TC_DOWNRIGHT, TC_DOWNLEFT} enumCorners;

    class CTagCoords
    {
    protected:
        int mTagId;
        std::map<enumCorners, cv::Point3f> mMapCornerCoords;

        cv::Point3f getCorner(ct::enumCorners _cornerId);
        void setCorner(ct::enumCorners _cornerId, cv::Point3f _pt);

    public:
        CTagCoords();

        void setId(int _tagId){ mTagId = _tagId; }
        int getId(){ return mTagId; }

        cv::Point3f getUpLeft(){ return getCorner(TC_UPLEFT); }
        cv::Point3f getUpRight(){ return getCorner(TC_UPRIGHT); }
        cv::Point3f getDownLeft(){ return getCorner(TC_DOWNLEFT); }
        cv::Point3f getDownRight(){ return getCorner(TC_DOWNRIGHT); }

        void setUpLeft(cv::Point3f _pt){ setCorner(TC_UPLEFT, _pt); }
        void setUpRight(cv::Point3f _pt){ setCorner(TC_UPRIGHT, _pt); }
        void setDownLeft(cv::Point3f _pt){ setCorner(TC_DOWNLEFT, _pt); }
        void setDownRight(cv::Point3f _pt){ setCorner(TC_DOWNRIGHT, _pt); }
    };

}

#endif // CTAGCOORDS_H
