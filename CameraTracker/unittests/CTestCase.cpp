#include "CTestCase.h"

#include "globalInclude.h"
#include "inputParsers.h"
#include "data/CTagCoords.h"

#include <string>
#include <map>
#include <opencv2/core/core.hpp>

//#include <boost/test/included/unit_test.hpp>
//#include <boost/bind.hpp>

using namespace std;
//using namespace boost::unit_test;
using namespace cv;

CTestCase::CTestCase(){

}

void CTestCase::test_load3dPoints(){
    string inFilename = "/home/raca/data/eyetracking/01_test_exp/ex01_points_coords.csv";
    map<int, ct::CTagCoords> mapCoords;
    // execution
    load3dPoints(inFilename, mapCoords);
    // checks
    ct::CTagCoords secondTag = mapCoords[1];
//    BOOST_CHECK( secondTag.getId() == 1 );
//    BOOST_CHECK( secondTag.getUpLeft() == Point3f(157, 186, 0) );
}

void CTestCase::test_loadCameraParams(){
    // input params
    string inFilename = "/home/raca/data/eyetracking/01_test_exp/ex01_mirko.calib.xml";
    // storing variables
    Mat cameraMatrix = Mat::eye(3,3, CV_64F);
    Mat distCoeffs = Mat::zeros(8,1,CV_64F);
    // next parameters change over time
    Mat rMat = Mat::zeros(3,3,CV_64F);
    Mat tMat = Mat::zeros(1,3,CV_64F);

    // execution
    loadCameraParams(inFilename, cameraMatrix, distCoeffs, rMat, tMat);
    // checks
//    BOOST_CHECK( cameraMatrix.at<double>(0,0) == 1.1430362208216586e+03 );
//    BOOST_CHECK( cameraMatrix.at<double>(1,1) == 1.1454345007726454e+03 );
//    BOOST_CHECK( cameraMatrix.at<double>(2,2) == 1.0 );

//    BOOST_CHECK( distCoeffs.at<double>(0,0) == 3.3186747984434668e-02 );
//    BOOST_CHECK( distCoeffs.at<double>(2,0) == 7.9471592397261466e-04 );

//    BOOST_CHECK( rMat.at<double>(0,0) == 0.0 );

//    BOOST_CHECK( tMat.at<double>(0,0) == 0.0 );
}

//#ifdef UNIT_TESTING

//test_suite* init_unit_test_suite( int argc, char* argv[] )
//{
//    CTestCase tester;
//    google::InitGoogleLogging(argv[0]);
////    framework::master_test_suite().add( BOOST_TEST_CASE( &test_load3dPoints ) );
//    framework::master_test_suite().add( BOOST_TEST_CASE(boost::bind( &CTestCase::test_load3dPoints, tester )));
//    framework::master_test_suite().add( BOOST_TEST_CASE(boost::bind( &CTestCase::test_loadCameraParams, tester )));
//    return 0;
//}


//#endif
