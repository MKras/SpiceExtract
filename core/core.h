#ifndef CORE_H
#define CORE_H

/*
 * Main.cpp
 *
 *  Created on: 27.02.2009
 *      Author: spike
 */
//#define HAVE_LAPACK

//#define ROS

#define use_namespace
//#define WITH_GALIB
//#define WITH_OPTPP
//#define WITH_LEVMAR

#include <string>
#include <iostream>
#include <fstream>
//#include <boost/regex.hpp>
#include <vector>
#include <stdio.h>

#ifdef WITH_LEVMAR
#include <lm.h>
#endif
#ifdef WITH_GALIB
#include <ga/ga.h>
#include <ga/std_stream.h>
#endif
#ifdef WITH_OPTPP
#include <OptNIPS.h>
#include <OptPDS.h>
#endif
#include "../util/globals.h"
#include "spiceextr.h"


using namespace std;

#ifdef WITH_OPTPP
using NEWMAT::ColumnVector;
using namespace OPTPP;
#endif

//class Core: public QObject{
//    Q_OBJECT
//public:
    //Core();
//int core (curve *cur, string path);
int core (curve *cur, SpiceExtr *sp);
void del();


/////////////////////////
void DirectSearch(void(*func)(int n, double *a, double &fx), int dim, double *x, double *lb, double *ub, double tol );
void PG_gold (void(*func)(int d, double *a, double &fx), int n, double &step, double l,
                double u, double *grad, double *x, double *lb, double *ub, double *a, double *b);
void PG(void(*func)(int n, double *a, double &fx), int dim, double *x, double *lb, double *ub,
              double lambda, double scale, double tol, double *dx);


void OptppInit(int n, double* x);

void OptppFeval (int dim, double* x, double& fx, int& result);

#ifdef WITH_OPTPP
void OptppFeval_conv (int dim, double* x, double& fx, int& result);
void OptppFeval_conv_cv (int dim, const ColumnVector& tx, double& fx, int& result);
void OptppInit_conv(int n, double* x);
void OptppInit_conv_cv(int n, ColumnVector& x);
void OptppFeval_conv_shtraf(int dim, double* x, double& fx, int& result);
void OptppFeval_conv_shtraf_cv(int dim, const ColumnVector& x, double& fx, int& result);

void OptppInit_cv(int n, ColumnVector& tx);
#endif

void Myfunc (int n, double *a, double &fx);

#ifdef WITH_LEVMAR
void LevMarEvalF(double *p, double *x, int m, int n, void *data);
#endif




//private:
///////////Private
void Grad(void(*func)(int n, double *a, double &fx), int dim, double *x, double *lb, double *ub, double *dx, double *grad,
                double *a, double *b);
void get_real_val (int n, double *a, double *b, double *x, double *res);
void get_trans_val (int n, double *a, double *b, double *x, double *res);
void bounds_transform (int n, double *min, double *max, double *a, double *b);
void trans_func(void(*func)(int d, double *x, double &f), int dim, double *a, double *b, double *val, double &fx);
void PG_step(void(*func)(int n, double *a, double &fx), int n, double *grad, double *x,// double *xarr,
                                                        double &lambda, double scale, double tol, double *lb, double *ub,
                                                        double *a, double *b);




#ifdef WITH_GALIB
float gaBin2Dec_opt_f(GAGenome & c);

//void gaBin2DecF (int dim, const ColumnVector& lower, const ColumnVector& upper,
//                int popsize,int ngen,float pmut , float pcross, int bin, ColumnVector& res);
void gaBin2DecF(int dim, double* lower, double* upper,
                int popsize,int ngen,float pmut , float pcross, int bin, double* res);


int	old_main (int argc, char *argv[]);

//};
#endif
#endif // CORE_H
