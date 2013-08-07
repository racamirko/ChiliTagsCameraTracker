#include "CTestCase.h"

#include "globalInclude.h"
#include "inputParsers.h"
#include "data/CTagCoords.h"

#include <string>
#include <map>
#include <opencv2/core/core.hpp>

#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>

using namespace std;
using namespace boost::unit_test;
using namespace cv;

CTestCase::CTestCase(){

}

void CTestCase::test_load3dPoints(){
    string inFilename = "/home/raca/data/eyetracking/01_test_exp/ex01_points_coords.csv";
    map<int, ct::CTagCoords> mapCoords;
    load3dPoints(inFilename, mapCoords);
    ct::CTagCoords secondTag = mapCoords[1];
    BOOST_CHECK( secondTag.getId() == 1 );
    BOOST_CHECK( secondTag.getUpLeft() == Point3f(157, 186, 0) );
}

#ifdef UNIT_TESTING

void test_load3dPoints();

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    CTestCase tester;
    google::InitGoogleLogging(argv[0]);
//    framework::master_test_suite().add( BOOST_TEST_CASE( &test_load3dPoints ) );
    framework::master_test_suite().add( BOOST_TEST_CASE(boost::bind( &CTestCase::test_load3dPoints, tester )));
    return 0;
}


#endif
