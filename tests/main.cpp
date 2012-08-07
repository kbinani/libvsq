#include "Util.hpp"
#include <string>

using namespace std;

string TestUtil::fixtureRootPath = ".";

int main( int argc, char* argv[] )
{
    string xmlPath = "";
    string currentParse = "";
    for( int i = 1; i < argc; i++ ){
        string s = argv[i];
        if( s.substr( 0, 1 ) == "-" ){
            currentParse = s;
        }else{
            if( currentParse == "-o" ){
                xmlPath = s;
            }else if( currentParse == "-f" ){
                TestUtil::setFixtureRootPath( s );
            }
            currentParse = "";
        }
    }
    CppUnit::TestResult controller;
    CppUnit::TestResultCollector results;
    controller.addListener( &results );

    CppUnit::BriefTestProgressListener progress;
    if( xmlPath.size() == 0 ){
        controller.addListener( &progress );
    }

    CppUnit::TestRunner runner;
    runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() );
    runner.run( controller );

    if( xmlPath.size() == 0 ){
        CppUnit::CompilerOutputter outputter( &results, CppUnit::stdCOut() );
        outputter.write();
    }else{
        std::ofstream ofs( xmlPath.c_str() );
        CppUnit::XmlOutputter outputter( &results, ofs, "UTF-8" );
        outputter.write();
    }

    return results.wasSuccessful() ? 0 : 1;
}
