#include "CTagCoords.h"

#include "globalInclude.h"

ct::CTagCoords::CTagCoords(){
    LOG(INFO) << "TagCoords created";
}

cv::Point3f ct::CTagCoords::getCorner(ct::enumCorners _cornerId){
    return mMapCornerCoords[_cornerId];
}

void ct::CTagCoords::setCorner(ct::enumCorners _cornerId, cv::Point3f _pt){
    mMapCornerCoords[_cornerId] = _pt;
}
