#define BOOST_TEST_MODULE stringtest
#include <boost/test/included/unit_test.hpp>
#include <boost/filesystem.hpp>
#include "../ng_helper.h"

#include <iostream>

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


    BOOST_CHECK_EQUAL (boost::filesystem3::exists(boost::filesystem::path(std::string(CMAKE_SOURCE_DIR)+std::string("/resourses/tests_data/ngspice_wrapper_data"))), true);
    boost::filesystem::path path (std::string(CMAKE_SOURCE_DIR)+std::string("/resourses/tests_data/ngspice_wrapper_data"));

    BOOST_CHECK_NO_THROW (copy_file(path/"idsvds1.1.cir",boost::filesystem::current_path()/"idsvds1.1.cir",boost::filesystem3::copy_option::overwrite_if_exists));
    BOOST_CHECK_EQUAL(boost::filesystem3::exists(path/"idsvds1.1.cir"), true);

    BOOST_CHECK_NO_THROW (copy_file(path/"idsvds1.1.cir",boost::filesystem::current_path()/"idsvds1.1.cir",boost::filesystem3::copy_option::overwrite_if_exists));
    BOOST_CHECK_EQUAL(boost::filesystem3::exists(path/"idsvds1.1.cir"), true);

    BOOST_CHECK_NO_THROW (copy_file(path/"idsvds1.1.exp",boost::filesystem::current_path()/"idsvds1.1.exp",boost::filesystem3::copy_option::overwrite_if_exists));
    BOOST_CHECK_EQUAL(boost::filesystem3::exists(path/"idsvds1.1.exp"), true);

    BOOST_CHECK_NO_THROW (copy_file(path/"spicelib.slb",boost::filesystem::current_path()/"spicelib.slb",boost::filesystem3::copy_option::overwrite_if_exists));
    BOOST_CHECK_EQUAL(boost::filesystem3::exists(path/"spicelib.slb"), true);

    std::string spice_lib = "spicelib.slb";
    BOOST_CHECK_NO_THROW(ngw.load_cir(spice_lib));

    BOOST_CHECK_NO_THROW( ngw.bg_run());

    ngw.wait_until_simulation_finishes();

    simulation_result_T res = ngw.get_AllVecs(ngw.get_CurPlot());

}

BOOST_AUTO_TEST_SUITE_END( )
