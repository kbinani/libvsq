#pragma once

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#define REGISTER_TEST_SUITE( ATestFixtureType )      \
	static CppUnit::AutoRegisterSuite< ATestFixtureType > CPPUNIT_JOIN( testSuite, ATestFixtureType )

class TestUtil
{
private:
	static std::string fixtureRootPath;

public:
	static void setFixtureRootPath(std::string const& fixtureRootPath)
	{
		TestUtil::fixtureRootPath = fixtureRootPath;
	}

	static std::string getFixtureRootPath()
	{
		return TestUtil::fixtureRootPath;
	}
};
