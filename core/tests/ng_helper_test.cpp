#define BOOST_TEST_MODULE stringtest
#include <boost/test/included/unit_test.hpp>
//#include "./str.h"

typedef std::string mystring;

BOOST_AUTO_TEST_SUITE (stringtest) // name of the test suite is stringtest

BOOST_AUTO_TEST_CASE (test1)
{
  mystring s;
  BOOST_CHECK(s.size() == 0);
}

BOOST_AUTO_TEST_CASE (test2)
{
  mystring s;
  s.setbuffer("hello world");
  BOOST_REQUIRE_EQUAL ('h', s[0]); // basic test
}

BOOST_AUTO_TEST_SUITE_END( )

#define BOOST_TEST_MODULE "Unit test example 05"
#include <boost/test/unit_test.hpp>


//#include "core/ng_helper.h"
//#include "core/sharedspice.h"
//#include <iostream>

//BOOST_AUTO_TEST_CASE(TestThatSucceeds)
//{
//    BOOST_CHECK(true);
//};
