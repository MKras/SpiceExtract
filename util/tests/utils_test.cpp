#define BOOST_TEST_MODULE stringtest
#include <boost/test/included/unit_test.hpp>
#include <boost/filesystem.hpp>


#include <iostream>

BOOST_AUTO_TEST_SUITE (NGWrapper_tests) // name of the test suite is stringtest

BOOST_AUTO_TEST_CASE (test1)
{
  std::string s;
  BOOST_CHECK(s.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END( )
