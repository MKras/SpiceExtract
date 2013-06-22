#define BOOST_TEST_MODULE stringtest
#include <boost/test/included/unit_test.hpp>
#include "../ng_helper.h"


BOOST_AUTO_TEST_SUITE (NGWrapper_tests) // name of the test suite is stringtest

BOOST_AUTO_TEST_CASE (test1)
{
  std::string s;
  BOOST_CHECK(s.size() == 0);

  NGSpiceWrapper ngw;
  BOOST_CHECK_NO_THROW (ngw.Init_dll_handler());
  std::cout<<" ngw.Init_dll_handler();"<<std::endl;
  BOOST_CHECK_NO_THROW (ngw.Init_handlers());
  std::cout<<" ngw.Init_handlers();"<<std::endl;
  BOOST_CHECK_NO_THROW (ngw.NGngSpice_Init_handle());
}

BOOST_AUTO_TEST_CASE (test2)
{
  BOOST_REQUIRE_EQUAL (true, true); // basic test
}

BOOST_AUTO_TEST_SUITE_END( )
