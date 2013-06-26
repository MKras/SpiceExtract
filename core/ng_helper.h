//#ifndef NG_HELPER_H
//#define NG_HELPER_H

#if defined(__MINGW32__) ||  defined(_MSC_VER)
#undef BOOLEAN
#include <windows.h>
typedef FARPROC funptr_t;
void *dlopen (const char *, int);
funptr_t dlsym (void *, const char *);
int dlclose (void *);
char *dlerror (void);
#define RTLD_LAZY	1	/* lazy function call binding */
#define RTLD_NOW	2	/* immediate function call binding */
#define RTLD_GLOBAL	4	/* symbols in this dlopen'ed obj are visible to other dlopen'ed objs */
static char errstr[128];
#else
#include <dlfcn.h> /* to load libraries*/
#include <unistd.h>
#include <ctype.h>
typedef void *  funptr_t;
#endif

//////////////////////////////////
#include <vector>
#include <map>
#include <exception>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

struct xyData{
        std::vector <double> x;
        std::vector <double> y;
};

typedef std::vector<float> plot_data_T;
typedef  std::map < std::string, plot_data_T > plots_data_T;

struct simulation_result_T
{
    std::string curplot;
    plots_data_T vec_data;
};

class NGSpiceWrapper_Exception : public std::exception
{
private:
   std::string s;
   bool no_bg = true;
public:
   const char* what() const throw();
   NGSpiceWrapper_Exception(std::string ss);
   ~NGSpiceWrapper_Exception()  throw();
};

class NGSpiceWrapper_Impl;
typedef boost::shared_ptr<NGSpiceWrapper_Impl> NGSpiceWrapper_ImplPtr;

class NGSpiceWrapper
{
private:
    /* functions exported by ngspice */
    funptr_t ngSpice_Init_handle;// = NULL;
    funptr_t ngSpice_Command_handle;// = NULL;
    funptr_t ngSpice_Circ_handle;// = NULL;
    funptr_t ngSpice_CurPlot_handle;// = NULL;
    funptr_t ngSpice_AllVecs_handle;// = NULL;
    funptr_t ngSpice_GVI_handle;// = NULL;

    char *errmsg; // = NULL,
    void * ngdllhandle;// = NULL;
    int *ret;

    char *loadstring, *curplot_, *vecname_;
    char **vecarray_;
    char ** circarray_;

#ifndef _MSC_VER
    pthread_t mainthread;
#endif // _MSC_VER

public:
    void Init_dll_handler(const char * loadstring = "libngspice.so");

    void Init_handlers();

    int * NGngSpice_Init_handle();
    int * ngSpice_Command(const std::string & command);

    void sleep_sec (int seconds);

    int * bg_resume();

    int * bg_run();

    int * bg_halt();

    void load_cir(std::string & cir_path);

    char ** load_cir(std::vector<std::string> cir);

    int * simulate_cir (char ** circarray);

    /* read current plot while simulation continues */
    char * get_CurPlot();

    simulation_result_T get_AllVecs (char * curplot);

    /* wait until simulation finishes */
    void wait_until_simulation_finishes();

    void stop_waiting_until_simulation_finishes();

    NGSpiceWrapper();

    void run_simulation();
    bool NGSpiceOut(simulation_result_T sp_sim, std::string first, std::string second, xyData *res_xy);

private:
    NGSpiceWrapper_Impl *NGSpiceWrapper_Impl_;
    //NGSpiceWrapper_ImplPtr NGSpiceWrapper_Impl_;

};

// example
//int main()
//{


//    NGSpiceWrapper ngw;
//    ngw.Init_dll_handler();
//    std::cout<<" ngw.Init_dll_handler();"<<std::endl;
//    ngw.Init_handlers();
//    std::cout<<" ngw.Init_handlers();"<<std::endl;
//    ngw.NGngSpice_Init_handle();


//    std::string cir_path= "adder_mos.cir";
//    std::cout<<" ngw.NGngSpice_Init_handle();"<<std::endl;
//    ngw.load_cir(cir_path);
//    std::cout<<" ngw.load_cir(adder_mos.cir);"<<std::endl;
//    ngw.bg_run();
//    std::cout<<"ngw.bg_run()"<<std::endl;
//    ngw.sleep_sec(5);
//    ngw.bg_halt();
//    ngw.sleep_sec(5);
//    ngw.bg_resume();
//    std::cout<<"ngw.wait_until_simulation_finishes()"<<std::endl;
//    ngw.wait_until_simulation_finishes();
//    std::cout<<"ngw.get_AllVecs()"<<std::endl;
//    simulation_result_T res = ngw.get_AllVecs(ngw.get_CurPlot());

//    std::cout<<res.curplot<<std::endl;

//    for ( plots_data_T::iterator iter = res.vec_data.begin(); iter != res.vec_data.end(); iter++)
//    {
//        std::cout<<iter->first<<std::endl;
//    }

//    std::vector <std::string> circarray_;
//    circarray_.resize(6);
//    circarray_[0] = strdup("test array");
//    circarray_[1] = strdup("V1 1 0 1");
//    circarray_[2] = strdup("R1 1 2 1");
//    circarray_[3] = strdup("C1 2 0 1 ic=0");
//    circarray_[4] = strdup(".tran 10u 3 uic");
//    circarray_[5] = strdup(".end");
//    //circarray_[6] = NULL;
//    ngw.simulate_cir(ngw.load_cir(circarray_));
//    std::cout<<"ngw.bg_run()"<<std::endl;
//    ngw.bg_run();
//    std::cout<<"ngw.wait_until_simulation_finishes()"<<std::endl;
//    ngw.wait_until_simulation_finishes();

//    std::cout<<"ngw.get_AllVecs()"<<std::endl;
//    //simulation_result_T res = ngw.get_AllVecs(ngw.get_CurPlot());
//    res = ngw.get_AllVecs(ngw.get_CurPlot());

//    for ( plots_data_T::iterator iter = res.vec_data.begin(); iter != res.vec_data.end(); iter++)
//    {
//        std::cout<<iter->first<<std::endl;
//    }

//    exit(0);
//}

//#endif // NG_HELPER_H
