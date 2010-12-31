/*
 * Main.cpp
 *
 *  Created on: 27.02.2009
 *      Author: spike
 */
//#define HAVE_LAPACK

//#define ROS

#include <string>
#include <iostream>
#include <fstream>
//#include <boost/regex.hpp>
#include <vector>

//#include "mymath.h"
#include <stdio.h>


#include <lm.h>
#include "core.h"
// для отладки
#include <QtGui>
#define use_namespace



SpiceExtr *sp;
int b;





/*
  // Powell's function, minimum at (0, 0)
void powell(double *p, double *x, int m, int n, void *data)
{
register int i;

  for(i=0; i<n; i+=2){
    x[i]=p[0];
    x[i+1]=10.0*p[0]/(p[0]+0.1) + 2*p[1]*p[1];
//    cout<<"x["<<i<<"] = "<<x[i]<<" "<<"x["<<i+1<<"] = "<<x[i+1]<<endl;
  }
  cout<<"p["<<0<<"] = "<<p[0]<<" "<<"p["<<1<<"] = "<<p[1]<<" x["<<0<<"] = "<<x[0]<<" "<<"x["<<1<<"] = "<<x[1]<<endl;
}

void jacpowell(double *p, double *jac, int m, int n, void *data)
{
register int i, j;

  for(i=j=0; i<n; i+=2){
    jac[j++]=1.0;
    jac[j++]=0.0;

    jac[j++]=1.0/((p[0]+0.1)*(p[0]+0.1));
    jac[j++]=4.0*p[1];
  }
}
*/

float gaBin2Dec_opt_f(GAGenome & c){
        GABin2DecGenome & genome = (GABin2DecGenome &)c;
          double fx;
          int dim,res, i;
          cout<<"Fitness = "<<genome.fitness()<<endl;
          dim=genome.length()/b;
          double x[dim];

          //for(i=0;i<=dim-1;i++){
          for(i=0;i<dim;i++){
                  x[i]=genome.phenotype(i);
                //  mx[i]=genome.phenotype(i);

          };

          OptppFeval (dim, x, fx, res);

  return fx;
};


//void gaBin2DecF(int dim, const ColumnVector& lower, const ColumnVector& upper,
//                int popsize,int ngen,float pmut , float pcross, int bin, ColumnVector& res){
void gaBin2DecF(int dim, double* lower, double* upper,
                int popsize,int ngen,float pmut , float pcross, int bin, double* res){

        cout<<"gaBin2DecF\n";
        cout<<"popsize = "<<popsize<<" ngen = "<<ngen<<" pmut = "<<pmut<<" pcross = "<<pcross<<endl;

  unsigned int seed = 0;
  b=bin;

  GABin2DecPhenotype map;
  for(int i=0;i<dim;i++){
          map.add(bin, lower[i],upper[i]);
  };

  GABin2DecGenome genome(map, gaBin2Dec_opt_f);
  GABoolean elit;
  elit=gaTrue;

  /*GADemeGA
   * GAIncrementalGA
   * GASimpleGA
   * GASteadyStateGA*/

  GASimpleGA ga(genome);
  GASigmaTruncationScaling scaling;
  //GASharing scaling;

  ga.populationSize(popsize);
  ga.nGenerations(ngen);
  ga.pMutation(pmut);
  ga.pCrossover(pcross);
  ga.scaling(scaling);
  ga.scoreFilename("bog.dat");
  ga.scoreFrequency(10);
  ga.flushFrequency(50);

  //ga.scaling(scaling);
  ga.elitist(elit);
  //ga.statistics().convergence();
  ga.minimize();
  ga.evolve(seed);

  //cout<< "\nconvergence: " << ga.statistics().convergence()<<endl;
  //cout << "\nThe GA bestPopulation:\n" << ga.statistics().bestPopulation() <<endl;//<< ", F = "<<gaReal_opt_f(genome)<<")."<<endl;

    //genome = ga.statistics().bestIndividual();

  //cout << "The GA bestIndividual: "<< ga.statistics().bestIndividual()<<endl; //found an optimum at the point (";
  //cout<< "\nF = "<<gaBin2Dec_opt_f(genome)<<endl;
  genome=ga.statistics().bestIndividual();
  for(int i=0; i<dim;i++){
          cout<<"res("<<i+1<<") = "<<genome.phenotype(i)<<endl;
          //cout<<"res("<<i+1<<") = "<<ga.statistics().bestIndividual(i)<<endl;
          res[i]=genome.phenotype(i);
          //res(i+1)=ga.statistics().bestIndividual(i);

    }
}

void del(){

    sp->deleteLater();
    //delete sp;
}

/////////// CORE ////////////////////
//int core (curve *cur, string path){
int core (curve *cur, SpiceExtr *spe){
    int dim; //к-во параметров
    int i=0; //итератор
    int result; //
    double fx; //результат расчета

    //sp = new SpiceExtr();
    //sp = new SpiceExtr(path);
    sp=spe;


    sp->Init(cur);
    qDebug()<<"dim=sp->getParametersNumber()\n";

    dim=sp->getParametersNumber();

    qDebug()<<"dim = "<<dim<<"\n";
    //qDebug()<<"cur->spiceParam.size() = "<<QString(cur->spiceParam.size())<<"\n";
    if(dim==cur->spiceParam.size()){
        qDebug()<<"core DIM Equal\n";
        qDebug()<<"ALG = "<<QString::fromStdString(cur->AlgOpt)<<"\n";
    }

    ColumnVector lower_cv(dim),upper_cv(dim), val_cv(dim), resultvector_cv(dim);// resultvector - вектор в котором будем хранить результаты оптимизации (иксы)
    double lower[dim],upper[dim], val[dim], resultvector[dim];// resultvector - вектор в котором будем хранить результаты оптимизации (иксы)

    /*
    OptBCQNewton +
    LevMar
    GA +
    Evaluate +
    Sence analyz+
    OptCG +
    OptFDNIPS +
    My DirectSearch +
    My PG +
*/

    if(cur->AlgOpt=="Evaluate"){
        qDebug() << "EVALUATING!!!\n";
        //double fx;
        int result;

        double x[dim];
        OptppInit(dim, x);
        OptppFeval( dim, x, fx, result);

        for(i=0;i<dim;i++){
            resultvector[i] = x[i];
        }

        cout<<"Result = "<<result<<"\n";

        //return 0;
    }    

    if((cur->AlgOpt=="OptBCQNewton")|(cur->AlgOpt=="OptCG")|(cur->AlgOpt=="OptFDNIPS")|(cur->AlgOpt=="OptPDS")){
        //трансформируем данные
        for(i=0;i<dim; i++){
            lower[i]=-1;
            lower_cv(i+1)=-1;
            val[i]=sp->getValue(i);
            resultvector[i]=sp->getValue(i); //result values vector from any optimization routines
            upper[i]=1;
            upper_cv(i+1)=1;
            cout<<"lower = "<<lower[i]<<" x = "<<val[i]<<" upper = "<<upper[i]<<endl;
        }

        // Начинается настройка методов оптимизации из библиотеки OPTPP

        Constraint boundc = new BoundConstraint(dim,lower_cv,upper_cv);
        CompoundConstraint* constraints = new CompoundConstraint(boundc);
        static char *status_file = {"work.out"};
        //FDNLF1 nlp(dim, OptppFeval,OptppInit, constraints);

        cout<<"!!!!!!Transform parameters value change to OptppInit_conf!!!!\n";
        FDNLF1 nlp(dim, OptppFeval_conv_cv,OptppInit_conv_cv, constraints);
        //NLF1 gragnpl(dim, OptppGradeval,OptppInit, constraints);
        nlp.setIsExpensive(true); //////////////////////////////////
        nlp.setConstraints(constraints);
        ColumnVector tx(dim);
        OptppInit_cv(dim, tx);

        ColumnVector FAc(dim), FcnAccrcy(dim);;
        FAc=nlp.getFcnAccrcy();
        for(i=1;i<=dim;i++){
                //nlp.setFcnAccrcy(i,(upper(i)-lower(i))*sp->getAccrcy());
                //nlp.setFcnAccrcy(i,tx(i)*sp->getAccrcy());
                nlp.setFcnAccrcy(i,sp->getAccrcy());
        };
        if(cur->AlgOpt=="OptPDS"){
            qDebug()<<"Core OptPDS\n";
            OptPDS objfunc(&nlp);
            objfunc.setFcnTol(sp->getFTol());
            objfunc.setMaxIter(sp->getMaxIter());
            objfunc.setMaxFeval(sp->getFEval());

            objfunc.setDebug();
            objfunc.checkConvg();

            objfunc.setMaxBacktrackIter(sp->getBackTrackIter());
            objfunc.setOutputFile(status_file, 0);
            objfunc.setLineSearchTol(sp->getFTol());
            objfunc.optimize();
            objfunc.printStatus("PARAM");
            //cout<<"LastEval"<<endl;
        };
        if(cur->AlgOpt=="OptBCQNewton"){
            qDebug()<<"Core OptBCQNewton\n";
            ColumnVector x(dim);
            OptppInit_cv(dim, x);

            ColumnVector acc(dim);
            acc=nlp.getFcnAccrcy();

            for(i=1;i<=dim;i++){
                            cout<<"ACC = "<<acc(i)<<endl;
            };
            OptBCQNewton objfunc(&nlp);
            objfunc.setMaxIter(sp->getMaxIter());
            objfunc.setMaxFeval(sp->getFEval());
            objfunc.setSearchStrategy(LineSearch);//LineSearch, TrustRegion, TrustPDS
            objfunc.setDebug();
            objfunc.setOutputFile(status_file, 0);
            objfunc.checkConvg();
            objfunc.optimize();
            objfunc.printStatus("PARAM");
        }
        if(cur->AlgOpt=="OptCG"){//используется штрафная функция
            qDebug()<<"Core OptCG\n";
            FDNLF1 nlpb(dim,OptppFeval_conv_shtraf_cv, OptppInit_conv_cv);
            //ColumnVector FAc(dim), FcnAccrcy(dim);;
            //FAc=nlpb.getFcnAccrcy();
            //задаем шаг для каждой из переменных accrcy !!!!!!
            //printf("\n");
            for(i=1;i<=dim;i++){
                    //nlpb.setFcnAccrcy(i,(upper(i)-lower(i))*sp->getAccrcy());
                    nlpb.setFcnAccrcy(i,sp->getAccrcy());
            };
            //printf("\n");
            OptCG objfunc(&nlpb);
            //LineSearch, TrustRegion, TrustPDS
            objfunc.setSearchStrategy(TrustPDS);//TrustPDS, TrustRegion
            objfunc.setFcnTol(sp->getFTol());
            objfunc.setLineSearchTol(sp->getFTol());
            objfunc.setMaxIter(sp->getMaxIter());
            objfunc.setMaxFeval(sp->getFEval());
            objfunc.setOutputFile(status_file, 0);
            objfunc.setDebug();
            objfunc.checkConvg();
            objfunc.optimize();
            objfunc.printStatus("PARAM");
            nlp=nlpb;
        }
        if(cur->AlgOpt=="OptFDNIPS"){//не очень хорошо считает
            qDebug()<<"Core OptFDNIPS\n";
            //cout<<"!!! ALG = 4!!!!!!! OptFDNIPS\n";
            OptFDNIPS objfunc(&nlp);
            objfunc.setOutputFile(status_file, 0);
            objfunc.setMaxFeval(sp->getFEval());
            objfunc.setFcnTol(sp->getFTol());
            cout<<"sp->getFTol() = "<<sp->getFTol()<<endl;
            objfunc.setMaxIter(sp->getMaxIter());
            objfunc.setDebug();
            objfunc.setStepLengthToBdry(10);
            objfunc.setMaxBacktrackIter(sp->getBackTrackIter());//backtrackiter
            objfunc.checkConvg();
            objfunc.setSearchStrategy(TrustPDS);
            objfunc.optimize();
            objfunc.printStatus("PARAM");
        }
        //последний расчет чтобы получить точное значение функции
        //после оптимизации
        //ColumnVector res(dim);
        OptppFeval_conv_cv( dim, nlp.getXc(), fx, result);
        //sp->get_real_val(nlp.getXc(),res);
        //resultvector=res;

        double getXc[dim];
        ColumnVector getXc_t(dim);
        getXc_t = nlp.getXc();
        for(i=0;i<dim;i++){
            getXc[i]=getXc_t(i+1);
        };
        sp->get_real_val(getXc, resultvector);
    }
    if((cur->AlgOpt=="My DirectSearch")|(cur->AlgOpt=="My PG")){

        double *mx;
        double *ml;
        double *mu;
        double *grad, *dx;

        mx = new double[dim];
        ml = new double[dim];
        mu = new double[dim];
        grad = new double[dim];
        dx = new double[dim];

        double alpha=1e-6;
        double scale=1.21;
        double gradstep;
        double tol=sp->getFTol();


        //ColumnVector x(dim);
        double x[dim];
        OptppInit(dim, x);

        for(i=0;i<dim;i++){
            mx[i]=sp->getValue(i);
            ml[i]=sp->getLower(i);
            mu[i]=sp->getUpper(i);
            //dx[i]=mx[i]*1e-1;
            dx[i]=sp->getdx();
        }
        cout<<"dx = "<<dx[0]<<"\n";

        if(cur->AlgOpt=="My DirectSearch"){
            qDebug()<<"Core My DirectSearch\n";
            DirectSearch(Myfunc, dim, mx, ml, mu, tol);

        }
        if(cur->AlgOpt=="My PG"){
            qDebug()<<"Core My PG\n";

            double lambda=sp->getLambda();
            //double lambda_l=lambda*1e-2;
            //double lambda_u=lambda*1e+2;

            PG(Myfunc, dim, mx, ml, mu, lambda, scale, tol, dx);

        }

        for(i=0;i<dim; i++){
            //resultvector(i+1)=mx[i];
            resultvector[i]=mx[i];
        }
    }
    if(cur->AlgOpt=="Sence analyz"){
        qDebug()<<"Sence analyz\n";
        double init=0;
        double yinit=0;
        //double ilow, iup;
        cout<<"SENCE"<<endl;
        //ColumnVector y(dim), x(dim), tl(dim), tu(dim), ylower(dim), yupper(dim);
        double  y[dim], x[dim];//, tl(dim), tu(dim), ylower(dim), yupper(dim);
        OptppInit(dim, x);
        OptppFeval( dim, x, fx, result);

        yinit=fx;
        cout<<"Fxinit = "<<fx<<endl;
        for(int i=0;i<dim;i++){
            init=x[i];
            x[i]=x[i]+((abs(sp->getUpper(i)-sp->getLower(i)))*0.01);
            OptppFeval( dim, x, fx, result);
            y[i]=fx;
            x[i]=init;
        };

        OptppFeval( dim, x, fx, result);
        cout<<"1\% deviation test: \n"<<endl;

        cout<<"Fxinit = "<<fx<<endl;
        for(int i=0;i<dim;i++){
            cout<<sp->getName(i)<<" = ";
            if (yinit==0){
                cout<< "yunit = 0! changed to y(i) only: ";
                cout<<x[i]<<"\t\tFxnew = "<<y[i]<<"\t\tDelta = "<<(abs(y[i]))<<"\%"<<endl;
            }else{
                cout<<x[i]<<"\t\tFxnew = "<<y[i]<<"\t\tDelta = "<<(abs(yinit-y[i])/yinit)*100<<"\%"<<endl;
            }
        };

        for(i=0;i<dim; i++){
            //resultvector(i+1)=mx[i];
            resultvector[i]=x[i];
        }

    }
    if(cur->AlgOpt=="GA"){
        qDebug()<<"GA\n";
        for(i=0;i<dim; i++){
            lower[i]=sp->getLower(i);
            val[i]=sp->getValue(i);
            resultvector[i]=sp->getValue(i); //result values vector from any optimization routines
            upper[i]=sp->getUpper(i);
        };
        gaBin2DecF(dim, lower, upper, sp->getPopSize(), sp->getNGen(), sp->getPMut(), sp->getPCross(), sp->getGABin(), resultvector);
        cout<<"GA Finished"<<endl;
    }

    /////////////////////////////////////////////////////
    if(cur->AlgOpt=="LevMar"){ //LevMar
        qDebug()<<"LevMar\n";
#define LM_DBL_PREC
            double info[LM_INFO_SZ];
                                        // O: information regarding the minimization. Set to NULL if don't care
                                        // info[0]= ||e||_2 at initial p.
                                        // info[1-4]=[ ||e||_2, ||J^T e||_inf,  ||Dp||_2, \mu/max[J^T J]_ii ], all computed at estimated p.
                                        // info[5]= # iterations,
                                        // info[6]=reason for terminating: 1 - stopped by small gradient J^T e
                                        //                                 2 - stopped by small Dp
                                        //                                 3 - stopped by itmax
                                        //                                 4 - singular matrix. Restart from current p with increased \mu
                                        //                                 5 - no further error reduction is possible. Restart with increased mu
                                        //                                 6 - stopped by small ||e||_2
                                        // info[7]= # function evaluations
                                        // info[8]= # Jacobian evaluations

                    double opts[LM_OPTS_SZ];
                                                            // I: opts[0-4] = minim. options [\tau, \epsilon1, \epsilon2, \epsilon3, \delta]. Respectively the
                                        // scale factor for initial \mu, stopping thresholds for ||J^T e||_inf, ||Dp||_2 and ||e||_2 and the
                                        // step used in difference approximation to the Jacobian. If \delta<0, the Jacobian is approximated
                                        // with central differences which are more accurate (but slower!) compared to the forward differences
                                        // employed by default. Set to NULL for defaults to be used.


                    int ret, itercount;

                    /*opts[0]=LM_INIT_MU; //LM_DIFF_DELTA; //<-- mu
                    opts[1]=LM_STOP_THRESH; //LM_STOP_THRESH; //<-- ||J^T e||_inf
                    opts[2]=LM_STOP_THRESH; //LM_STOP_THRESH;// <-- ||Dp||_2
                    opts[3]=LM_STOP_THRESH; //LM_STOP_THRESH; // <-- ||e||_2
                                            //opts[4]=-1;//NULL; //<-- If \delta<0, the Jacobian is approximated with central differences which are more accurate (but slower!)
                    opts[4]=LM_DIFF_DELTA;*/

                    /*opts[0]=1e-5; //LM_DIFF_DELTA; //<-- mu
                    //opts[0]=1e-6; //LM_DIFF_DELTA; //<-- mu
                    opts[1]=1.0e-21; //LM_STOP_THRESH; //<-- ||J^T e||_inf
                    opts[2]=1.0e-21; //LM_STOP_THRESH;// <-- ||Dp||_2
                    opts[3]=1.0e-21; //LM_STOP_THRESH; // <-- ||e||_2
                    //opts[4]=-1;//NULL; //<-- If \delta<0, the Jacobian is approximated with central differences which are more accurate (but slower!)
                    opts[4]=1e-7;*/

                    opts[0]=sp->getLM_mu(); //LM_DIFF_DELTA; //<-- mu
                    //opts[0]=1e-6; //LM_DIFF_DELTA; //<-- mu
                    opts[1]=sp->getLM_epsilon1(); //LM_STOP_THRESH; //<-- ||J^T e||_inf
                    opts[2]=sp->getLM_epsilon2(); //LM_STOP_THRESH;// <-- ||Dp||_2
                    opts[3]=sp->getLM_epsilon3(); //LM_STOP_THRESH; // <-- ||e||_2
                    //opts[4]=-1;//NULL; //<-- If \delta<0, the Jacobian is approximated with central differences which are more accurate (but slower!)
                    opts[4]=sp->getLM_JacobianStep();
                    itercount = sp->getLM_iter();

                    /*	#define LM_INIT_MU    	 1E-03
                            #define LM_STOP_THRESH	 1E-17
                            #define LM_DIFF_DELTA    1E-06*/


            //ColumnVector xinit(dim);
            double xinit[dim];
            //OptppInit_conv(dim, xinit);
            OptppInit(dim, xinit);
            double p[dim], lb[dim], ub[dim], hx[dim];
            //double opts[dim], info[dim];
            for (int i=0;i<dim;i++){
                    /*p[i]=xinit(i+1);
                    lb[i]=lower(i+1);
                    ub[i]=upper(i+1);*/

                p[i]=sp->getValue(i);
                lb[i]=sp->getLower(i);
                ub[i]=sp->getUpper(i);

                    //opts[i]=lower(i+1)/upper(i+1)*lower(i+1)*(1e-3)*accrcy;
                    cout<<"lb["<<i<<"] = "<<lb[i]<<"\t"<<"x["<<i<<"] = "<<p[i]<<"\t"<<"ub["<<i<<"] = "<<ub[i];
                    cout<<" opts["<<i<<"] = "<<opts[i]<<endl;
            };
            //int arr_size=get_y_size(experiment, sim_exp_num);
            //GetAllExperimentResults
            vector<double> y_res;
            y_res = sp->GetAllExperimentResults();
            //int arr_size=sp->GetAllExperimentResults().size();
            int arr_size=y_res.size();
            cout<<"arr_size = "<<arr_size<<endl;
            //double *x;

            //получаем результат эксперимента
            //x=get_y(experiment, sim_exp_num);
            cout<<" test"<<endl;
            double *x= new (nothrow) double[(int) arr_size];
            for (int i=0;i<((int)y_res.size());i++){
                    //cout<<" res["<<i<<"] = "<<y_res.at(i)<<endl;
                    x[i]=y_res.at(i);
            }

            cout<<"arr_size = "<<arr_size<<endl<<endl;

            { double *work, *covar;
            int m=dim; int n=arr_size;
            work= new double [(LM_DIF_WORKSZ(m, n)+m*m)];
                if(!work){
                    fprintf(stderr, "memory allocation request failed in main()\n");
                  exit(1);
            }

            covar=work+LM_DIF_WORKSZ(dim, arr_size);
            ret=dlevmar_bc_dif(LevMarEvalF, p, x, dim, arr_size, lb, ub, itercount, opts, info, work, covar, NULL);

            for (int i=0;i<dim;i++){
                    cout<<"lb["<<i<<"] = "<<lb[i]<<"\t"<<"x["<<i<<"] = "<<p[i]<<"\t"<<"ub["<<i<<"] = "<<ub[i];
                    cout<<" opts["<<i<<"] = "<<opts[i]<<endl;
            };

            cout<<"LevMar Res = "<<endl;
            //LevMarEvalF(p, x, m, n, (void*)&n);
            LevMarEvalF(p, x, m, n, NULL);
            cout<<"LevMar Res End"<<endl;

            for (int i=0;i<dim;i++){
                cout<<"lb["<<i<<"] = "<<lb[i]<<"\t"<<"x["<<i<<"] = "<<p[i]<<"\t"<<"ub["<<i<<"] = "<<ub[i];
                    cout<<" opts["<<i<<"] = "<<opts[i]<<endl;
            };

        printf("Covariance of the fit:\n");
        for(i=0; i<dim; i++){
          for(int j=0; j<dim; j++)
            printf("%g ", covar[i*dim+j]);
          printf("\n");
        }
        printf("\n");

        free(work);
       }

    delete [] x;


        printf("Results for Power:\n");

          printf("Levenberg-Marquardt returned %d in %g iter, reason %g\nSolution: ", ret, info[5], info[6]);
          for(i=0; i<dim; ++i) printf("%.7g ", p[i]);
          printf("\n\nMinimization info:\n");
          printf ("info[0]= ||e||_2 at initial p = %g\n",info[0]);
          printf("info[1-4]=[ ||e||_2, ||J^T e||_inf,  ||Dp||_2, mu/max[J^T J]_ii ], all computed at estimated p.\n");
          printf("              %g   %g   %g   %g\n",info[1], info[2], info[3], info[4]);
          printf("info[5]= %g iterations, \n", info[5]);
          printf("info[6]= reason for terminating: %g \n 1 - stopped by small gradient J^T e",info[6]);
          printf("\n 2 - stopped by small Dp \n 3 - stopped by itmax");
          printf("\n 4 - singular matrix. Restart from current p with increased mu \n 5 - no further error reduction is possible. Restart with increased mu");
          printf("\n 6 - stopped by small ||e||_2 \n");
          printf("info[7]= %g function evaluations \n info[8]= %g Jacobian evaluations\n\n", info[7], info[8]);

          /*for(i=0; i<LM_INFO_SZ; ++i)
            printf("%g ", info[i]);
          printf("\n");*/

          //saving result to sp
          cout<<"LM Saving res\n";
          //sp->get_real_val(nlp.getXc(),res)
          ColumnVector res(dim);
          for (int i=0;i<dim;i++){              
              //res(i+1)=p[i];
              resultvector[i]=p[i];
              qDebug()<<"res("<<i<<"+1) = "<<QString("%1").arg(res(i+1));
          }
          //sp->get_real_val(res,resultvector);
          //resultvector=res;
    };
    /////////////////////////////////////////////////////
    /////////////////////////////////////////////////////

    //Выводим результаты расчетов
    //Окончательный расчет с оптимальными значениями параметров
    //для получения результата расчета
    OptppFeval(dim, resultvector, fx, result);

    qDebug()<<"Result:\n";
    for(i=0;i<dim;i++){
        qDebug()<<QString::fromStdString(sp->getName(i))<<" = "
            <<"x["<<i<<"] = "<<resultvector[i]<<"\n";
        cur->spiceInit.at(i)=QString(QString("%1").arg(resultvector[i], 0, 'e', 5)).toStdString();
    }
    cur->result=fx;
    qDebug()<<"fx = "<<fx<<"\n";



    return 0;
}
////////////////////////////////////
void goldPDS(void(*func)(int n, double *a, double &fx), int n, double *xarr, double *l, double *u){

        int i=n;
        double tval=xarr[i];
        double finit;

        double tol=1e-6;
        double xtol=(u[i]-l[i])*tol;
        cout<<"X = "<<tval<<" XTOL = "<<xtol<<endl;

        double g=0.618034;
        double r=(u[i]-l[i])*g;
        double x1=l[i]+r;
        double x2=u[i]-r;
        double f1,f2,tf;

        func(n, xarr,finit);

        xarr[i]=x1;
        func(n, xarr,f1);
        xarr[i]=x2;
        func(n, xarr,f2);

        cout<<"\nf1("<<x1<<") = "<<f1<<" f2("<<x2<<") = "<<f2<<" finit("<<tval<<") = "<<finit<<endl<<endl;

        while((abs(x2-x1)>=xtol)){
                r=r*g;
                cout<<" r = "<<r<<" ";
                if(f1<f2){
                        xarr[i]=x2+r;
                        x2=x1;
                        f2=f1;
                        //arra[i]=x;
                        func(n, xarr,f1);
                        x1=xarr[i];
                        tf=f1;
                }else{
                        xarr[i]=x1-r;
                        x1=x2;
                        f1=f2;
                        //a[i]=x;
                        func(n, xarr,f2);
                        x2=xarr[i];
                        tf=f2;
                }
        }
        cout<<"xinit = "<<tval<<" Gold xTol = "<<xtol<<" abs(x2-x1) = "<<x2<<" - "<<x1<<" = "<<abs(x2-x1)<<endl;
}

void DirectSearch(void(*func)(int n, double *a, double &fx), int dim, double *x, double *lb, double *ub, double tol ){

        bool exit=false;
        int z=0;
        int iter=0;
        double tx;
        double f1,f2;
        while(!exit){
                func(dim,x,f1);

                for (int i=0; i<dim;i++){
                        iter++;
                        cout<<" I = "<<i<<endl;
                        tx=x[i];
                        goldPDS(func, i, x, lb, ub);
                        if(abs(tx-x[i])<=tol) z++;
                        cout<<"PDS TOL = "<<tol<<" abs(tx-x[i]) = "<<tx<<" - "<<x[i]<<" = "<<abs(tx-x[i])<<endl;

                }

                func(dim,x,f2);
                if(z==dim-1) {
                        //cout<<"abs(tx-x[i])<=tol is TRUE for all parameters\n";
                        exit=true;
                }
                if(abs(f2-f1)<tol) {
                        cout<<"Stop Criterie: f1 = "<<f1<<" f2 = "<<f2<<endl;
                        exit=true;
                }
                cout<<"Iteration "<<iter<<endl;
                if(iter>1000) exit=true;
        }
        cout<<"Last Iteration = "<<iter<<endl;
}


void PG(void(*func)(int n, double *a, double &fx), int dim, double *x, double *lb, double *ub,
                double lambda, double scale, double tol, double *dx){
int i, fin=0, iter=0;
double alpha0=lambda;
double y, y0;
double res;
double *grad;
double *res_x;
double *x_prev;
double *pds_xres;//result x vector after optim Lambda search
double *x_converted;
double *x_init;

double *a, *b;
double *l_converted, *u_converted;

a = new double[dim];
b = new double[dim];
x_converted = new double[dim];
l_converted = new double[dim];
u_converted = new double[dim];
x_init = new double[dim];

func(dim, x, res);
res_x = new double[dim];
x_prev = new double[dim];
pds_xres = new double[dim];
grad = new double[dim];

bounds_transform(dim, lb, ub, a, b);
for(int i=0;i<dim;i++){
                        cout<<"a["<<i<<"] = "<<a[i]<<" b["<<i<<"] = "<<b[i]<<endl;
}

get_trans_val(dim, a, b, x, x_converted);

for (i=0;i<dim;i++){
        //cout<<"x["<<i<<"] = "<<x[i]<<" x_converted["<<i<<"] = "<<x_converted[i]<<endl;
        res_x[i]=x[i];
        x_init[i]=x[i];
        x[i]=x_converted[i];
        l_converted[i]=-1;
        u_converted[i]=1;
}

while (fin!=1){
        iter=iter+1;

        //func(dim, x, y);
        //func(dim, x_init, y);
        trans_func(func, dim, a, b, x, y);
        y0=y;
        //Grad(func, dim, x, lb, ub, grad, dx);
        cout<<"lambda = "<<lambda<<endl;
        cout<<"\nGRAD START\n";
        Grad(func, dim, x, lb, ub, dx, grad, a, b);
        cout<<"GRAD STOP\n";

        cout<<endl;
        for(i=0;i<dim;i++){
                cout<<"grad["<<i<<"] = "<<grad[i]<<" ";
        }
        cout<<endl;
        double tmp;

        cout<<"\nIter = "<<iter<<" x:\n";
        double lambda_old=lambda;

        //cout<<"Lambda_l = "<<lambda_l<<" Lambda = "<<lambda<<" Lambda_U = "<<lambda_u<<endl;
        cout<<"Lambda_PDS Start\n"<<" Lambda"<<lambda<<endl;
        //Lambda_PDS(func, dim, grad, x, pds_xres, lambda, tol, lambda_l, lambda_u, lb, ub);
        //Step_opt(func, dim, grad, x, pds_xres, lambda, tol, lb, ub);

        //Step_opt(func, dim, grad, x, pds_xres, lambda, scale, tol, lb, ub, a, b);
        PG_step(func, dim, grad, x, lambda, scale, tol,lb, ub,  a, b);
        cout<<"Lambda_PDS END\n"<<"New Lambda"<<lambda<<endl;

        for(i=0;i<dim;i++) {
                tmp=x[i];
                x[i]=x[i]-(lambda*grad[i]);
                cout<<"x["<<i<<"]="<<x[i]<<" = "<<tmp<<" -("<<lambda<<" * "<<grad[i]<<");\n";
                if (x[i]<-1) x[i]=-1;
                if (x[i]>1) x[i]=1;
                cout<<"x["<<i<<"] = "<<x[i]<<" \n";
        }

        cout<<"Old Lambda ="<<lambda_old<<" New Lambda = "<<lambda<<endl;
        //func(dim, x, z);
        trans_func(func, dim, a, b, x, y);
        //y=z;

        //cout<<"Z = "<<z<<" Y = "<<y<<endl;
        cout<<"Y0 = "<<y0<<" Y = "<<y<<endl;

        if(y0>y){
                res=y;
                for(i=0;i<dim;i++) {
                        res_x[i]=x[i];
                }
                cout<<"z>y\n";
        }/*else{
                y=z;
        }*/

        //alpha0=lambda;
        lambda=alpha0;
        //lambda=lambda_old;
        //z=y;
        //y=z;
        //x0=x;

        if((abs(y0-y)<tol)or(y0<y)){
                fin=1;
                cout<<"abs(y0-y) ="<<abs(y0-y)<<" = "<<abs(y0)<<" - "<<abs(y)<<" tol\n";
                cout<<"TOL = "<<tol<<endl;
        }else{
                cout<<"abs(y0-y) = "<<abs(y0-y)<<" = "<<abs(y0)<<" - "<<abs(y)<<" >tol\n";
                cout<<"TOL = "<<tol<<endl;
        }
        //sleep(3);
        cout<<"Loop cycle finished\n";
};

get_real_val (dim, a, b, x, res_x);
for (i=0;i<dim;i++){
        x[i]=res_x[i];
};
cout<<"Iterations = "<<iter<<endl;

}



void Grad (void(*func)(int n, double *a, double &fx), int dim, double *x, double *lb, double *ub, double *dx, double *grad,
                double *a, double *b){

        double tmpx, grad1, grad2;
        double h;
        double tmpgrad1, tmpgrad2;
        double err;

        /*bounds_transform(dim, lb, ub, a, b);
        for(int i=0;i<dim;i++){
                                cout<<"a["<<i<<"] = "<<a[i]<<" b["<<i<<"] = "<<b[i]<<endl;
        }*/

        for(int i=0;i<dim;i++){
                h=dx[i];
                cout<<"\nGrad I: "<<i<<" dx = "<<dx[i]<<" h = "<<h<<endl;
                tmpx=x[i];

                if(!((x[i]-h<-1)or(x[i]+h>1))){
                        x[i]=tmpx-h;
                        cout<<"xl["<<i<<"] = "<<x[i]<<" = "<<tmpx<<" - "<<h<<endl;

                        trans_func(func, dim, a, b, x, grad1);

                        x[i]=tmpx+h;
                        cout<<"xh["<<i<<"] = "<<x[i]<<" = "<<tmpx<<" + "<<h<<endl;

                        trans_func(func, dim, a, b, x, grad2);

                        grad[i]=((grad2-grad1)/(2*h));

                        cout<<"grad = "<<grad[i]<<" = ("<<grad2<<" - "<<grad1<<")/(2* "<<dx[i]<<"))"<<" = "<<(grad2-grad1)<<"/(2* "<<dx[i]<<")"<<endl;

                }else{
                        if(x[i]-h<-1){
                                x[i]=tmpx;
                                cout<<"xl["<<i<<"] = "<<x[i]<<" = "<<tmpx<<endl;
                                trans_func(func, dim, a, b, x, grad1);

                                x[i]=tmpx+h;
                                cout<<"xh["<<i<<"] = "<<x[i]<<" = "<<tmpx<<" + "<<h<<endl;

                                trans_func(func, dim, a, b, x, grad2);

                                grad[i]=((grad2-grad1)/(h));
                                cout<<"grad = "<<grad[i]<<" = ("<<grad2<<" - "<<grad1<<")/( "<<dx[i]<<"))"<<" = "<<(grad2-grad1)<<"/( "<<dx[i]<<")"<<endl;
                        }
                        if(x[i]+h>1){
                                x[i]=tmpx-h;
                                cout<<"xl["<<i<<"] = "<<x[i]<<" = "<<tmpx<<" - "<<h<<endl;
                                trans_func(func, dim, a, b, x, grad1);

                                x[i]=tmpx;
                                cout<<"xh["<<i<<"] = "<<x[i]<<" = "<<tmpx<<endl;

                                trans_func(func, dim, a, b, x, grad2);

                                grad[i]=((grad2-grad1)/(h));
                                cout<<"grad = "<<grad[i]<<" = ("<<grad2<<" - "<<grad1<<")/( "<<dx[i]<<"))"<<" = "<<(grad2-grad1)<<"/( "<<dx[i]<<")"<<endl;
                        }
                }

                x[i]=tmpx;
        }
}


void bounds_transform (int n, double *min, double *max, double *a, double *b){
        //функция преобразования границ переменных для приведения их всех
        // к одним размерам. Используемая формула (с-ма ур-й)
        //{Amin+b=-1
        //{Amax+b=1
        //solution:
        //a=2/(max-min)
        //b=1-max*a
        int i;

        for(i=0;i<n;i++){
                a[i]=2/(max[i]-min[i]);
                b[i]=1-max[i]*a[i];
        }
}

void get_trans_val (int n, double *a, double *b, double *x, double *res){
        int i;
        for(i=0;i<n;i++){
                res[i]=a[i]*x[i]+b[i];
        }
}

void get_real_val (int n, double *a, double *b, double *x, double *res){
        int i;
        for(i=0;i<n;i++){
                res[i]=(x[i]-b[i])/a[i];
                cout<<"get_real_val res["<<i<<"]=(x["<<i<<"]-b["<<i<<"])/a["<<i<<"]; == "<<res[i]<<" = "<<"("<<x[i]<<" - "<<b[i]<<")/"<<a[i]<<endl;
        }
}

void trans_func(void(*func)(int d, double *x, double &f), int dim, double *a, double *b, double *val, double &fx){
        double *res;
        res = new double[dim];
        get_real_val (dim, a, b, val, res);
        func(dim, res, fx);
}


void PG_step(void(*func)(int n, double *a, double &fx), int n, double *grad, double *x,// double *xarr,
                                                        double &lambda, double scale, double tol, double *lb, double *ub,
                                                        double *a, double *b){

int k=0;
double step=lambda, tstep, low, up;
double finit, f1, f2;
double length=scale;
double *xarr;

xarr = new double[n];

cout<<"CG_step start\n";

        for(k=0;k<n;k++){
                xarr[k]=x[k]-(step*grad[k]);
                if (xarr[k]<-1) xarr[k]=-1;
                if (xarr[k]>1) xarr[k]=1;
                cout<<"xarr["<<k<<"] = "<<xarr[k]<<" ";
        }
        cout<<endl;

        //func(n, xarr,finit);
        trans_func(func, n, a, b, xarr, finit);
        cout<<"Step = "<<step<<endl;

        step=step/length;
        for(k=0;k<n;k++){
                xarr[k]=x[k]-(step*grad[k]);
                if (xarr[k]<-1) xarr[k]=-1;
                if (xarr[k]>1) xarr[k]=1;;
                cout<<"xarr["<<k<<"] = "<<xarr[k]<<" ";
        }
        cout<<endl;

        //func(n, xarr,f1);
        trans_func(func, n, a, b, xarr, f1);
        cout<<"Step = "<<step<<endl;
        step=step*length;

        step=step*length;
        for(k=0;k<n;k++){
                xarr[k]=x[k]-(step*grad[k]);
                if (xarr[k]<-1) xarr[k]=-1;
                if (xarr[k]>1) xarr[k]=1;
                cout<<"xarr["<<k<<"] = "<<xarr[k]<<" ";
        }
        cout<<endl;
        cout<<"Step*length = "<<step<<endl;
        trans_func(func, n, a, b, xarr, f2);
        cout<<"f1 = "<<f1<<" f2 = "<<f2<<" finit = "<<finit<<endl;

        if(!((finit<f1)and(finit<f2))){
        if (f1<f2){
                        while(f1<f2){
                                f2=f1;
                                tstep=step;
                                step=step/length;
                                cout<<"step=step/length = "<<step<<" / "<<length<<endl;
                                for(k=0;k<n;k++){
                                        xarr[k]=x[k]-(step*grad[k]);
                                        if (xarr[k]<-1) xarr[k]=-1;
                                        if (xarr[k]>1) xarr[k]=1;
                                }
                                trans_func(func, n, a, b, xarr, f1);
                                cout<<"Step = "<<step<<endl;
                        }
        }else{
                        while(f1>f2){
                                f1=f2;
                                tstep=step;
                                step=step*length;
                                cout<<"step=step*length = "<<step<<" * "<<length<<endl;
                                for(k=0;k<n;k++){
                                        xarr[k]=x[k]-(step*grad[k]);
                                        if (xarr[k]<-1) xarr[k]=-1;
                                        if (xarr[k]>1) xarr[k]=1;
                                        }
                                trans_func(func, n, a, b, xarr, f2);
                                cout<<"Step = "<<step<<endl;
                                }
                }

        if(tstep<step){
                low=tstep;
                up=step;
        }else{
                low=step;
                up=tstep;
        }

        }
        cout<<"low = "<<low<<" up = "<<up<<endl;
        PG_gold(func, n, step, low, up, grad, xarr, lb, ub, a, b);

        lambda=step;
        cout<<"Step_OPT Lambda = "<<lambda<<endl;
        cout<<"CG_step Finish\n";
        delete xarr;
}




void CG(void(*func)(int n, double *a, double &fx), int dim, double *x, double *lb, double *ub,
                double lambda, double scale, double tol, double *dx){

        double *Sk, *grad, *h, *a, *b, *x_converted, *res_x, *x_init, *l_converted, *u_converted;
        int iter=0, MAXITER=100, i=0;
        double fres, f;

        Sk=new double [dim];
        grad=new double [dim];
        h=new double [dim];
        a=new double [dim];
        b=new double [dim];
        x_converted=new double [dim];
        res_x=new double [dim];
        x_init=new double [dim];
        l_converted=new double [dim];
        u_converted=new double [dim];

        bounds_transform(dim, lb, ub, a, b);
        for(int i=0;i<dim;i++){
                                cout<<"a["<<i<<"] = "<<a[i]<<" b["<<i<<"] = "<<b[i]<<endl;
        }

        get_trans_val(dim, a, b, x, x_converted);

        for (i=0;i<dim;i++){
                //cout<<"x["<<i<<"] = "<<x[i]<<" x_converted["<<i<<"] = "<<x_converted[i]<<endl;
                res_x[i]=x[i];
                x_init[i]=x[i];
                x[i]=x_converted[i];
                l_converted[i]=-1;
                u_converted[i]=1;
        }

        trans_func(func, dim, a, b, x, fres);
        Grad(func, dim, x, lb, ub, dx, grad, a, b);
        PG_step(func, dim, grad, x, lambda, scale, tol,lb, ub,  a, b);

}

//void gold(void(*func)(int n, double *a, double &fx), double x, double l, double u){
void PG_gold (void(*func)(int d, double *a, double &fx), int n, double &step, double l,
                double u, double *grad, double *x, double *lb, double *ub, double *a, double *b){


                int i=0, k=0;
                double finit;
                //double l1, l2, lt;
                double *xarr;
                xarr=new double[n];

                for(i=0; i<n; i++){
                        xarr[i]=x[i];
                }

                cout<<"CG_gold START\n";

                int iter=20;

                double tol=1e-6;
                double xtol=(u-l)*tol;
                cout<<" XTOL = "<<xtol<<endl;

                double g=0.618034;
                double l1=u-(u-l)/g;
                double l2=l+(u-l)/g;
                double f1,f2;

                //func(n, xarr,finit);
                trans_func(func, n, a, b, xarr, finit);

                for(k=0;k<n;k++){
                        xarr[k]=x[k]-(l1*grad[k]);
                        //cout<<"xarr["<<k<<"] = xk["<<k<<"] - ("<<" l1 "<<" * "<<"grag["<<k<<"])"<<endl;
                        cout<<xarr[k]<<" = "<<x[k]<<" - ( "<<l1<<" * "<<grad[k]<<")"<<endl;
                        if (xarr[k]<-1) xarr[k]=-1;
                        if (xarr[k]>1) xarr[k]=1;
                        cout<<"xarr["<<k<<"] = "<<xarr[k]<<endl;
                }
                //func(n, xarr,f1);
                trans_func(func, n, a, b, xarr, f1);

                for(k=0;k<n;k++){
                        xarr[k]=x[k]-(l2*grad[k]);
                        //cout<<"xarr["<<k<<"] = xk["<<k<<"] - ("<<" l2 "<<" * "<<"grag["<<k<<"])"<<endl;
                        cout<<xarr[k]<<" = "<<x[k]<<" - ( "<<l2<<" * "<<grad[k]<<")"<<endl;
                        if (xarr[k]<-1) xarr[k]=-1;
                        if (xarr[k]>1) xarr[k]=1;
                        cout<<"xarr["<<k<<"] = "<<xarr[k]<<endl;
                }
                //xarr[i]=x2;
                //func(n, xarr,f2);
                trans_func(func, n, a, b, xarr, f2);

                cout<<"f1 = "<<f1<<" l1 = "<<l1<<" f2 = "<<f2<<" l2 = "<<l2<<endl<<endl;

                for (int i=0; i<=iter;i++){
                        //r=r*g;
                        //cout<<" r = "<<r<<" ";
                        if(f1<=f2){
                                cout<<"f1<=f2\n";
                                u=l2;
                                l2=l1;
                                l1=u-(u-l)/g;
                                f2=f1;

                                for(k=0;k<n;k++){
                                        xarr[k]=x[k]-(l1*grad[k]);
                                        if (xarr[k]<-1) xarr[k]=-1;
                                        if (xarr[k]>1) xarr[k]=1;
                                }
                                //func(n, xarr,f1);
                                trans_func(func, n, a, b, xarr, f1);
                                step=l2;
                                cout<<"step = "<<step<<endl;
                        }else{
                                cout<<"f1>f2\n";
                                l=l1;
                                l1=l2;
                                l2=l+(u-l)/g;
                                f1=f2;

                                for(k=0;k<n;k++){
                                        xarr[k]=x[k]-(l2*grad[k]);
                                        if (xarr[k]<-1) xarr[k]=-1;
                                        if (xarr[k]>1) xarr[k]=1;
                                }
                                //func(n, xarr,f2);
                                trans_func(func, n, a, b, xarr, f2);
                                step=l1;
                                cout<<"step = "<<step<<endl;

                        }
                        if((abs(l2-l1)>=xtol)){
                                if(f1<f2){
                                        step=l1;
                                }else{
                                        step=l2;
                                }
                                break;
                        }
                }
                cout<<"CG_gold Finish\n";
                delete xarr;
}


void OptppInit(int n, double* x){
sp->OptppInit(n,x);
};

void OptppInit_cv(int n, ColumnVector& x_cv){
    double x[n];
    int i;
    for(i=0;i<n;i++){
        x[i]=x_cv(i+1);
    }
sp->OptppInit(n,x);
    for(i=0;i<n;i++){
        x_cv(i+1)=x[i];
    }
};

/*void OptppInit(int n, ColumnVector& tx){
    double x[n];
    for(int i=0; i<n;i++){
        x[i]=tx(i+1);
    }
sp->OptppInit(n,x);

};*/

void OptppInit_conv(int n, double* x){
        cout<<"OptppInit_conv Start"<<endl;
        double res[n];
        sp->OptppInit(n,res);
        cout<<"sp->OptppInit(n,res);"<<endl;
        sp->get_trans_val(res,x);

        for(int i=0; i<n; i++){
                cout<<" xinit = "<<res[i]<<" x_сonv = "<<x[i]<<endl;
        }
        cout<<"OptppInit_conv Finish"<<endl;
};


void OptppInit_conv_cv(int n, ColumnVector& x_cv){
    cout<<"OptppInit_conv_cv Start"<<endl;
    double res[n];
    double x[n];
    sp->OptppInit(n,res);
    cout<<"sp->OptppInit(n,res);"<<endl;
    sp->get_trans_val(res,x);
    for(int i=0; i<n; i++){
        x_cv(i+1)=x[i];
        //cout<<" xinit = "<<x_cv(i+1)<<" x_сv = "<<x[i]<<endl;
    }
    cout<<"OptppInit_conv_cv Finish"<<endl;
};



void LevMarEvalF(double *p, double *x, int m, int n, void *data){
        //ColumnVector cvp(m);
        vector <double> vx(n);
        double fx;
        int result;        
        //OptppFeval_conv(m, cvp,fx,result);
        OptppFeval(m, p,fx,result);
        vx=sp->GetAllSimulationResults();
        for(int i=0;i<n;i++){
                x[i]=vx.at(i);
        }
}


void LevMarEvalQ(double *p, double *x, int m, int n, void *data){
        for(int i=0;i<n;i++){
                x[i]=p[0]*i*i+p[1];
        }
}

void OptppFeval_conv (int dim, double* x, double& fx, int& result){
//sp->OptppFeval(dim, x,fx,result);
sp->OptppFeval_conv(dim, x,fx,result);
//sp->OptppFeval(dim, tmp,fx,result);

};
void OptppFeval_conv_cv (int dim, const ColumnVector& tx, double& fx, int& result){
//sp->OptppFeval(dim, x,fx,result);
    double x[dim];
    for(int i=0;i<dim;i++){
        x[i]=tx(i+1);
    }
sp->OptppFeval_conv(dim, x,fx,result);
//sp->OptppFeval(dim, tmp,fx,result);

};

void OptppFeval_conv_shtraf(int dim, double* x, double& fx, int& result){
        //sp->OptppFeval(dim, x,fx,result);
        cout<<"\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Shtraf Start"<<endl;
        sp->OptppFeval_conv(dim, x,fx,result);
        double shtraf=0, tmp=0;
        double tmpx;
        //bool shtraff=false;

        for(int i=0;i<dim;i++){
                tmpx=x[i];
                cout<<"x("<<i<<")= "<<x[i]<<endl;
                //tmp=pow(abs(1-abs(tmpx)),100);
                tmp=pow(abs(tmpx),100);
                        shtraf=shtraf+tmp;
                        cout<<" Shtraf_["<<i<<"] = "<<shtraf<<" tmpshtraf = "<<tmp<<" tmpx = "<<tmpx<<endl;
          };
        cout<<"FX = "<<fx<<" SHTRAF = "<<shtraf<<endl;

        fx=fx+shtraf;
        cout<<"RES = "<<fx<<endl;
        cout<<"Shtraf End"<<endl;
        };

void OptppFeval_conv_shtraf_cv(int dim, const ColumnVector& x, double& fx, int& result){
        //sp->OptppFeval(dim, x,fx,result);
        cout<<"\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Shtraf Start"<<endl;
        OptppFeval_conv_cv(dim, x,fx,result);
        double shtraf=0, tmp=0;
        double tmpx;
        //bool shtraff=false;

        for(int i=1;i<=dim;i++){
                tmpx=x(i);
                cout<<"x("<<i<<")= "<<x(i)<<endl;
                //tmp=pow(abs(1-abs(tmpx)),100);
                tmp=pow(abs(tmpx),100);
                        shtraf=shtraf+tmp;
                        cout<<" Shtraf_["<<i<<"] = "<<shtraf<<" tmpshtraf = "<<tmp<<" tmpx = "<<tmpx<<endl;
          };
        cout<<"FX = "<<fx<<" SHTRAF = "<<shtraf<<endl;

        fx=fx+shtraf;
        cout<<"RES = "<<fx<<endl;
        cout<<"Shtraf End"<<endl;
};


void OptppFeval (int dim, double* x, double& fx, int& result){
sp->OptppFeval(dim, x,fx,result);
//sp->OptppFeval(dim, tmp,fx,result);
cout<<endl;
for(int i=0;i<dim;i++){
          cout<<" x("<<i<<") = "<<scientific<<x[i];
  };
  cout<<" Fx = "<<fx<<endl;
};



void Myfunc (int n, double *a, double &fx){
        int dim=sp->getXCount();
        //вектор считается от нуля
        int result;

        OptppFeval(dim, a,fx,result);

};
