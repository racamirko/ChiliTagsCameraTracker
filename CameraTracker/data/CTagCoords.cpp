#include "CTagCoords.h"

#include "globalInclude.h"

ct::CTagCoords::CTagCoords(){
    LOG(DEBUG) << "TagCoords created";
}

cv::Point3f ct::CTagCoords::getCorner(ct::enumCorners _cornerId){
    return mapCornerCoords[_cornerId];
}

void ct::CTagCoords::setCorner(ct::enumCorners _cornerId, cv::Point3f _pt){
    mapCornerCoords[_cornerId] = _pt;
}
