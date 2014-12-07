#pragma once

#if 0
#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#endif
#include <gtest/gtest.h>
#include "../include/libvsq/DynamicsMode.hpp"
#include "../include/libvsq/PlayMode.hpp"
#include "../include/libvsq/HandleType.hpp"
#include "../include/libvsq/ArticulationType.hpp"
#include "../include/libvsq/EventListIndexIteratorKind.hpp"
#include "../include/libvsq/MidiParameterType.hpp"
#include "../include/libvsq/VoiceLanguage.hpp"
#include "../include/libvsq/EventType.hpp"

#ifdef _MSC_VER
#    pragma execution_character_set("utf-8")
#endif

#define REGISTER_TEST_SUITE( ATestFixtureType )      \
	static CppUnit::AutoRegisterSuite< ATestFixtureType > CPPUNIT_JOIN( testSuite, ATestFixtureType )

/**
 * @def LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR
 * @brief enum class の値を std::basic_ostream へ書き込む << 演算子を定義する.
 * @param ENUM_NAME 対象の enum class の名前.
 */
#define LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(ENUM_NAME) \
	/**																											\
	 * @fn std::basic_ostream<char, Traits>& operator << (std::basic_ostream<char, Traits>& os, ENUM_NAME v)	\
	 * @brief ENUM_NAME をストリームに書き込む.																	\
	 * @param os 書き込み先のストリーム.																			\
	 * @param v 書き込む ENUM_NAME の値.																			\
	 * @return 書き込んだ後のストリーム @a os.																		\
	 */																											\
	template<class Traits>																						\
	inline std::basic_ostream<char, Traits>& operator << (std::basic_ostream<char, Traits>& os, ENUM_NAME v)	\
	{																											\
		os << static_cast<std::underlying_type<ENUM_NAME>::type>(v);											\
		return os;																								\
	}

LIBVSQ_BEGIN_NAMESPACE

LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(DynamicsMode);
LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(PlayMode);
LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(HandleType);
LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(ArticulationType);
LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(EventListIndexIteratorKind);
LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(MidiParameterType);
LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(VoiceLanguage);
LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(EventType);

LIBVSQ_END_NAMESPACE

#undef LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR
