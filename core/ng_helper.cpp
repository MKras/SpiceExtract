#ifndef NG_HELPER_H
#define NG_HELPER_H


#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#ifndef _MSC_VER
#include <stdbool.h>
#include <pthread.h>
#else
#define bool int
#define true 1
#define false 0
#define strdup _strdup
#endif

#include <pthread.h>
#include <signal.h>

#include "core/ng_helper.h"
#include "core/sharedspice.h"
#include <iostream>

bool no_bg = true;
bool not_yet = true;
bool will_unload = false;

ControlledExit ng_exit;
SendData ng_data;
SendInitData ng_initdata;

int vecgetnumber = 0;
double v2dat;
static bool has_break = false;
int testnumber = 0;
void alterp(int sig);

/* functions exported by ngspice */
funptr_t ngSpice_Init_handle = NULL;
funptr_t ngSpice_Command_handle = NULL;
funptr_t ngSpice_Circ_handle = NULL;
funptr_t ngSpice_CurPlot_handle = NULL;
funptr_t ngSpice_AllVecs_handle = NULL;
funptr_t ngSpice_GVI_handle = NULL;

void * ngdllhandle = NULL;

#ifndef _MSC_VER
pthread_t mainthread;
#endif // _MSC_VER



/* Callback function called from bg thread in ngspice to transfer
   any string created by printf or puts. Output to stdout in ngspice is
   preceded by token stdout, same with stderr.*/
int
ng_getchar(char* outputreturn, void* userdata)
{
    printf("%s\n", outputreturn);
    return 0;
}

/* Callback function called from bg thread in ngspice to transfer
   simulation status (type and progress in percent. */
int
ng_getstat(char* outputreturn, void* userdata)
{
    printf("%s\n", outputreturn);
    return 0;
}

/* Callback function called from ngspice upon starting (returns true) or
  leaving (returns false) the bg thread. */
int
ng_thread_runs(bool noruns, void* userdata)
{
    no_bg = noruns;
    if (noruns)
        printf("bg not running\n");
    else
        printf("bg running\n");

    return 0;
}

/* Callback function called from bg thread in ngspice if fcn controlled_exit()
   is hit. Do not exit, but unload ngspice. */
int
ng_exit(int exitstatus, bool immediate, bool quitexit, void* userdata)
{

    if(quitexit) {
        printf("DNote: Returned form quit with exit status %d\n", exitstatus);
    }
    if(immediate) {
        printf("DNote: Unload ngspice\n");
        ((int * (*)(char*)) ngSpice_Command_handle)("bg_pstop");
        dlclose(ngdllhandle);
    }

    else {
        printf("DNote: Prepare unloading ngspice\n");
        will_unload = true;
    }

    return exitstatus;

}

/* Callback function called from bg thre        ad in ngspice once per accepted data point */
int
ng_data(pvecvaluesall vdata, int numvecs, void* userdata)
{
    int *ret;

    v2dat = vdata->vecsa[vecgetnumber]->creal;
    if (!has_break && (v2dat > 0.5)) {
    /* using signal SIGTERM by sending to main thread, alterp() then is run from the main thread,
      (not on Windows though!)  */
#ifndef _MSC_VER
        if (testnumber == 4)
            pthread_kill(mainthread, SIGTERM);
#endif
        has_break = true;
    /* leave bg thread for a while to allow halting it from main */
#if defined(__MINGW32__) || defined(_MSC_VER)
        Sleep (100);
#else
        usleep (100000);
#endif
//        ret = ((int * (*)(char*)) ngSpice_Command_handle)("bg_halt");
    }
    return 0;
}

/* Case insensitive str eq. */
/* Like strcasecmp( ) XXX */

int
cieq(register char *p, register char *s)
{
    while (*p) {
        if ((isupper(*p) ? tolower(*p) : *p) !=
            (isupper(*s) ? tolower(*s) : *s))
            return(false);
        p++;
        s++;
    }
    return (*s ? false : true);
}

/* Callback function called from bg thread in ngspice once upon intialization
   of the simulation vectors)*/
int
ng_initdata(pvecinfoall intdata, void* userdata)
{
    int i;
    int vn = intdata->veccount;
    for (i = 0; i < vn; i++) {
        printf("Vector: %s\n", intdata->vecs[i]->vecname);
        /* find the location of V(2) */
        if (cieq(intdata->vecs[i]->vecname, "V(2)"))
            vecgetnumber = i;
    }
    return 0;
}

/* Funcion called from main thread upon receiving signal SIGTERM */
void
alterp(int sig) {
    ((int * (*)(char*)) ngSpice_Command_handle)("bg_halt");
}

/* Unify LINUX and Windows dynamic library handling */
#if defined(__MINGW32__) ||  defined(_MSC_VER)

void *dlopen(const char *name,int type)
{
    return LoadLibrary((LPCSTR)name);
}

funptr_t dlsym(void *hDll, const char *funcname)
{
    return GetProcAddress(hDll, funcname);
}

char *dlerror(void)
{
    LPVOID lpMsgBuf;
    char * testerr;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0,
        NULL
    );
    testerr = (char*)lpMsgBuf;
    strcpy(errstr,lpMsgBuf);
    LocalFree(lpMsgBuf);
    return errstr;
}

int dlclose (void *lhandle)
{
    return (int)FreeLibrary(lhandle);
}
#endif


//////////////////////////////////

#include <exception>
#include <stdexcept>

//NGSpiceWrapper_Exception/////////////////////////////////
const char* NGSpiceWrapper_Exception::what() const throw() { return s.c_str(); }
NGSpiceWrapper_Exception::NGSpiceWrapper_Exception(std::string ss) : s(ss) {}
NGSpiceWrapper_Exception::~NGSpiceWrapper_Exception()  throw() {}
/////////////////////////////////NGSpiceWrapper_Exception


class NGSpiceWrapper_Impl
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
    void Init_dll_handler(const char * loadstring = "libngspice.so")
    {
#ifndef _MSC_VER
        mainthread = pthread_self();
#endif // _MSC_VER
        printf("Load ngspice.dll\n");
#ifdef __CYGWIN__
        loadstring = "/cygdrive/c/cygwin/usr/local/bin/cygngspice-0.dll";
#elif _MSC_VER
        loadstring = "ngspice.dll";
    //    loadstring = "d:/Spice_general/ngspice/visualc-shared/Debug/bin/ngspice.dll";
#elif __MINGW32__
        loadstring = "D:\\Spice_general\\ngspice\\visualc-shared\\Debug\\bin\\ngspice.dll";
#else
        loadstring = "libngspice.so";
#endif
        ngdllhandle = dlopen(loadstring, RTLD_NOW);
        errmsg = dlerror();
        if (errmsg)
            throw NGSpiceWrapper_Exception (std::string (errmsg));
        if (ngdllhandle)
        {
            printf("ngspice.dll loaded\n");
        }
        else
        {
           throw NGSpiceWrapper_Exception (std::string ("ngspice.dll not loaded !\n"));
           //exit(1);
        }
    }

    void Init_handlers()
    {
        ngSpice_Init_handle = dlsym(ngdllhandle, "ngSpice_Init");
        errmsg = dlerror();
        if (errmsg)
            throw NGSpiceWrapper_Exception (std::string (errmsg));
        ngSpice_Command_handle = dlsym(ngdllhandle, "ngSpice_Command");
        errmsg = dlerror();
        if (errmsg)
            throw NGSpiceWrapper_Exception (std::string (errmsg));
        ngSpice_CurPlot_handle = dlsym(ngdllhandle, "ngSpice_CurPlot");
        errmsg = dlerror();
        if (errmsg)
           throw NGSpiceWrapper_Exception (std::string (errmsg));;
        ngSpice_AllVecs_handle = dlsym(ngdllhandle, "ngSpice_AllVecs");
        errmsg = dlerror();
        if (errmsg)
            throw NGSpiceWrapper_Exception (std::string (errmsg));
        ngSpice_GVI_handle = dlsym(ngdllhandle, "ngGet_Vec_Info");
        errmsg = dlerror();
        if (errmsg)
            throw NGSpiceWrapper_Exception (std::string (errmsg));
        ngSpice_Circ_handle = dlsym(ngdllhandle, "ngSpice_Circ");
         errmsg = dlerror();
        if (errmsg)
            throw NGSpiceWrapper_Exception (std::string (errmsg));


    }

    int * NGngSpice_Init_handle()
    {
        ret = ((int * (*)(SendChar*, SendStat*, ControlledExit*, SendData*, SendInitData*,
                 BGThreadRunning*, void*)) ngSpice_Init_handle)(ng_getchar, ng_getstat,
                 ng_exit, NULL, ng_initdata, ng_thread_runs, NULL);

        return ret;
    }

    int * ngSpice_Command(const std::string & command)
    {
        ret = ((int * (*)(char*)) ngSpice_Command_handle)((char*)&command[0]);
        return  ret;
    }

    void sleep_sec (int seconds)
    {
#if defined(__MINGW32__) || defined(_MSC_VER)
        Sleep (seconds * 1000);
#else
        usleep (seconds * 1000000);
#endif
    }

    int * bg_resume()
    {
        ret = ((int * (*)(char*)) ngSpice_Command_handle)("bg_resume");
        return  ret;
    }

    int * bg_run()
    {
        ret = ((int * (*)(char*)) ngSpice_Command_handle)("bg_run");
        return  ret;
    }

    int * bg_halt()
    {
        ret = ((int * (*)(char*)) ngSpice_Command_handle)("bg_halt");
        return  ret;
    }

    void load_cir(std::string & cir_path)
    {
        std::string cir_path_t = std::string ("source "+cir_path);
#if defined(__CYGWIN__)
        ret = ((int * (*)(char*)) ngSpice_Command_handle)( (char*)&cir_path_t[0]);
#elif __MINGW32__
        ret = ((int * (*)(char*)) ngSpice_Command_handle)(std::string( (char*)&cir_path_t[0]);
#else
        ret = ((int * (*)(char*)) ngSpice_Command_handle)(  (char*)&cir_path[0]);
#endif
    }

    char ** load_cir(std::vector<std::string> cir)
    {
        char ** circarray;
        circarray = (char**)malloc(sizeof(char*) * (cir.size() + 1));
        std::vector<std::string>::iterator iter = cir.begin();
        //    circarray[0] = strdup("test array");
        for (int k=0; k<cir.size(); k++)
        {
            circarray[k] = strdup(&cir[k][0]);
            std::cout<<std::string(&cir[k][0])<<std::endl;
        }
        circarray[cir.size()] = NULL;
//            ret = ((int * (*)(char**)) ngSpice_Circ_handle)(circarray);
        return circarray;
    }

    int * simulate_cir (char ** circarray)
    {
        ret = ((int * (*)(char**)) ngSpice_Circ_handle)(circarray);
        return  ret;
    }

    /* read current plot while simulation continues */
    char * get_CurPlot()
    {
         curplot_ = ((char * (*)()) ngSpice_CurPlot_handle)();
         std::cout<<"CurPlot: "<<std::string(curplot_)<<std::endl;
         return curplot_;
    }

    simulation_result_T get_AllVecs (char * curplot)
    {

        plots_data_T plots_data;
        simulation_result_T result;

        vecarray_ = ((char ** (*)(char*)) ngSpice_AllVecs_handle)(curplot);
        /* get length of first vector */
        if (vecarray_) {
            char plotvec[256];
            pvector_info myvec;
            int veclength;

            int i=0;
            while(true)
            {
                if (vecarray_[i])
                {
                    vecname_ = vecarray_[i];
                    sprintf(plotvec, "%s.%s", curplot, vecname_);
                    myvec = ((pvector_info (*)(char*)) ngSpice_GVI_handle)(plotvec);
                    veclength = myvec->v_length;
                    printf("\nActual length of vector %s is %d\n", plotvec, veclength);
                    plot_data_T plot_data;
                    for (int k = 0; k<veclength; k++)
                    {
                        //printf ("vector %s data %f k=%i\n" ,plotvec, myvec->v_realdata[k], k);
                        plot_data.push_back(myvec->v_realdata[k]);
                    }
                    plots_data[std::string(vecname_)] = plot_data;
                    result.curplot = std::string(curplot);
                    result.vec_data =  plots_data;

                }else
                    break;

                i++;
            }
        }

        return result;
    }

    /* wait until simulation finishes */
    void wait_until_simulation_finishes()
    {
        for (;;) {
#if defined(__MINGW32__) || defined(_MSC_VER)
            Sleep (100);
#else
            usleep (100000);
#endif
            if (no_bg)
                break;
        }
    }

    void stop_waiting_until_simulation_finishes()
    {
        no_bg = true;
    }

public:
    NGSpiceWrapper_Impl():
        ngSpice_Init_handle (NULL)
      , ngSpice_Command_handle (NULL)
      , ngSpice_Circ_handle (NULL)
      , ngSpice_CurPlot_handle (NULL)
      , ngSpice_AllVecs_handle (NULL)
      , ngSpice_GVI_handle (NULL)
      , errmsg (NULL)
      , ngdllhandle(NULL)
    {

    }

    void run_simulation();

    bool NGSpiceOut(simulation_result_T sp_sim, std::string first_, std::string second_, xyData *res_xy){

        if( (sp_sim.vec_data.end() == sp_sim.vec_data.find(first_))
                && (sp_sim.vec_data.end() == sp_sim.vec_data.find(first_)) ){

            std::string error = "Can't find curves for compare. Only the next ";
            for (plots_data_T::iterator it = sp_sim.vec_data.begin(); it != sp_sim.vec_data.end(); it++){
                error = error + it->first+" ";
            }
            error = error + " avalible";
            throw NGSpiceWrapper_Exception("Not supported simulator");
        }
        return false;
    }
};

NGSpiceWrapper::NGSpiceWrapper():
    ngSpice_Init_handle (NULL)
  , ngSpice_Command_handle (NULL)
  , ngSpice_Circ_handle (NULL)
  , ngSpice_CurPlot_handle (NULL)
  , ngSpice_AllVecs_handle (NULL)
  , ngSpice_GVI_handle (NULL)
  , errmsg (NULL)
  , ngdllhandle(NULL)
{
   //NGSpiceWrapper_Impl_ ( new NGSpiceWrapper_Impl( ) );
   NGSpiceWrapper_Impl_ = new NGSpiceWrapper_Impl( );
}


void NGSpiceWrapper::Init_dll_handler(const char * loadstring ){
    return NGSpiceWrapper_Impl_->Init_dll_handler(loadstring);
}

void NGSpiceWrapper::Init_handlers(){
    return NGSpiceWrapper_Impl_->Init_handlers();
}

int * NGSpiceWrapper::NGngSpice_Init_handle(){
    return NGSpiceWrapper_Impl_->NGngSpice_Init_handle();
}

int * NGSpiceWrapper::ngSpice_Command(const std::string & command){
    return NGSpiceWrapper_Impl_->ngSpice_Command(command);
}

void NGSpiceWrapper::sleep_sec (int seconds){
    return NGSpiceWrapper_Impl_->sleep_sec(seconds);
}

int * NGSpiceWrapper::bg_resume(){
    return NGSpiceWrapper_Impl_->bg_resume();
}

int * NGSpiceWrapper::bg_run(){
    return NGSpiceWrapper_Impl_->bg_run();
}

int * NGSpiceWrapper::bg_halt(){
    return NGSpiceWrapper_Impl_->bg_halt();
}

void NGSpiceWrapper::load_cir(std::string & cir_path){
    return NGSpiceWrapper_Impl_->load_cir(cir_path);
}

char ** NGSpiceWrapper::load_cir(std::vector<std::string> cir){
    return NGSpiceWrapper_Impl_->load_cir(cir);
}

int * NGSpiceWrapper::simulate_cir (char ** circarray){
    return NGSpiceWrapper_Impl_->simulate_cir(circarray);
}

char * NGSpiceWrapper::get_CurPlot(){
    return NGSpiceWrapper_Impl_->get_CurPlot();
}

simulation_result_T NGSpiceWrapper::get_AllVecs (char * curplot){
    return NGSpiceWrapper_Impl_->get_AllVecs(curplot);
}

void NGSpiceWrapper::wait_until_simulation_finishes(){
    return NGSpiceWrapper_Impl_->wait_until_simulation_finishes();
}

void NGSpiceWrapper::stop_waiting_until_simulation_finishes(){
    return NGSpiceWrapper_Impl_->stop_waiting_until_simulation_finishes();
}

bool NGSpiceWrapper::NGSpiceOut(simulation_result_T sp_sim, std::string first, std::string second, xyData *res_xy){
    return NGSpiceWrapper_Impl_->NGSpiceOut(sp_sim, first,  second, res_xy);
}

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

#endif // NG_HELPER_H
