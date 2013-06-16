#ifndef SPICEEXTR_H
#define SPICEEXTR_H

/*
 * options.h
 *
 *  Created on: 09.02.2009
 *      Author: spike
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_
#define use_namespace


#include <string>
//#include <boost/regex.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>// for MAX_PATH
//#include "mymath.h"
#include "../util/globals.h"
#include <QProcess>
#include <QTextStream>
#include <unistd.h>
#include <core/ng_helper.h>

#ifdef WITH_OPTPP
//////OPTPP/////////////
//
#include <OptNIPS.h>
#include <OptPDS.h>
#include <OptConstrFDNewton.h>
#include <OptConstrQNewton.h>
#include <OptBCQNewton.h>
#include <OptCG.h>
#include <NLF.h>
//#include <NLF1.h>
//constraints

#include <OptNIPSLike.h>
#include <OptNIPS.h>
#include <OptFDNIPS.h>
#include <OptBCEllipsoid.h>
#include <OptBCNewtonLike.h>
#include <OptBCFDNewton.h>
#include <OptBCNewton.h>
#include <OptConstrFDNewton.h>

//#include <lm.h>

#include <Constraint.h>
#include <BoundConstraint.h>
#include <CompoundConstraint.h>
//////////////////////////

using NEWMAT::ColumnVector;
using namespace OPTPP;
#endif



using namespace std;
//class SpiceExtr: public QProcess {
class SpiceExtr: public QObject {
     Q_OBJECT
public:
        void MultiRun(int dim, double* x, double& fx, int& result);
        void kill();
        SpiceExtr();        
        virtual ~SpiceExtr();
        void setopt_path(string s);
        string getopt_path();
        void setspice_path(string s);
        string getspice_path();
        void setSpiceParams(string s);
        string getSpiceParams();
        void addSpiceIn(string s);
        vector<string> getSpiceIn();
        void addSpiceOut(string s);
        vector<string> getSpiceOut();
        void addSpiceExp(string s);
        vector<string> getSpiceExp();
        void setSpiceLib(string s);
        string getSpiceLib();
        void getOptInput(string spice_param);
        //void Run();
        //void Init();
        void Init(curve *cur);
        double Simulate(vector<double>spiceinput);
        int getParametersNumber();
        void setAlgorythm(int alg);
        int getAlgorythm();
        int getXCount();
        void setErrorFunction(int e);
        int getErrorFunction();

        double getValue(int i);
        double getUpper(int i);
        double getLower(int i);
        string getName(int i);
        void setValue(size_t i, double v);
        void setUpper(size_t i, double u);
        void setLower(size_t i, double l);
        double GetGradStep(size_t i);
        void SetGradStep(size_t i, double step);

        //GA
        float getPCross();
        float getPMut();
        int getNGen();
        int getPopSize();
        int getGABin();

        //Opt++
        void setFTol (double ft);
        double getFTol ();
        void setAccrcy (double ft);
        void setMaxIter(int m);
        int getMaxIter();
        void setFEval(int me);
        int getFEval();
        void setBackTrackIter(int bti);
        int getBackTrackIter();
        void setGtols(double gt);
        double getGtols();
        double getdx();
        void setdx(double dx);
        double getLambda();
        void setLambda(double lamb);


        double getAccrcy ();
        void OptppInit(int n, double *x);
        void OptppFeval (int dim, double* x, double& fx, int& result);
        void OptppFeval_conv (int dim, double*  x, double& fx, int& result);
        //void OptppGradeval (int dim, const ColumnVector& x, ColumnVector& grad, int& result);
        int CountExperimentPoints();
        vector<double> GetAllExperimentResults();
        vector<double> GetAllSimulationResults();
        int getParamSize();
        void get_trans_val (double* x, double* res);
        void bounds_transform ();
        void get_real_val (double* x_conv, double* res);
        //LevMar
        void setLM_mu(double val);
        double getLM_mu();

        double getLM_epsilon1();
        void setLM_epsilon1(double  val);

        double getLM_epsilon2();
        void setLM_epsilon2(double val);

        double getLM_epsilon3();
        void setLM_epsilon3(double  val);

        double getLM_JacobianStep();
        void setLM_JacobianStep(double val);
        int getLM_iter();
        void setLM_iter(int val);



private:
        //stringstream d2str;
        bool fileWrites;
        //bool killOK;//=false;
        bool stop;
        //int simulator;// 1 - NGSpice, 2 - Spectre
        enum Simulator {NGSpice, Spectre, GNUCap};// 1 - NGSpice, 2 - Spectre
        Simulator simulator;

        string out_pars; //указывает какие колонки с результатами моделирования вырывать
        string opt_path; //path to opt.conf file
        string prefix; //prefix to opt.conf e.g. ./srcipt/
        string spice_path; //path to spice sinulator
        string spiceparams; //initial values of spice parameters
        vector <string> spicein; //path to cir file
        vector <string> spiceout; //path to output file
        vector <string> spiceexp; //path to experemet results to
        vector<double> gradstep;
        string spicelib; //path to slb file with model to fit
        vector <double> val;
        vector <double> val_up;
        vector <double> val_low;
        vector <string> val_name;
        int ErrorFunction; //1 - mean error; 2 - square error
        int ExperimentPoints;
        int algorithm;
        double LM_mu;
        double LM_epsilon1;
        double LM_epsilon2;
        double LM_epsilon3;
        double LM_JacobianStep;
        int LM_iter;
        int itercount;
        struct xyData{
                vector <double> x;
                vector <double> y;
        };

        //A and B parameters to convert parameters in range [-1; 1]
        double *a, *b;

        //Variables for optimozation routines
        double FTol; //OPT++ function tolerance =1.49012e-8
        double accrcy;//accuricy for optimization STOP criterie =1
        int maxiter;//maximum iteration =10000
        int Feval;//maximum function evaluation =100000
        int backtrackiter;//maximum backtracing iterations =1000000
        double Gtols;//tolerance for Gradient calculation =1e-8
        double dx; //Gradient evaluation step
        double lambda; //step size along antigradient direction
        //LevMar algorithm

        //GA
        int popsize; //population size   = 100
        int ngen; //namber of generatopns  = 250
        float pmut; //pmut = 0.1
        float pcross; //pcross  = 0.6
        int gaalg; //GA alg  = 1
                        //1 - GASteadyStateGA (overlapping populations)
                        //2 - GASimpleGA (non-overlapping populations)
                        //3 - GAIncrementalGA (overlapping with custom replacement)
                        //4 - GADemeGA (parallel populations with migration)
        int gascaling; //GA scaling  = 1
                        //1 - GANoScaling
                        //2 - GALinearScaling
                        //3 - GASigmaTruncationScaling
                        //4 - GAPowerLawScaling
                        //5 - GASharing
        int gaselection; //GA selection  = 1
                        //1 - GARankSelector
                        //2 - GARouletteWheelSelector
                        //3 - GATournamentSelector
                        //4 - GAUniformSelector
                        //5 - GASRSSelector
                        //6	- GADSSelector
        int gabin; //Binary with of genotype  = 24

        void addVal(double val);
        void addValName(string val_name);
        void abbValLow(double val_low);
        void addValUp(double val_up);
        double getVal_by_Name(string val_name);

        void setSpiceInputValues(string spicelib);
        double RunSimulation();
        xyData runNGSpice(string spice_path, string tmpspicein, string tmpspiceout);
        //vector<double> GetSimulationResults(string spiceout);
        //vector<double> GetSimulationResults(string spiceout);
        //vector<double> GetExperimentResults(string spiceout);

        xyData GetSimulationResults_xy(string sp_sim);
        xyData GetExperimentResults_xy(string sp_exp);
        bool NGSpiceOut(QTextStream *stream, QTextStream *tmpstream, QString first, QString second, xyData *res_xy);
        bool GNUcapOut(QTextStream *stream, QTextStream *tmpstream, QString first, QString second, xyData *res_xy);
        bool Spectre_psfascii_Out(QTextStream *stream, QTextStream *tmpstream, QString first, QString second, xyData *res_xy);


        //double CompareCurves(vector <double> sim, vector <double> exp);
        double CompareCurves( xyData sim, xyData exp, string Fout, string Fexp);
        double CompareCurves_Err1( xyData sim, xyData exp);
        double CompareCurves_Err2( xyData sim, xyData exp);
        double CompareCurves_Err3( xyData sim, xyData exp);
        double CompareCurves_Err4_x( xyData sim, xyData exp);
        void getParams();
        void getParams(curve *cur);
        xyData GSL_approximate(xyData *sim, xyData* exp);
        double interpolate(int init, std::vector<double> x, std::vector<double> y, double x_val);
        SpiceExtr::xyData interpolation(xyData *sim, xyData *exp);
        //GA
        void setGAalg(int ga);
        int getGAalg();
        void setPCross(float ga);
        void setPMut(float pm);
        void setNGen(int ng);
        void setPopSize(int ps);
        void setGAScaling(int gs);
        int getGAScaling();
        void setGASelection(int gs);
        int getGASelection();
        void setGABin(int gb);
        void CentralDeriv(int dim, double* x, int& res,
                        int i /*X number for derivative*/, double h, double *result,
                        double *abserr_round, double *abserr_trunc);
        void CentralDeriv_adapt_h (int dim, double* x, double& fx, int& res,
                        int i/*X number for derivative*/, double h,double *result, double *abserr);
        void OptppGradCalibre (int dim, double* x, double* grad, int& result);
        double XviaY(xyData data, double f, int i);


signals:
    void startModelChanging();
    void stopModelChanging();
    void resF(QString);

private slots:
    //void toBeKilled();

private:
    NGSpiceWrapper *NGWrapper_;

public:






};



#endif /* OPTIONS_H_ */



#endif // SPICEEXTR_H
