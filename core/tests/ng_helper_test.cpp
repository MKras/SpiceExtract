#define BOOST_TEST_MODULE stringtest
#include <boost/test/included/unit_test.hpp>
#include <boost/filesystem.hpp>
#include "../ng_helper.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE (NGWrapper_tests) // name of the test suite is stringtest

BOOST_AUTO_TEST_CASE (NGSpiceWrapper_run_test)
{    
    NGSpiceWrapper ngw;
    BOOST_CHECK_NO_THROW (ngw.Init_dll_handler());
    std::cout<<" ngw.Init_dll_handler();"<<std::endl;
    BOOST_CHECK_NO_THROW (ngw.Init_handlers());
    std::cout<<" ngw.Init_handlers();"<<std::endl;
    BOOST_CHECK_NO_THROW (ngw.NGngSpice_Init_handle());

    BOOST_CHECK_EQUAL (boost::filesystem::exists(boost::filesystem::path(std::string(CMAKE_SOURCE_DIR)+std::string("/resourses/tests_data/ngspice_wrapper_data"))), true);
    boost::filesystem::path path (std::string(CMAKE_SOURCE_DIR)+std::string("/resourses/tests_data/ngspice_wrapper_data"));

    BOOST_CHECK_NO_THROW (copy_file(path/"idsvds1.1.cir",boost::filesystem::current_path()/"idsvds1.1.cir",boost::filesystem::copy_option::overwrite_if_exists));
    BOOST_CHECK_EQUAL(boost::filesystem::exists(path/"idsvds1.1.cir"), true);

    BOOST_CHECK_NO_THROW (copy_file(path/"idsvds1.1.cir",boost::filesystem::current_path()/"idsvds1.1.cir",boost::filesystem::copy_option::overwrite_if_exists));
    BOOST_CHECK_EQUAL(boost::filesystem::exists(path/"idsvds1.1.cir"), true);

    BOOST_CHECK_NO_THROW (copy_file(path/"idsvds1.1.exp",boost::filesystem::current_path()/"idsvds1.1.exp",boost::filesystem::copy_option::overwrite_if_exists));
    BOOST_CHECK_EQUAL(boost::filesystem::exists(path/"idsvds1.1.exp"), true);

    BOOST_CHECK_NO_THROW (copy_file(path/"spicelib.slb",boost::filesystem::current_path()/"spicelib.slb",boost::filesystem::copy_option::overwrite_if_exists));
    BOOST_CHECK_EQUAL(boost::filesystem::exists(path/"spicelib.slb"), true);

    std::string spice_cir = "idsvds1.1.cir";
    BOOST_CHECK_NO_THROW(ngw.load_cir(spice_cir));

    BOOST_CHECK_NO_THROW( ngw.bg_run());

    BOOST_CHECK_NO_THROW( ngw.wait_until_simulation_finishes());

    simulation_result_T res = ngw.get_AllVecs(ngw.get_CurPlot());

    xyData xy;
//    std::cout <<"Cur plot "<< res.curplot<<std::endl;
//    for (plots_data_T::iterator it = res.vec_data.begin(); it != res.vec_data.end(); it++){
//        std::cout<<it->first<<std::endl;
//    };
    BOOST_CHECK_EQUAL(res.vec_data.end() != res.vec_data.find("vd#branch"), true);
    BOOST_CHECK_EQUAL(res.vec_data.end() != res.vec_data.find("v-sweep"), true);
    plots_data_T::iterator y = res.vec_data.find("vd#branch");
    plots_data_T::iterator x = res.vec_data.find("v-sweep");

    //std::cout<< "X\tY"<<std::endl;
    for(int i = 0; i < x->second.size(); i++){
        //std::cout<<x->second[i]<<" "<<y->second[i]<<std::endl;
        BOOST_CHECK_NO_THROW( xy.x.push_back(x->second[i]) );
        BOOST_CHECK_NO_THROW( xy.y.push_back(y->second[i]) );
    }

    xyData xy_compare;
    ngw.NGSpiceOut(res, "v-sweep", "vd#branch", &xy_compare);

    for(int i = 0; i < xy_compare.x.size(); i++){
        //std::cout<<xy_compare.x[i]<<" "<<xy_compare.y[i]<<std::endl;
        BOOST_CHECK_EQUAL( xy_compare.x[i], xy.x[i] );
        BOOST_CHECK_EQUAL( xy_compare.y[i], xy.y[i] );
    }

    bool catched = false;

    try{
        ngw.NGSpiceOut(res, "v-sweep", "vd#branc", &xy_compare);
    }catch(NGSpiceWrapper_Exception &e){
        std::cout<<e.what()<<std::endl;
        catched = true;
    }

    BOOST_CHECK_EQUAL(catched, true);

}

BOOST_AUTO_TEST_SUITE_END( )
