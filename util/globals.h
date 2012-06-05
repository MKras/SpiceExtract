#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <vector>
#include <QString>
#include <QModelIndex>
//#include "globals.h"

using namespace std;



struct curve{
        vector<string>  inFile;
        vector<string>  outFile;
        vector<string>  expFile;
        vector<string>  spiceParam;
        vector<string>  spiceMin;
        vector<string>  spiceInit;
        vector<string>  spiceMax;
        //string outdata;
        string AlgOpt;
        string model_path;
        string prefix;
        string path;
        double result;
        ///////////////////
        vector<string>  spiceOldVal;
        QModelIndex idx;
        //curve
        int ErrorFunction;//=2;
        string OutParse;//="{1};{2}";

        //global
        string  spice;//="spice -b";
        //SET TOLERANCE
        //#define number of intervals.  Accurancy = Upper - Lower * accrcy Default: 1e-6
        double accrcy;//=1e-6;
        //#Function stopping tolerance Default value: 1.49012e-8
        double FTol;//=1e-6;
        //#Gradient toegance Default value: 1e-8
        double Gtols;//=1e-8;
        //#Limit on the number of iterations. Default value:1000000, max val 1000000000
        int maxiter;//=600;
        //#Limit on the number of BacktrackIter. Default value:1000000, max val 1410065408
        int backtrackiter;//=10000;
        //#An upper bound on the number of function evaluations. Default value: 1000
        int Feval;//=20000;

        //#LevMar routine
        //#scale factor for initial \mu
        double LM_mu;//=1e-5;
        //#stopping threshold for ||J^T e||_inf
        double LM_epsilon1;//=1.0e-10;
        //#stopping threshold for ||Dp||_2
        double LM_epsilon2;//=1.0e-10;
        //#stopping threshold for ||e||_2
        double LM_epsilon3;//=1.0e-10;
        //#step used in difference approximation to the Jacobian
        double LM_JacobianStep;//=1e-8;
        //#LevMar Max Iteration Coutn
        double LM_iter;//=50;

        //My Algs
        //#Gradient evaluation step
        double dx;//=1e-6;
        //#step size along antigradient direction
        double lambda;//=1e-8;

        //#GA Parameters
        //#Population size (recomended 2xDimension). Defautl: 100
        int popsize;//=20;
        //#Namber of generatopns. Default: 250
        int ngen;//=50;
        //#pmut 0.02 (to avoid Local Minimum)
        double pmut;//=0.02;
        //#pcross 0.8 (Crossover)
        double pcross;//=0.9;
        //#Binary Width of Genotype: 24
        int bin;//=32;
        //#GA alg:
        //#1 - GASteadyStateGA (overlapping populations)
        //#2 - GASimpleGA (non-overlapping populations)
        //#3 - GAIncrementalGA (overlapping with custom replacement)
        //#4 - GADemeGA (parallel populations with migration)
        int gaalg;//=2;
        //#GA scaling
        //#1 - GANoScaling
        //#2 - GALinearScaling
        //#3 - GASigmaTruncationScaling
        //#4 - GAPowerLawScaling
        //#5 - GASharing
        int gascaling;//=3;
        string get_prefix()
        {
            prefix = path.substr(0, path.find(model_path.substr(0, model_path.find_last_of("/"))));
            return prefix;
        }


    };

/*struct curve{
        QVector<QString>  inFile;
        QVector<QString>  outFile;
        QVector<QString>  expFile;
        QVector<string>  spiceParam;
        QVector<string>  spiceMin;
        QVector<string>  spiceInit;
        QVector<string>  spiceMax;
        QString AlgOpt;
    };*/


#endif // GLOBALS_H
