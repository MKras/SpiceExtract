/*
 * options.cpp
 *
 *  Created on: 09.02.2009
 *      Author: spike
 */

#define GSL_DBL_EPSILON        2.2204460492503131e-16
#define GSL_MAX(a,b) ((a) > (b) ? (a) : (b))
#define GSL_MIN(a,b) ((a) < (b) ? (a) : (b))

#include "spiceextr.h"
#include <stdlib.h>// for MAX_PATH
#include <cmath>//

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include <sstream>
#include <locale>

// для отладки
//#include <QtGui>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QRegExp>
#include <QValidator>
#include <QProcess>
#include <QtScript>

using namespace std;

SpiceExtr::~SpiceExtr(){
}

SpiceExtr::SpiceExtr(): QObject(){
stop=false;

//QRegExp m("(.*\\/).*");
//int pos = m.indexIn(QString::fromStdString(path));
//if( pos > -1){
//    prefix = m.cap(1).toStdString();
//}else{
    prefix = "";
//}
        setFTol(1.49012e-8);
        setAccrcy(1);
        setMaxIter(10000);
        setFEval(100000);
        setBackTrackIter(1000000);
        setGtols(1e-8);
        //GA
        setGAalg(1);
        setPCross(0.8);
        setPMut(0.02);
        setNGen(250);
        setPopSize(100);
        setGAScaling(1);
        setGASelection(1);
        setGABin(24);
        setdx(1e-6);
        setLambda(1e-6);
        setErrorFunction(1);
        itercount=0;
        //out_pars = "{1},{2}";
        simulator = NGSpice;

        //Init NGwrapper
        NGWrapper_.reset(new NGSpiceWrapper());
        NGWrapper_->Init_dll_handler();
        NGWrapper_->Init_handlers();
        NGWrapper_->NGngSpice_Init_handle();

}


void SpiceExtr::setspice_path(string s){
        spice_path = s;
        //size_t found;
        if (spice_path.find("ngspice ")!=string::npos){//ngspice -b
            simulator=NGSpice;
        }
        if(spice_path.find("spectre ")!=string::npos){//spectre -format nutascii
            simulator=Spectre;
        }
        if (spice_path.find("gnucap ")!=string::npos){//gnucap -b
            simulator=GNUCap;
        }

};

string SpiceExtr::getspice_path(){
        return spice_path;
};

void SpiceExtr::setSpiceParams(string s){
        spiceparams = s;
};
string SpiceExtr::getSpiceParams(){
        return spiceparams;
};

void SpiceExtr::addSpiceIn(string s){
        spicein.push_back(s);
};
vector <string> SpiceExtr::getSpiceIn(){
        return spicein;
};
void SpiceExtr::addSpiceOut(string s){
        spiceout.push_back(s);
};
vector <string> SpiceExtr::getSpiceOut(){
        return spiceout;
};

void SpiceExtr::addSpiceExp(string s){
        spiceexp .push_back(s);
};
vector<string> SpiceExtr::getSpiceExp(){
        return spiceexp;
};
void SpiceExtr::setSpiceLib(string s){
        spicelib = s;
};
string SpiceExtr::getSpiceLib(){
        return spicelib;
};

void SpiceExtr::addVal(double v){
        val.push_back(v);
};
void SpiceExtr::addValName(string v_n){
        val_name.push_back(v_n);
};
void SpiceExtr::abbValLow(double v_l){
        val_low.push_back(v_l);
};
void SpiceExtr::addValUp(double v_u){
        val_up.push_back(v_u);
};

int SpiceExtr::getParamSize(){
        return val.size();
};

void SpiceExtr::setFTol (double ft){
        FTol=ft;
};
double SpiceExtr::getFTol (){
        return FTol;
};

void SpiceExtr::setAccrcy (double ac){
        accrcy=ac;
};
double SpiceExtr::getAccrcy (){
        return accrcy;
};
void SpiceExtr::setMaxIter(int m){
        maxiter=m;
};
int SpiceExtr::getMaxIter(){
        return maxiter;
};
void SpiceExtr::setFEval(int me){
        Feval=me;
};
int SpiceExtr::getFEval(){
        return Feval;
};
void SpiceExtr::setBackTrackIter(int bti){
        backtrackiter=bti;
};
int SpiceExtr::getBackTrackIter(){
        return backtrackiter;
};
void SpiceExtr::setGtols(double gt){
        Gtols=gt;
};
double SpiceExtr::getGtols(){
        return Gtols;
};
void SpiceExtr::setGAalg(int ga){
        gaalg=ga;
};
int SpiceExtr::getGAalg(){
        return gaalg;
};
void SpiceExtr::setPCross(float pc){
        pcross=pc;
};
float SpiceExtr::getPCross(){
        return pcross;
};
void  SpiceExtr::setPMut(float pm){
        pmut=pm;
};
float  SpiceExtr::getPMut(){
        return pmut;
};
void SpiceExtr::setNGen(int ng){
        ngen=ng;
};
int SpiceExtr::getNGen(){
        return ngen;
};
void SpiceExtr::setPopSize(int ps){
        popsize=ps;
};
int SpiceExtr::getPopSize(){
        return popsize;
};
void SpiceExtr::setGAScaling(int gs){
        gascaling=gs;
};
int SpiceExtr::getGAScaling(){
        return gascaling;
};
void SpiceExtr::setGASelection(int gs){
        gaselection=gs;
};
int SpiceExtr::getGASelection(){
        return gaselection;
};
void SpiceExtr::setGABin(int gb){
        gabin=gb;
};
int SpiceExtr::getGABin(){
        return gabin;
};

void SpiceExtr::setAlgorythm(int alg){
        algorithm=alg;
};
int SpiceExtr::getAlgorythm(){
        return algorithm;
};
double SpiceExtr::getValue(int i){
        //[0] - the firtst element
        return val[i];
};
double SpiceExtr::getUpper(int i){
        //[0] - the firtst element
        return val_up[i];
};
double SpiceExtr::getLower(int i){
        //[0] - the firtst element
        return val_low[i];
};
string SpiceExtr::getName(int i){
        //[0] - the firtst element
        return val_name[i];
}

double SpiceExtr::getdx(){
        return dx;
};
void SpiceExtr::setdx(double d){
        dx=d;
};
double SpiceExtr::getLambda(){
        return lambda;
};
void SpiceExtr::setLambda(double lamb){
        lambda=lamb;
};

void SpiceExtr::setValue(size_t i, double v){
        if(val.size()>i){
                val[i]=v;
        }else{
                cerr<<"SpiceExtr::setValue size is out of range"<<"val.size = "<<val.size()<<" i = "<<i<<endl;
        }
};
void SpiceExtr::setUpper(size_t i, double u){
        if(val_up.size()>i){
                val_up[i]=u;
        }else{
                cerr<<"SpiceExtr::getUpper size is out of range"<<"val_up.size = "<<val_up.size()<<" i = "<<i<<endl;
        }
};
void SpiceExtr::setLower(size_t i, double l){
        if(val_low.size()<=i){
                val_low[i]=l;
        }else{
                cerr<<"SpiceExtr::getLower size is out of ramge"<<"val_low.size = "<<val_low.size()<<" i = "<<i<<endl;
        }
};

//LevMar Gettrers - Setters
double SpiceExtr::getLM_mu(){
        return LM_mu;
};
void SpiceExtr::setLM_mu(double val){
        LM_mu=val;
};

double SpiceExtr::getLM_epsilon1(){
        return LM_epsilon1;
};
void SpiceExtr::setLM_epsilon1(double val){
        LM_epsilon1=val;
};

double SpiceExtr::getLM_epsilon2(){
        return LM_epsilon2;
};
void SpiceExtr::setLM_epsilon2(double val){
        LM_epsilon2=val;
};

double SpiceExtr::getLM_epsilon3(){
        return LM_epsilon3;
};
void SpiceExtr::setLM_epsilon3(double val){
        LM_epsilon3=val;
};

double SpiceExtr::getLM_JacobianStep(){
        return LM_JacobianStep;
};
void SpiceExtr::setLM_JacobianStep(double val){
        LM_JacobianStep=val;
};

int SpiceExtr::getLM_iter(){
        return LM_iter;
};
void SpiceExtr::setLM_iter(int val){
        LM_iter=val;
};

void SpiceExtr::setErrorFunction(int e){
        ErrorFunction = e;
};

int SpiceExtr::getErrorFunction(){
        return ErrorFunction;
};

/*void SpiceExtr::Run(){
        getParams();       
};*/

double SpiceExtr::GetGradStep(size_t i){
        return gradstep[i];
};
void SpiceExtr::SetGradStep(size_t i, double step){
        if(gradstep.size()<=i){
                gradstep[i]=step;
                }else{
                        cerr<<"SpiceExtr::SetGradStep size is out of ramge"<<"val_low.size = "<<val_low.size()<<" i = "<<i<<endl;
                }
};

/*void SpiceExtr::Init(){
        getParams();
        getOptInput(prefix+getSpiceParams());        
        setSpiceInputValues(prefix+getSpiceLib());
        for(int i=0;i<val.size();i++){
                gradstep.push_back((val_up[i]-val_low[i])*accrcy);
        }
        bounds_transform();

};*/

void SpiceExtr::Init(curve *cur){
    /*qDebug()<<"\nINIT\n";
    qDebug()<<"bool SpiceExtr::Init(curve *cur)\n";
    for(int i=0;i<cur->inFile.size();i++){
        qDebug()<<QString::fromStdString(cur->outFile.at(i))<<"\n";
        qDebug()<<QString::fromStdString(cur->outFile.at(i))<<"\n";
        qDebug()<<QString::fromStdString(cur->expFile.at(i))<<"\n";
    }

    qDebug()<<"SpiceExtr::Init Model Path = ";
    qDebug()<<QString::fromStdString(cur->model_path)<<"\n";*/

        getParams(cur);
        //getOptInput(prefix+getSpiceParams());
        qDebug()<<"prefix+spicelib = "<<QString::fromStdString(prefix+getSpiceLib())<<"\n";
        //setSpiceInputValues(prefix+spicelib);

        prefix = cur->get_prefix();

        setSpiceInputValues(prefix+getSpiceLib());
        for(size_t i=0;i<val.size();i++){
                gradstep.push_back((val_up[i]-val_low[i])*accrcy);
        }
        bounds_transform();
};

void SpiceExtr::getParams(curve *cur){
size_t i=0;
    qDebug()<<"void SpiceExtr::getParams(curve *cur)\n";   
    qDebug()<<"SpiceExtr::getParams ModelPath = "<<QString::fromStdString(cur->model_path)<<"\n";

    for(i=0;i<cur->inFile.size();i++){           
              addSpiceOut(cur->outFile.at(i));           
              addSpiceIn(cur->inFile.at(i));           
              addSpiceExp(cur->expFile.at(i));
        }
    qDebug()<<"void SpiceExtr::getParams(curve *cur)2\n";

    string low="";
    string init="";
    string up="";

    for(i=0;i<cur->spiceParam.size();i++){

        addValName(cur->spiceParam.at(i));

        double low=0.0f;
        double init=0.0f;
        double max=0.0f;
        std::istringstream istrl(cur->spiceMin.at(i).c_str());
        istrl>>low;
        std::istringstream istri(cur->spiceInit.at(i).c_str());
        istri>>init;
        std::istringstream istrm(cur->spiceMax.at(i).c_str());
        istrm>>max;

        abbValLow(low);
        addVal(init);
        addValUp(max);

    }
    qDebug()<<"void SpiceExtr::getParams(curve *cur)3\n";

    qDebug()<<" setSpiceLib(cur->model_path);"<<"\n";
    setSpiceLib(cur->model_path);
    qDebug()<<QString::fromStdString(cur->model_path)<<"\n";
    qDebug()<<" setSpiceLib(cur->model_path);"<<"\n";

    //////////////////////////////////////////////////////////////////////////////////

    //if( pos > -1 ) setSpiceParams(m.cap(1).toStdString());

    setspice_path(cur->spice);
    //Optimization Parameters (OPT++)
    setFTol( cur->FTol );
    setAccrcy( cur->accrcy);
    setMaxIter( cur->maxiter);
    setFEval( cur->Feval );
    setBackTrackIter(cur->backtrackiter);
    setGtols(cur->Gtols);

    setdx( cur->dx);
    setLambda(cur->lambda );

    setGAalg( cur->gaalg);
    setPCross( cur->pcross );
    setPMut( cur->pmut );
    setNGen( cur->ngen );
    setPopSize( cur->popsize );
    setGAScaling( cur->gascaling );
    //setGASelection( cur->ga ); not used
    setGABin( cur->bin );

    setLM_mu( cur->LM_mu );
    setLM_epsilon1( cur->LM_epsilon1 );
    setLM_epsilon2( cur->LM_epsilon2 );
    setLM_epsilon3( cur->LM_epsilon3 );
    setLM_JacobianStep(cur->LM_JacobianStep );
    setLM_iter( cur->LM_iter );

    setErrorFunction( cur->ErrorFunction);
    out_pars = cur->OutParse;

    //getParams();

    //заполняем инфу о входных и выходных файлах
    qDebug()<<"void SpiceExtr::getParams(curve *cur)"<<"\n";
}




void SpiceExtr::getOptInput(string spice_param){
        string line;
        ifstream inp;
        QRegExp m("(.*)\\s+(.*)\\s+(.*)\\s+(.*).*");        
                inp.open(spice_param.c_str(), ios::in);
                if (inp.is_open()){
                                while (! inp.eof() ){
                                      getline (inp,line);
                                      int pos = m.indexIn(QString::fromStdString(line));
                                      if ( pos > -1 ) {
                                          addValName( m.cap(1).toStdString() );
                                          abbValLow( m.cap(2).toDouble() );
                                          addVal( m.cap(3).toDouble() );
                                          addValUp( m.cap(4).toDouble() );
                                      }
                                }
                }else{
                        cout<<"False to open "<<spice_param<<endl;
                }
                inp.close();
};


double SpiceExtr::getVal_by_Name(string v_n){
        for(size_t i=0; i<val_name.size();i++){
                if(val_name[i] == v_n) {

                cout<<"getVal_by_Name: "<<val_name[i]<<" "<<val[i]<<endl;
                return val[i];
                }else return NULL;
        }
        cerr<<"There no Value for this parameter Name\n";
        return NULL;
};

// в данной версси будет предполагаться, что настраиваемая модель хранится в отдельном файле.
// + определенный синтаксис файла: param=val - без пробелов
void SpiceExtr::setSpiceInputValues(string sl){
    emit startModelChanging();

        fileWrites=false;
        string filename=sl+"_tmp";
        //QRegExp m;

        QFile new_f(QString::fromStdString(filename.c_str()));
        QFile old_f(QString::fromStdString(sl.c_str()));
        if(old_f.open(QIODevice::ReadWrite)){
            if(new_f.open(QIODevice::ReadWrite)){
                QTextStream in_stream( &old_f );
                QTextStream out_stream( &new_f );
                QString line;
                while(!in_stream.atEnd()){
                    line = in_stream.readLine();
                    if (line.indexOf("*")!=0){
                        QString res_str="";
                        QRegExp m;
                        //m.setPattern(string("[+]?\\s*([\\w\\d]+)\\s*=\\s*([[-+]?\\d*\\.?\\d*[eE]?[-+]?\\d*)").c_str());
                        m.setPattern(string("([\\w\\d]+)\\s*=\\s*([[-+]?\\d*\\.?\\d*[eE]?[-+]?\\d*)").c_str());
                        res_str=line;
                              int pos = 0;//m.indexIn(line);
                              while ((pos = m.indexIn(line, pos)) != -1) {
                                   QString  line_par = m.cap(1);
                                   for(size_t k=0; k<val_name.size();k++){
                                       stringstream d2str;
                                       string tstr;
                                       d2str<<val[k];
                                       tstr=d2str.str();
                                       if((QString::compare(line_par, QString::fromStdString(val_name[k]), Qt::CaseSensitive)==0)&&
                                          (m.cap(2).length()!=0)  ) {
                                           line=line.mid(0, pos)+m.cap(1)+" = "+QString::fromStdString(tstr)+line.mid(line.indexOf(m.cap(2), pos+m.cap(1).length()+1)+m.cap(2).length());
                                       }
                                   }
                                   pos += m.matchedLength();
                               }
                              }
                              out_stream<<line<<"\n";
                }            
            }else{
                qDebug()<<"setSpiceInputValues NewFile Error\n";
                exit(0);
            }
        }else{
            qDebug()<<"setSpiceInputValues OldFile Error\n";
            exit(0);            
        }
        old_f.close();
        new_f.close();

        if(old_f.remove()){        
            if(!new_f.rename(QString::fromStdString(sl.c_str()))){
                qDebug()<<"Error file"<< QString::fromStdString(filename.c_str())<<" renaming failed\n";               
                exit(0);               
            }else {}
        }else{

        }

        fileWrites=true;
        emit stopModelChanging();        
};

int SpiceExtr::CountExperimentPoints(){
                ExperimentPoints=0;
        for(size_t i=0;i<spicein.size();i++){
                ExperimentPoints=ExperimentPoints+GetExperimentResults_xy(prefix+spiceexp[i]).y.size();
        }
        return ExperimentPoints;
};

xyData SpiceExtr::runNGSpice(string spice_path){
    qDebug()<<"runNGSpice or GNUCap";

    QDir dir;
    qDebug() <<"Current Dir: " << dir.absolutePath() << endl;

    std::string path = dir.absolutePath().toStdString()+"/"+spice_path;
    qDebug()<<"SpiceExtr::runNGSpice spice_path="<<QString::fromStdString(path);

   //std::vector<std::string> cir_vec = spice_cir_to_vector(spice_path);
    //NGWrapper_->load_cir(cir_vec);

    NGWrapper_->load_cir(spice_path);

    //exec=spice_path+" "+tmpspicein+" > "+tmpspiceout;

    //sys_res=system(exec.c_str());

     NGWrapper_->bg_run();
     NGWrapper_->wait_until_simulation_finishes();

     simulation_result_T res_vec = NGWrapper_->get_AllVecs(NGWrapper_->get_CurPlot());

     xyData res=GetSimulationResults_xy(res_vec);

     return res;

}
double SpiceExtr::RunSimulation(){    
        char *cp=get_current_dir_name ();

        double res=0;
        int sys_res;

        size_t found;       
        string tmplib=getSpiceLib();
        found = tmplib.find_last_of("/");
        tmplib=tmplib.substr(0, found+1);        
        string tmpspicein, tmpspiceout, tmpspiceexp;
        string cddir=prefix+tmplib;        
        chdir(cddir.c_str());

        qDebug()<<"SpiceExtr::RunSimulation start loop";
        for(size_t i=0;i<spicein.size();i++){


                tmpspicein=spicein[i];
                tmpspiceout=spiceout[i];
                tmpspiceexp=spiceexp[i];

                tmpspicein = tmpspicein.substr(found+1);
                tmpspiceout = tmpspiceout.substr(found+1);
                tmpspiceexp = tmpspiceexp.substr(found+1);


                //spectre -format nutascii  input.scs
                //string exec=spice_path+" -b "+tmpspicein+" > "+tmpspiceout;

                string exec="";

                if (simulator==NGSpice || simulator==GNUCap){//NGSpice or GNUcap - the run in the same
                    //попытаемся реализовать собственный параметрический анализ
                    //exec=spice_path+" "+tmpspicein+" > "+tmpspiceout;
                    //sys_res=system(exec.c_str());

                    //runNGSpice(spice_path, tmpspicein, tmpspiceout);
                    qDebug()<<"before ReStmp["<<i<<"]"<<" "<<QString::fromStdString(exec);
                    res=res+CompareCurves(runNGSpice(tmpspicein),GetExperimentResults_xy(tmpspiceexp), tmpspiceout, tmpspiceexp);
                    qDebug()<<"piceExtr::RunSimulation ReStmp["<<i<<"] ="<<res;
                }
                else {
                    SpiceExtr_Exception("Unknown simulator");
                }
        }
        chdir(cp);        
        res=res/spicein.size();        
        free(cp);
        qDebug()<<"SpiceExtr::RunSimulation FINISH = "<<res;
        return res;
};

vector<double> SpiceExtr::GetAllSimulationResults(){
        vector <double> allres;

        for(size_t i=0;i<spiceout.size();i++){
                qDebug()<<" SpiceExtr::GetAllSimulationResults";

                QRegExp rx("^\\s*([+-]?\\d+.?\\d*[eE]?[+-]?\\d*)\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*).*");
                QRegExpValidator v(rx, 0);
                QString s;
                QFile sim(QString::fromStdString("./"+spiceout[i]));
                        if(sim.open(QIODevice::ReadOnly)){
                            QTextStream stream( &sim );
                                QString line;
                                while(!stream.atEnd()){
                                    line = stream.readLine();

                                        //это экспериментальные данные, которые должны быть представленны в виде двух колонок
                                        //1-я колонка - X, 2-я - Y

                                    int pos = rx.indexIn(line);
                                    if (pos > -1) {
                                        //qDebug()<<line;
                                        allres.push_back(rx.cap(2).toDouble());
                                    }
                        };

                }else{
                        cerr<<"Can't open file for GetSimulationResults ";
                };

                sim.close();
            }

        return allres;
}

bool SpiceExtr::NGSpiceOut(simulation_result_T sp_sim, const string first, const string second, xyData *res_xy){
    return NGWrapper_->NGSpiceOut(sp_sim, first, second, res_xy);
}

bool  SpiceExtr::NGSpiceOut(QTextStream *stream, QTextStream *tmpstream, QString first, QString second, xyData *res_xy){

    QRegExp rx_global("^\\s*(\\d+)\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*,?)\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*,?).*");
    QRegExp rx_local("\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*)");
    QRegExp val("(\\{\\d+\\})");

    QString line;
    //qDebug()<<"NGSpiceOut";
    QString first_str_out;
    QString second_str_out;


    while(!stream->atEnd()){
        line = stream->readLine();



    QString first_out, second_out;
    int pos = rx_global.indexIn(line);

    if(pos>-1){
        QVector<double> dv;
        pos=0;
        //qDebug()<<"LINE = "<<line;
        while ((pos = rx_local.indexIn(line, pos)) != -1) {
            dv.append(rx_local.cap(1).toDouble());
             pos += rx_local.matchedLength();
         }
        int size=dv.size();

        //First expr
        pos=0;
        first_out=first;
        while ((pos = val.indexIn(first_out, pos)) != -1) {
            bool ok=false;
            int val_int=val.cap(1).mid(1,val.cap(1).length()-2).toInt(&ok);
            //qDebug()<<val.cap(1).mid(1,val.cap(1).length()-2)<<" val_int="<<QString("%1").arg(val_int);
            if((ok)&&(val_int<=size)){
                first_out.replace(pos,val.cap(1).length(), "("+QString("%1").arg(dv.at(val_int-1))+")");
            }
             pos += val.matchedLength();
         }

        //Second expr
        pos=0;
        second_out=second;
        while ((pos = val.indexIn(second_out, pos)) != -1) {

            bool ok=false;
            int val_int=val.cap(1).mid(1,val.cap(1).length()-2).toInt(&ok);
            //qDebug()<<val.cap(1).mid(1,val.cap(1).length()-2)<<" val_int="<<QString("%1").arg(val_int);

            if((ok)&&(val_int<=size)){
                second_out.replace(pos,val.cap(1).length(), "("+QString("%1").arg(dv.at(val_int-1))+")");
            };
             pos += val.matchedLength();
         }

        //Не забываем записать результат в файл!
        QScriptEngine engine_first;
        QScriptEngine engine_out;

        first_str_out = engine_first.evaluate(first_out).toString();
        second_str_out = engine_out.evaluate(second_out).toString();
        res_xy->x.push_back(first_str_out.toDouble());
        res_xy->y.push_back(second_str_out.toDouble());
        //qDebug()<<" NGSpiceOut "<<first_str_out.toDouble()<<" "<<second_str_out.toDouble()<<"\n";

        //tmpstream<<first_str_out.toDouble()<<" "<<second_str_out.toDouble()<<"\n";
        tmpstream->operator<<(first_str_out+" "+second_str_out+"\n"); //<<first_str_out.toDouble()<<" "<<second_str_out.toDouble()<<"\n";

    }
    //qDebug()<<"res size = "<<res_xy->x.size();

    }   return true;

}

bool  SpiceExtr::GNUcapOut(QTextStream *stream, QTextStream *tmpstream, QString first, QString second, xyData *res_xy){

    //QRegExp rx_global("^\\s*(\\d+)\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*[fpnum]?,?)\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*[fpnum]?,?).*");
    //QRegExp rx_global("^\\s*(\\d+.?\\d*)\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*[fpnum]?,?)\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*[fpnum]?,?).*");
    QRegExp rx_global("^\\s*(\\d+.?\\d*[fpnumk]?)\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*[fpnum]?).*");// на случай когда одна зависимость
    //QRegExp rx_local("\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*[fpnum]?)");
    QRegExp rx_local("\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*[fpnum]?)");
    QRegExp val("(\\{\\d+\\})");

    QString line;
    qDebug()<<"GNUcapOut start";
    QString first_str_out;
    QString second_str_out;

    while(!stream->atEnd()){
        line = stream->readLine();
        //qDebug()<<"Line: "<<line;

    QString first_out, second_out;
    int pos = rx_global.indexIn(line);

    if(pos>-1){
        QVector<double> dv;
        pos=0;
        //qDebug()<<"LINE = "<<line;
        while ((pos = rx_local.indexIn(line, pos)) != -1) {
            QString tmp;
            //qDebug()<<"Local: "<<rx_local.cap(1);

            tmp=rx_local.cap(1);

            if (rx_local.cap(1).endsWith("f")){
                tmp=rx_local.cap(1).mid(0,rx_local.cap(1).length()-2);
                tmp=tmp+"e-15";
            };

            if (rx_local.cap(1).endsWith("p")){
                tmp=rx_local.cap(1).mid(0,rx_local.cap(1).length()-2);
                tmp=tmp+"e-12";
            };
            if (rx_local.cap(1).endsWith("n")){
                tmp=rx_local.cap(1).mid(0,rx_local.cap(1).length()-2);
                tmp=tmp+"e-9";
            };
            if (rx_local.cap(1).endsWith("u")){
                tmp=rx_local.cap(1).mid(0,rx_local.cap(1).length()-2);
                tmp=tmp+"e-6";
            };
            if (rx_local.cap(1).endsWith("m")){
                tmp=rx_local.cap(1).mid(0,rx_local.cap(1).length()-2);
                tmp=tmp+"e-3";
            };
            if (rx_local.cap(1).endsWith("k")){
                tmp=rx_local.cap(1).mid(0,rx_local.cap(1).length()-2);
                tmp=tmp+"e+3";
            };
            //qDebug()<<tmp;

            //dv.append(rx_local.cap(1).toDouble());
            dv.append(tmp.toDouble());
             pos += rx_local.matchedLength();
         }
        int size=dv.size();

        //First expr
        pos=0;
        first_out=first;
        while ((pos = val.indexIn(first_out, pos)) != -1) {
            bool ok=false;
            int val_int=val.cap(1).mid(1,val.cap(1).length()-2).toInt(&ok);
            //qDebug()<<val.cap(1).mid(1,val.cap(1).length()-2)<<" val_int="<<QString("%1").arg(val_int);
            if((ok)&&(val_int<=size)){
                //qDebug()<<first_out;
                first_out.replace(pos,val.cap(1).length(), "("+QString("%1").arg(dv.at(val_int-1))+")");
                //qDebug()<<first_out;
            }
             pos += val.matchedLength();
         }

        //Second expr
        pos=0;
        second_out=second;
        while ((pos = val.indexIn(second_out, pos)) != -1) {

            bool ok=false;
            int val_int=val.cap(1).mid(1,val.cap(1).length()-2).toInt(&ok);
            //qDebug()<<val.cap(1).mid(1,val.cap(1).length()-2)<<" val_int="<<QString("%1").arg(val_int);

            if((ok)&&(val_int<=size)){
                second_out.replace(pos,val.cap(1).length(), "("+QString("%1").arg(dv.at(val_int-1))+")");
            };
             pos += val.matchedLength();
         }

        //Не забываем записать результат в файл!
        QScriptEngine engine_first;
        QScriptEngine engine_out;

        first_str_out = engine_first.evaluate(first_out).toString();
        second_str_out = engine_out.evaluate(second_out).toString();
        res_xy->x.push_back(first_str_out.toDouble());
        res_xy->y.push_back(second_str_out.toDouble());
        //qDebug()<<" GNUcapOut "<<first_str_out.toDouble()<<" "<<second_str_out.toDouble()<<"\n";

        //tmpstream<<first_str_out.toDouble()<<" "<<second_str_out.toDouble()<<"\n";
        tmpstream->operator<<(first_str_out+" "+second_str_out+"\n"); //<<first_str_out.toDouble()<<" "<<second_str_out.toDouble()<<"\n";

    }
    //qDebug()<<"res size = "<<res_xy->x.size();

    }   return true;

}

bool  SpiceExtr::Spectre_psfascii_Out(QTextStream *stream, QTextStream *tmpstream, QString first, QString second, xyData *res_xy){
//spice=spectre -format psfascii
    QRegExp rx_global("^\\s*(\\d+)[\\s\\t]+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*,?)[\\s\\t]+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*,?).*");
    QRegExp rx_local("[\".*\"] ([+-]?\\d+.?\\d*[eE]?[+-]?\\d*)");
    QRegExp val("(\\{\\d+\\})");

    QString line;
    QString first_out, second_out;

    QString first_str_out;
    QString second_str_out;

    while(!stream->atEnd()){
        line = stream->readLine();
        if(line.contains("VALUE", Qt::CaseSensitive)){
            line = stream->readLine();
            QString first_param=line.mid(0,line.indexOf("\" ")+1);
            int pos=0;
            QVector <double> dv;
            dv.append(line.mid(line.indexOf("\" ")+1, line.length()).toDouble());            

            line = stream->readLine();

            while(!stream->atEnd()){

                if(!line.contains(first_param)){//NOT Contains!!!!
                    dv.append(line.mid(line.indexOf("\" ")+1, line.length()).toDouble());

                }else{//Contains!!!!                    
                    int size=dv.size();

                    //First expr
                    pos=0;
                    first_out=first;
                    while ((pos = val.indexIn(first_out, pos)) != -1) {
                        bool ok=false;
                        int val_int=val.cap(1).mid(1,val.cap(1).length()-2).toInt(&ok);
                        //qDebug()<<val.cap(1).mid(1,val.cap(1).length()-2)<<" val_int="<<QString("%1").arg(val_int);
                        if((ok)&&(val_int<=size)){
                            first_out.replace(pos,val.cap(1).length(), "("+QString("%1").arg(dv.at(val_int-1))+")");
                        }
                         pos += val.matchedLength();
                     }

                    //Second expr
                    pos=0;
                    second_out=second;
                    while ((pos = val.indexIn(second_out, pos)) != -1) {

                        bool ok=false;
                        int val_int=val.cap(1).mid(1,val.cap(1).length()-2).toInt(&ok);
                        //qDebug()<<val.cap(1).mid(1,val.cap(1).length()-2)<<" val_int="<<QString("%1").arg(val_int);

                        if((ok)&&(val_int<=size)){
                            second_out.replace(pos,val.cap(1).length(), "("+QString("%1").arg(dv.at(val_int-1))+")");
                        };
                         pos += val.matchedLength();
                     }

                    //Не забываем записать результат в файл!
                    QScriptEngine engine_first;
                    QScriptEngine engine_out;

                    first_str_out = engine_first.evaluate(first_out).toString();
                    second_str_out = engine_out.evaluate(second_out).toString();
                    res_xy->x.push_back(first_str_out.toDouble());
                    res_xy->y.push_back(second_str_out.toDouble());
                    //qDebug()<<first_str_out.toDouble()<<" "<<second_str_out.toDouble()<<"\n";
                    //qDebug()<<first_str_out<<" "<<second_str_out<<"\n";

                    //tmpstream<<first_str_out.toDouble()<<" "<<second_str_out.toDouble()<<"\n";
                    tmpstream->operator<<(first_str_out+" "+second_str_out+"\n"); //<<first_str_out.toDouble()<<" "<<second_str_out.toDouble()<<"\n";

                    dv.clear();
                    dv.append(line.mid(line.indexOf("\" ")+1, line.length()).toDouble());
                }
                //qDebug()<<"LINE="<<line;
                line = stream->readLine();
                if(line.contains("END", Qt::CaseSensitive)){
                    //qDebug()<<"dev size = "<<dv.size()<<"\n";

                    int size=dv.size();

                    //First expr
                    pos=0;
                    first_out=first;
                    while ((pos = val.indexIn(first_out, pos)) != -1) {
                        bool ok=false;
                        int val_int=val.cap(1).mid(1,val.cap(1).length()-2).toInt(&ok);
                        //qDebug()<<val.cap(1).mid(1,val.cap(1).length()-2)<<" val_int="<<QString("%1").arg(val_int);
                        if((ok)&&(val_int<=size)){
                            first_out.replace(pos,val.cap(1).length(), "("+QString("%1").arg(dv.at(val_int-1))+")");
                        }
                         pos += val.matchedLength();
                     }

                    //Second expr
                    pos=0;
                    second_out=second;
                    while ((pos = val.indexIn(second_out, pos)) != -1) {

                        bool ok=false;
                        int val_int=val.cap(1).mid(1,val.cap(1).length()-2).toInt(&ok);
                        //qDebug()<<val.cap(1).mid(1,val.cap(1).length()-2)<<" val_int="<<QString("%1").arg(val_int);

                        if((ok)&&(val_int<=size)){
                            second_out.replace(pos,val.cap(1).length(), "("+QString("%1").arg(dv.at(val_int-1))+")");
                        };
                         pos += val.matchedLength();
                     }

                    //Не забываем записать результат в файл!
                    QScriptEngine engine_first;
                    QScriptEngine engine_out;

                    first_str_out = engine_first.evaluate(first_out).toString();
                    second_str_out = engine_out.evaluate(second_out).toString();
                    res_xy->x.push_back(first_str_out.toDouble());
                    res_xy->y.push_back(second_str_out.toDouble());
                    //qDebug()<<first_str_out.toDouble()<<" "<<second_str_out.toDouble()<<"\n";
                    //qDebug()<<first_str_out<<" "<<second_str_out<<"\n";

                    //tmpstream<<first_str_out.toDouble()<<" "<<second_str_out.toDouble()<<"\n";
                    tmpstream->operator<<(first_str_out+" "+second_str_out+"\n"); //<<first_str_out.toDouble()<<" "<<second_str_out.toDouble()<<"\n";

                    dv.clear();

                    break;
                }
            }
        }

    }   return true;

}

xyData SpiceExtr::GetSimulationResults_xy(simulation_result_T sp_sim){

    if( (0 == out_pars.pasrse_x.length()) && (0 == out_pars.pasrse_y.length()) ){
        qDebug()<<"GetSimulationResults_xy first Second parce erroe Set to Def.";
        std::string error;
        error = "Can't parce simulated data for compare: "+std::string(out_pars.pasrse_x)+" "+std::string(out_pars.pasrse_y);
        throw SpiceExtr_Exception(error);
    };

    xyData res_xy;

    if(simulator==NGSpice){//NGSpice
        NGSpiceOut(sp_sim,  out_pars.pasrse_x, out_pars.pasrse_y, &res_xy);
        qDebug()<<"NGSpiceOut";
    }else throw SpiceExtr_Exception("Not supported simulator");

    return res_xy;
}

vector<double> SpiceExtr::GetAllExperimentResults(){
        vector <double> allres;
        xyData tmpres;
        for(size_t i=0;i<spiceexp.size();i++){

                QRegExp rx("([+-]?\\d+.?\\d*[eE]?[+-]?\\d*)\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*).*");
                QRegExpValidator v(rx, 0);
                QString s;
                QFile exp(QString::fromStdString("./"+spiceexp[i]));
                        if(exp.open(QIODevice::ReadOnly)){
                            QTextStream stream( &exp );
                                QString line;
                                while(!stream.atEnd()){
                                    line = stream.readLine();

                                        //это экспериментальные данные, которые должны быть представленны в виде двух колонок
                                        //1-я колонка - X, 2-я - Y


                                    int pos = rx.indexIn(line);
                                    if (pos > -1) {
                                        allres.push_back(rx.cap(2).toDouble());
                                    }
                        };

                }else{
                        cerr<<"Can't open file for GetSimulationResults ";
                };
                exp.close();
            }
        return allres;
}

xyData SpiceExtr::GetExperimentResults_xy(string sp_exp){

    xyData res;
    qDebug()<<"GetExperimentResults_xy";

    QRegExp rx("\\s*([+-]?\\d+.?\\d*[eE]?[+-]?\\d*)\\s+([+-]?\\d+.?\\d*[eE]?[+-]?\\d*).*");
                 //QRegExpValidator v(rx, 0);
                QFile file(QString::fromStdString("./"+sp_exp));

                if(file.open(QIODevice::ReadOnly)){
                    QTextStream stream( &file );
                        QString line;
                        while(!stream.atEnd()){
                            line = stream.readLine();                                
                                //это экспериментальные данные, которые должны быть представленны в виде двух колонок
                                //1-я колонка - X, 2-я - Y

                            int pos = rx.indexIn(line);
                            if (pos > -1) {                           
                                //res.x.push_back(fabs(rx.cap(1).toDouble()));
                                //res.y.push_back(fabs(rx.cap(2).toDouble()));
                                res.x.push_back(rx.cap(1).toDouble());
                                res.y.push_back(rx.cap(2).toDouble());
                            }
                        };
                }else{                       
                    qDebug()<<"Can't open file for GetExperimentResults_xy ";
                }                
                file.close();               
                qDebug()<<"GetExperimentResults_xy finished";
                return res;
};

double SpiceExtr::CompareCurves_Err1( xyData sim, xyData exp){
    int zero=0; double tmp, res;
                for(size_t i=0;i<sim.x.size();i++){
                     if(sim.y[i]!=0){

                                tmp=fabs((sim.y[i]-exp.y[i])/sim.y[i]);
                                res=res+tmp;

                            }else zero++;
                }
                qDebug()<<"Final Res"<<res<<"/"<<(sim.y.size()-zero);
                res=res/(sim.y.size()-zero);

                if(res!=res) res=1;
     return res;
}
double SpiceExtr::CompareCurves_Err2( xyData sim, xyData exp){
    size_t zero=0; double tmp, res;
    //tmp = fabs(sim.y[i]-exp.y[i])/fabs(max(sim.y[i],exp.y[i]));
        for(size_t i=0;i<sim.y.size();i++){
            tmp = fabs((sim.y[i]-exp.y[i])/max(fabs(sim.y[i]),fabs(exp.y[i])));
            if(tmp!=tmp) zero++;
                else res=res+tmp;
                }
        if(sim.y.size()==zero) qDebug()<<"sim.y.size()==zero";
        res=res/(sim.y.size()-zero);
        if(res!=res) res=1;
        return res;
}

double SpiceExtr::CompareCurves_Err3( xyData sim, xyData exp){
    int zero=0; double tmp, res, a;
        for(size_t i=0;i<sim.y.size();i++){
                if(exp.x[i]!=0){
                        a=fabs((sim.y[i]-exp.y[i])/exp.y[i]);

                        tmp=a*a;
                        res=res+tmp;

                }else zero++;
        }
        res=sqrt(res)/(sim.y.size()-zero);//*100; //i.e. *100%
        if(res!=res) res=1;
        return res;
}

double SpiceExtr::CompareCurves_Err4_x( xyData sim, xyData exp){
size_t zero=0; double res_x;
double tmp_x, tmp;
int size = exp.x.size();

for(int i=0;i<size;i++){
    tmp_x=XviaY(exp,sim.y.at(i),i);
    tmp = fabs((exp.x.at(i)-tmp_x)/max(fabs(tmp_x),fabs(exp.x.at(i))));
    if(tmp!=tmp) zero++;
        else res_x=res_x+tmp;
}
if(sim.y.size()==zero){
    res_x=1;
}else res_x=res_x/(sim.y.size()-zero);

return res_x;
}

xyData SpiceExtr::GSL_approximate(xyData *sim, xyData *exp){
    size_t size = 0, i=0;
    xyData simOut;
    if(exp->y.size()>sim->y.size()) size=sim->y.size();
        else size=exp->y.size();

        /*try{
            if(sim->y.size()/exp->y.size()<0.5)
                qDebug()<<"GSL_approximate sim->y.size()/exp->y.size()<0.5";
            return simOut;
        }catch(...){
            qDebug()<<"GSL_approximate sim->y.size()/exp->y.size()<0.5";
            return simOut;
        }*/
    qDebug()<<"GSL_approximate exp.y.size() = "<<exp->y.size();
    double x[sim->y.size()], y[sim->y.size()], x_app[exp->y.size()], y_app[exp->y.size()];

    for (i=0;i<sim->y.size();i++) {
        x[i]= sim->x[i];
        y[i]= sim->y[i];
        //qDebug()<<x[i]<<" i = "<<i<<" "<<y[i];
    }

    qDebug()<<"block try{}catch next";

    try{

        gsl_interp_accel *acc
                = gsl_interp_accel_alloc ();
        gsl_spline *spline
                = gsl_spline_alloc (gsl_interp_cspline, size);


         gsl_spline_init (spline, x, y, size);


        for (i=0;i<exp->y.size();i++)
        {
            y_app[i] = gsl_spline_eval (spline, exp->x[i], acc);
            x_app[i] = exp->x[i];
            //qDebug()<<"y_app["<<i<<"] = "<<y_app[i];

        }
        gsl_spline_free (spline);
        gsl_interp_accel_free (acc);

        sim->x.clear();
        sim->y.clear();


        for (i=0;i<exp->y.size();i++)
        {
            sim->x.push_back(x_app[i]);
            sim->y.push_back(y_app[i]);
            simOut.x.push_back(x_app[i]);
            simOut.y.push_back(y_app[i]);
        }

    }catch(...){
        qDebug()<<"GSL approximation ERROR";
        simOut.x.clear();
        simOut.y.clear();
    }
    qDebug()<<"GSL_approximate end";
    return simOut;
}

double SpiceExtr::interpolate(int init, std::vector<double> x, std::vector<double> y, double x_val){
    //qDebug()<<"INTERPOLATE START";
    double res;
    size_t min, max, tmp;
    //if((init<=x.size())&&(init>0)){
    if((init>0)){

        if (init>=x.size()-1){
            init=x.size()-1;
        }

        min=init, max=init, tmp=init;

        if(x[init]>x_val){
            qDebug()<<"x[init]>x_val";
            while((x[tmp-1]<x[tmp])&&(x[tmp]>=x_val)){
                qDebug()<<"x[tmp] = "<<x[tmp]<<" x_val = "<<x_val;
                tmp--;
                min=tmp;
                if (tmp==0) break;                
            }
        }

        //tmp=init;
        if(x[tmp]<x_val){
            qDebug()<<"x[init]<x_val";
            while((x[tmp+1]>x[tmp])&&(x[tmp]<=x_val)){
                qDebug()<<"x[tmp] = "<<x[tmp]<<" x_val = "<<x_val;
                tmp++;
                max=tmp;
                if (tmp==x.size()-1) break;

            };
        }



    //find the nearest point
    /*if(x[init]<x_val){
        //qDebug()<<x[init]<<"<"<<x_val;
        while(x[tmp]<x_val){
            if ((tmp<x.size()-2)) {
                tmp++;
                max=tmp;
                //qDebug()<<"max="<<max;
            }else break;
        }
    }
    tmp=init;
        if(x[init]>x_val){
            //qDebug()<<x[init]<<">"<<x_val;
        while(x[tmp]>x_val){
            if((tmp>0)){
                tmp--;
                min=tmp;
                //qDebug()<<"min="<<min;
            }else break;
        }
    }

    }else {
        if(init>x.size()){
            qDebug()<<"init>x.size() "<<init<< ">"<<x.size();
            qDebug()<<"F(Xinit) = F("<<x[init]<<")="<<y[x.size()-1]<<" XVAL = "<<x_val<<"res = ";

            return y[x.size()-1];

            }
        }else{
            return y[0];
        }*/
    }else{
        return y[0];
    }

    try{
        if (min==max) {
            res=y[init];
            //qDebug()<<"min==max";
        }else if(x_val==x[min]){
            res=y[min];
            //qDebug()<<"x_val==x[min]";
        }else if(x_val==x[max]){
            res=y[max];
            //qDebug()<<"x_val==x[max]";
        }
        else {
            res = y[min] + (y[max]-y[min])/(x[max]-x[min]) * (x_val-x[min]);
            qDebug()<<"res: "<<res<<" = "<<y[min]<<" + ("<<y[max]<<"-"<<y[min]<<")/("<<x[max]<<"-"<<x[min]<<") * ("<<x_val<<"-"<<x[min]<<")";
            qDebug()<<"F(Xmin) = F("<<x[min]<<")="<<y[min]<<" F(Xmax) = F("<<x[max]<<")="<<y[max]<<" XVAL = "<<x_val<<"res = "<<res;
        }
    }catch(...){
        res=NAN;
    }
    //qDebug()<<"F(Xmin) = F("<<x[min]<<")="<<y[min]<<" F(Xmax) = F("<<x[max]<<")="<<y[max]<<" XVAL = "<<x_val<<"res = "<<res;
    qDebug()<<"x[init] ="<<x[init]<<" x_val = " << x_val<<" F(xinit)="<<y[init]<<"  F(x_val)="<<res;
    return res;
}

//линейная интерполяция данных (на случай разного количества экспериментальных
//и моделируемых точек)
xyData SpiceExtr::interpolation(xyData *sim, xyData *exp){
    xyData simOut;

    size_t i=0, size=0;

    if(exp->y.size()>sim->y.size()) size=sim->y.size();
        else size=exp->y.size();

        //std::vector <double> x(sim->y.size()), y(sim->y.size()), x_app(exp->y.size()), y_app(exp->y.size());
        std::vector <double> x, y;

        //for (i=0;i<sim->y.size();i++) {
            //x[i]= sim->x[i];
            //y[i]= sim->y[i];
            //qDebug()<<x[i]<<" i = "<<i<<" "<<y[i];
        //}

        try{
            if(exp->y.size()>sim->y.size()){
                for (i=0;i<exp->y.size();i++) {
                    x.push_back(exp->x[i]);
                }
            }else{
                for (i=0;i<sim->y.size();i++) {
                    x.push_back(sim->x[i]);
                }
            }
            for(i=0;i<x.size();i++){
                //qDebug()<<"X["<<i<<"]="<<x[i];
                y.push_back(interpolate(i, sim->x, sim->y, x[i]));
            }


            /*for (i=0;i<sim->y.size();i++) {
                //если точки Х не совпадают
                if(sim->x[i]!=exp->x[i]){
                    // I==0 такое маловероятно
                    if(i==0){
                        qDebug()<<"I==0";
                        if(sim->x[i]>exp->x[i]){
                            //y[i]=interpolate(i, i+1, exp->x, exp->y, sim->x[i]);
                            y.push_back(exp->y[i]);
                            x.push_back(sim->x[i]);

                        };
                        if(sim->x[i]<exp->x[i]){
                            y.push_back(interpolate(i, sim->x, sim->y, exp->x[i]));
                            x.push_back(sim->x[i]);
                        }
                    }else
                    // I==last element
                    if (i==sim->x.size()){
                        qDebug()<<"I==last";
                        if(sim->x[i]>exp->x[i]){
                            x.push_back(sim->x[i]);
                            //y[i]=interpolate(i, i+1, exp->x, exp->y, sim->x[i]);
                            y.push_back(interpolate(i, sim->x, sim->y, exp->x[i]));
                        };
                        if(sim->x[i]<exp->x[i]){
                            x.push_back(sim->x[i]);
                            //y[i]=interpolate(i, i+1, sim->x, sim->y, exp->x[i]);
                            y.push_back(exp->y[i]);
                        }
                    }// I belong (min elem, max elem)
                    else {
                        x.push_back(exp->x[i]);
                        y.push_back(interpolate(i, sim->x, sim->y, exp->x[i]));
                    }
                }

                qDebug()<<x[i]<<" i = "<<i<<" "<<y[i]<<" exp: "<<exp->x[i]<<" "<<exp->y[i];
            }

            //уравниваем количество точек
            if (x.size()!=exp->x.size()){
                if(x.size()<exp->x.size()){
                    for(i=0; i<exp->x.size();i++){
                        if(x[i]!=exp->x[i]) {
                            //exp->x.erase(exp->x.begin()+i);
                            //exp->y.erase(exp->y.begin()+i);
                            x.insert(x.begin()+i-1, exp->x[i]);
                            y.insert(y.begin()+i-1, interpolate(i, sim->x, sim->y, exp->x[i]));
                        }
                    }
                }
            }*/

            for (i=0;i<y.size();i++)
            {
                //sim->x.push_back(x_app[i]);
                //sim->y.push_back(y_app[i]);
                simOut.x.push_back(x[i]);
                simOut.y.push_back(y[i]);
                //qDebug()<<"Imterpolated: "<<x[i]<<" i = "<<i<<" "<<y[i]<<" exp:"<<exp->x[i]<<" "<<exp->y[i];
            }


        }catch(...){
            qDebug()<<"Interpolation ERROR";
            simOut.x.clear();
            simOut.y.clear();
        }

    return simOut;
}

double SpiceExtr::CompareCurves( xyData sim, xyData exp, string Fout, string Fexp){

        double res;//,tmp, a;
        //int zero=0; //для учета нулевого значения знаменателя, чтобы избежать деление на нуль
        res=0;
        // GSL
        //double *s_x,*s_y, *e_x, *e_y, *delta, *rel_err;
        qDebug()<<"before exp.x.size()";
        //int size = exp.x.size(); //все размеры равны
        size_t i;
        double res_y,res_x;

        qDebug()<<"before ErrorFunction";
        //на случай, когда при моделировании произошла ошибка (например, длина канала отрицательна)
        //и нету результатов расчета
        qDebug()<<"sim.y.size() and exp.y.size() = "<<sim.y.size()<<"    "<<exp.y.size();

        if (sim.y.size()!=exp.y.size()){
            //если не заполнены экспериментальные данные
            if((sim.y.size()!=0)&&((exp.y.size()==0))){
                // они заполнытся результатами моделирования
                for (size_t i=0;i<sim.x.size();i++) {
                    exp.x.push_back(sim.x[i]);
                    exp.y.push_back(sim.y[i]);
                }
                QFile FExp(QString::fromStdString("./"+Fexp));
                FExp.remove(QString::fromStdString("./"+Fexp));
                if(FExp.open(QIODevice::ReadWrite)){
                    QTextStream stream( &FExp );
                    for (i=0;i<exp.y.size();i++){
                        stream<<exp.x[i]<<" "<<exp.y[i]<<endl;
                    }
                }else{
                    cerr<<"Can't open file for GetSimulationResults ";
                };
                FExp.close();
            }else
                //случай, когда при моделировании произошла
                if((sim.y.size()==0)){
            for (size_t i=0;i<exp.x.size();i++) {
                sim.x.push_back(0);
                sim.y.push_back(0);
            }
        }else
            //когда количество экспериментальных точек не соответствует количеству точек при моделировании
            if (sim.y.size()!=exp.y.size()){
            //sim=GSL_approximate(&sim, &exp);
             sim=interpolation(&sim, &exp);
        }

        //сохраняем аппроксимированные значения в файл (результат моделирования)
        QFile Fsim(QString::fromStdString("./"+Fout));
        Fsim.remove(QString::fromStdString("./"+Fout));
        if(Fsim.open(QIODevice::ReadWrite)){
            QTextStream stream( &Fsim );
            for (i=0;i<exp.y.size();i++){
                stream<<sim.x[i]<<" "<<sim.y[i]<<endl;
            }
        }else{
            cerr<<"Can't open file for GetSimulationResults ";
        };
        Fsim.close();
        //сохранили
            res=1;
        };//else

            if(sim.x.size()==exp.x.size()){
                //предполагается, что количество точек эксперимента и моделирования одиноковое
                if (ErrorFunction == 1){
                    res = CompareCurves_Err1( sim, exp);
                    }else if (ErrorFunction == 2){
                        res = CompareCurves_Err2( sim, exp);
                        }else if (ErrorFunction == 3){
                            res = CompareCurves_Err3( sim, exp);
                            }else if(ErrorFunction == 4){ //расчет ошибки как предложено в статье...
                             //Опубликовано в журнале "Компоненты и технологии", №7, 2004 г., с.26-29, №8 с. 56-61 и №9, с.32-39.
                             //Моделирование МОП транзисторов
                             //МЕТОДОЛОГИЧЕСКИЙ АСПЕКТ
                             //В. Денисенко, к.т.н.,
                             //Victor@RLDA.ru

                            //zero=0;
                            res=0;
                            res_y=0;
                            //                                                     |    e      s |
                            //сначала расчитаем средне относительное отклонение по | F(x) - F(x) |
                            //                                                     |    i      i |
                            res_y = CompareCurves_Err2( sim, exp);
                            res_x=0;                                                       
                            res_x = CompareCurves_Err4_x( sim, exp);

                            if(res_x!=res_x) res=res_y;
                                else res=sqrt((res_y*res_y)+(res_x*res_x));

                            qDebug()<<"Res_x = "<<res_x<<" Res_y = "<<res_y<<"\nRes = "<<res;

                            if(res!=res) res=1;
                        }
        }else{
                cout<<"CompareCurves SIZES NOT are equal"<<" sim: "<<sim.x.size()<<" exp: "<<exp.x.size()<<endl;
        }        
        return res;
};

double SpiceExtr::XviaY(xyData data, double f, int i){
    size_t tmp;
    double res=0;

    bool erise;
    if(data.y.at(i)==f) {
        res=data.x.at(i);       
        return res;
    }
    tmp=i;
    //не является ли крайней точкой
    //if((i!=0)&&(i!=data.x.size())){
        //ф-я увеличивается по Х или уменьшается

    if(i==data.x.size()-1){
        if(data.y.at(i-1)<data.y.at(i)) erise=true;
            else erise=false;
        }else{
            if(data.y.at(i)<data.y.at(i+1)) erise=true;
                else erise=false;
        }

        //if(data.y.at(i)<data.y.at(i+1)){
        if(erise){
            //возраставет
            //больше или меньше заданного значения?
            if(data.y.at(i)<f){
                //меньше
                while(tmp<=data.x.size()){
                    if(tmp==data.x.size()){
                        res=data.x.at(data.x.size()-1);                        
                        return res;                        
                    }
                    if(tmp==0){
                        tmp++;
                    }
                    if(data.y.at(tmp)>f){                        
                        res=data.x.at(tmp-1)+((data.x.at(tmp)-data.x.at(tmp-1))/(data.y.at(tmp)-data.y.at(tmp-1)))*(f-data.y.at(tmp-1));                        
                        return res;
                    }                    
                    tmp++;
                }
            }else{
                //больше
                while(tmp>=0){

                    if(tmp==0){
                        res=data.x.at(0);                        
                        return res;
                    }
                    if(tmp==data.x.size()-1){
                        tmp--;
                    }
                    if(data.y.at(tmp)<f){                        
                        res=data.x.at(tmp)+((data.x.at(tmp+1)-data.x.at(tmp))/(data.y.at(tmp+1)-data.y.at(tmp)))*(f-data.y.at(tmp));                        
                        return res;
                    }
                    tmp--;
                }
            }
        }else{
            //убывает
            //больше или меньше заданного значения?
            if(data.y.at(i)<f){
                //меньше
                while(tmp>=0){
                    if(tmp==0){
                        res=data.x.at(0);                        
                        return res;
                    }
                    if(tmp==data.x.size()-1){
                        tmp--;
                    }
                    if(data.y.at(tmp)<f){                        
                        res=data.x.at(tmp)+((data.x.at(tmp+1)-data.x.at(tmp))/(data.y.at(tmp+1)-data.y.at(tmp)))*(f-data.y.at(tmp));                        
                        return res;
                    }
                    tmp--;
                }
            }else{
                //больше
                while(tmp<=data.x.size()){
                    if(tmp==data.x.size()){
                        res=data.x.at(data.x.size()-1);                       
                        return res;
                    }
                    if(tmp==0){
                        tmp++;
                    }
                    if(data.y.at(tmp)>f){                        
                        res=data.x.at(tmp-1)+((data.x.at(tmp)-data.x.at(tmp-1))/(data.y.at(tmp)-data.y.at(tmp-1)))*(f-data.y.at(tmp-1));                        
                        return res;
                    }
                    tmp++;
                }
            }
        }
    return res;
}

double SpiceExtr::Simulate(vector<double>inputval){
        if (inputval.size()==val.size()){
                for(size_t i=0;i<inputval.size();i++){
                        val[i]=inputval[i];
                }
                return RunSimulation();
        }else{
                cerr<<"SpiceExtr::Simulate size inputval != parameters size\n";
                exit(1);
        }
};
int SpiceExtr::getParametersNumber(){
        return val.size();
};

void SpiceExtr::OptppInit(int n, double* x){
        for(size_t i=0;i<val.size();i++){
                x[i]=val[i];
        }     
};

int SpiceExtr::getXCount(){
        return val.size();
}

void SpiceExtr::OptppFeval (int dim, double *x, double& fx, int& result){
    //cout<<"SpiceExtr::OptppFeval IN\n";
    //stop -  исрользуется для прекращения расчетов
    //по-другому поток итить не знаю как)
    //тут неБОЛЬШИЕ грабли. Т.к. толком не понимаю, как правильно истановить выполнение данного класса
    //из потока. По симу ввожу переменную stop. Если stop, то просто ф-я просто возращает абы что, НО
    //до этого в потоке возвращаю исходные значения в GUI.
    if(!stop){
        itercount++;
        cout<<"Evaluating: "<<itercount<<endl;
        if(dim==val.size()){
                cout<<"OptppFeval:"<<endl;
                for(int i=0;i<dim;i++){
                        setValue(i, x[i]);
                        cout<<"x("<<i<<") = "<<x[i]<<" ";
                }                
                setSpiceInputValues(prefix+getSpiceLib());                
                fx=RunSimulation();
                cout<<"Fx = "<<fx<<endl;
        }else{
                cout<<"x.size = "<<dim<<" val.size = "<<val.size()<<endl;
                cerr<<"OptppFeval size of Column Vector != parameters number\n";
                exit(1);
        }        
        QString res="";
        for(int i=0;i<dim;i++){
            res=res+QString::fromStdString(getName(i))+"_x("+QString("%1").arg(i)+") = "+QString("%1").arg(x[i])+" ";
        }
        res=res+QString("Fx = %1").arg(fx);        
        emit resF(res);
    }else{
        qDebug()<<"SpiceExtr::OptppFeval dropped\n";
    }
};


/*void SpiceExtr::OptppGradeval (int dim, const ColumnVector& x, ColumnVector& grad, int& result){
        double fx;
        double h;//=1e-6;
    double res;
    double abserr;    
    int reduse_iter=10;
    ColumnVector Vabserr(dim);
        for(int i=1; i<=dim;i++){                
                h=gradstep[i-1];
                cout<<"\nOptppGradeval:\nh = "<<h<<endl;

                CentralDeriv_adapt_h (dim, x, fx, result, i, h, &res, &abserr);
                cout<<"LOOP "<<i<<"x("<<i<<") = "<<x(i)<<": "<<"Diff = "<<fx<<" AbsErr = "<<abserr<<" h = "<<h<<endl;

                grad(i)=fx;
                grad(i)=res;
                Vabserr(i)=abserr;
                cout<<"\n\n Iter Finish grad("<<i<<") = "<<grad(i)<<"x = "<<x(i)<<endl;
        }
        for(int i=1; i<=dim;i++){
                cout<<"grad("<<i<<") = "<<grad(i)<<" x = "<<x(i)<<" abserr = "<<Vabserr(i)<<endl;
        }
        sleep(3);
};*/

void SpiceExtr::OptppGradCalibre (int dim, double* x, double* grad, int& result){
        double fx;
        double h;//=1e-6;
    double res;
    double abserr;    
    int reduse_iter=10;
    double* Vabserr[dim];
        for(int i=0; i<dim;i++){

                h=gradstep[i];
                cout<<"\nOptppGradeval:\nh = "<<h<<endl;

                CentralDeriv_adapt_h (dim, x, fx, result, i, h, &res, &abserr);
                cout<<"LOOP "<<i<<"x("<<i<<") = "<<x[i]<<": "<<"Diff = "<<fx<<" AbsErr = "<<abserr<<" h = "<<h<<endl;

                if((0==fx)){
                        int k=0;                        
                        while(fx==0){
                                k++;
                                CentralDeriv_adapt_h (dim, x, fx, result, i, h, &res, &abserr);
                                cout<<"diff0 LOOP "<<k<<"x("<<i<<") = "<<x[i]<<": "<<"Diff = "<<fx<<" AbsErr = "<<abserr<<" h = "<<h<<endl;

                                if(fx==0){                                        
                                        h=h*1.25;
                                        cout<<"NEW H = "<<h<<endl;                               
                                }else{
                                        gradstep[i-i]=h;
                                        break;
                                }

                                if (k>reduse_iter){

                                        break;
                                }

                        }
                }
                grad[i]=fx;
                grad[i]=res;
                Vabserr[i]=&abserr;
                cout<<"\n\n Iter Finish grad("<<i<<") = "<<grad[i]<<"x = "<<x[i]<<endl;
        }
        for(int i=0; i<dim;i++){
                cout<<"grad("<<i<<") = "<<grad[i]<<" x = "<<x[i]<<" abserr = "<<Vabserr[i]<<endl;
        }
        sleep(3);
};

void SpiceExtr::CentralDeriv_adapt_h (int dim,  double *x, double& fx, int& res, int i/*X number for derivative*/, double h,
                   double *result, double *abserr){
        double r_0, round, trunc, error;

        double th_opt;         
        CentralDeriv(dim, x, res, i , h, &r_0, &round, &trunc);

          error = round + trunc;

          if (round < trunc && (round > 0 && trunc > 0))
            {
              double r_opt, round_opt, trunc_opt, error_opt;

              /* Compute an optimised stepsize to minimize the total error,
                 using the scaling of the truncation error (O(h^2)) and
                 rounding error (O(1/h)). */

              double h_opt = h * pow (round / (2.0 * trunc), 1.0 / 3.0);
              th_opt=h_opt;

              cout<<"\nCentralDeriv_adapt_h X("<<i<<") = "<<x[i]<<" h_opt = "<<h_opt<<endl;
              CentralDeriv(dim, x, res, i, h_opt, &r_opt, &round_opt, &trunc_opt);

              error_opt = round_opt + trunc_opt;

              /* Check that the new error is smaller, and that the new derivative.
                 is consistent with the error bounds of the original estimate. */

              if (error_opt < error && fabs (r_opt - r_0) < 4.0 * error)
                {
                  r_0 = r_opt;
                  error = error_opt;
                }
            }

          cout<<"CentralDeriv_adapt_h = "<<th_opt<<endl;          
          fx=r_0;
          *result = r_0;
          cout<<"CentralDeriv_adapt_h r_0 = "<<r_0<<endl;         
          *abserr = error;
          cout<<"CentralDeriv_adapt_h error = "<<error<<endl;
          cout<<"Deriv resul = "<<r_0<<" abserr = "<<error<<" h_opt = "<<th_opt<<endl;
}

//GSL adopted derivative algorithm
void SpiceExtr::CentralDeriv(int dim, double* x, int& res,
                int i /*X number for derivative*/, double h, double *result,
                double *abserr_round, double *abserr_trunc){

        /* Compute the derivative using the 5-point rule (x-h, x-h/2, x,
             x+h/2, x+h). Note that the central point is not used...

             Compute the error using the difference between the 5-point and
             the 3-point rule (x-h,x,x+h). Again the central point is not
             used. */


        cout<<"CentralDeriv: \nh = "<<h<<endl;
        double tmpx[dim];

        for(int ii=0;ii<dim;ii++){
                tmpx[ii]=x[ii];
        };        

        double fm1;
        tmpx[i]=x[i]-h;
        cout<<"tmpx("<<i<<"): "<<tmpx[i]<<"="<<x[i]-h<<endl;
        OptppFeval(dim, tmpx,fm1,  res);

        double fp1;
        tmpx[i]=x[i]+h;
        OptppFeval( dim, tmpx,  fp1,  res);

        double fmh;
        tmpx[i]=x[i]-h/2;
        OptppFeval( dim, tmpx,  fmh,  res);

        double fph;
        tmpx[i]=x[i]+h/2;
        OptppFeval( dim, tmpx,  fph,  res);

        double r3 = 0.5 * (fp1 - fm1);
        double r5 = (4.0 / 3.0) * (fph - fmh) - (1.0 / 3.0) * r3;

        double e3 = (fabs (fp1) + fabs (fm1)) * GSL_DBL_EPSILON;
        double e5 = 2.0 * (fabs (fph) + fabs (fmh)) * GSL_DBL_EPSILON + e3;

        double dy = GSL_MAX (fabs (r3), fabs (r5)) * fabs (x[i]) * GSL_DBL_EPSILON;

          /* The truncation error in the r5 approximation itself is O(h^4).
             However, for safety, we estimate the error from r5-r3, which is
             O(h^2).  By scaling h we will minimise this estimated error, not
             the actual truncation error in r5. */

          *result = r5 / h;
          *abserr_trunc = fabs ((r5 - r3) / h); /* Estimated truncation error O(h^2) */
          *abserr_round = fabs (e5 / h) + dy;   /* Rounding error (cancellations) */
          cout<<"\nCentralDeriv: abserr_trunc = "<< fabs ((r5 - r3) / h)<<" abserr_round = "<<(fabs (e5 / h) + dy)<<" Dx = "<<(r5 / h)<<endl;
          cout<<"CentralDeriv END\n";
}

void SpiceExtr::bounds_transform (){//, double *a, double *b){
        //функция преобразования границ переменных для приведения их всех
        // к одним размерам. Используемая формула (с-ма ур-й)
        //{Amin+b=-1
        //{Amax+b=1
        //solution:
        //a=2/(max-min)
        //b=1-max*a
        //try mutliply it all 100 times

        int s=getParamSize();
        a = new double[s];
        b = new double [s];

        for(int i=0;i<s;i++){
                a[i]=2/(getUpper(i)-getLower(i));
                b[i]=1-getUpper(i)*a[i];
                cout<<"getUpper(i) ="<<getUpper(i)<<" getLower(i) = "<<getLower(i)<<" a[i] = "<<a[i]<<" b[i] = "<<b[i]<<endl;
        }
}

void SpiceExtr::get_trans_val (double* x, double* res){

        int s=getParamSize();
        cout<<"s = "<<s<<endl;
        for(int i=0;i<s;i++){
                res[i]=a[i]*x[i]+b[i];
                cout<<"a[i] = "<<a[i]<<" x(i) = "<<x[i]<<" b[i] = "<<b[i]<<endl;
        }
}

void SpiceExtr::get_real_val (double *x_conv, double* res){

        int s=getParamSize();
        for(int i=0;i<s;i++){
                res[i]=(x_conv[i]-b[i])/a[i];
                //cout<<" get_real_val a[i] = "<<a[i]<<" x(i) = "<<x_conv[i]<<" b[i] = "<<b[i]<<" res[i] "<<res[i]<<endl;
        }
}

void SpiceExtr::OptppFeval_conv (int dim, double* x, double& fx, int& result){

    double x_conv[dim];
        get_real_val (x, x_conv);
        OptppFeval (dim,x_conv, fx, result);

}


void SpiceExtr::MultiRun(int dim, double* x, double& fx, int& result){
    double tmp[dim];
    for(int i=0;i<10;i++){
        for(int k=0;k<dim;k++){
            tmp[k]=x[k]*0.01*i;
        }
        OptppFeval (dim,tmp, fx, result);
    }
}

void SpiceExtr::kill(){
    stop=true;
}

std::vector<std::string> SpiceExtr::get_outputs(std::string & cir){
    std::vector<std::string> res;

    boost::weak_ptr<NGSpiceWrapper> NGWrapper_w(NGWrapper_);

    if(boost::shared_ptr<NGSpiceWrapper> ngw = NGWrapper_w.lock()){
        ngw->load_cir(cir);
    }

    return res;
}

//NGSpiceWrapper_Exception/////////////////////////////////
const char* SpiceExtr_Exception::what() const throw() { return s_.c_str(); }
SpiceExtr_Exception::SpiceExtr_Exception(std::string s) : s_(s) {}
SpiceExtr_Exception::~SpiceExtr_Exception()  throw() {}
/////////////////////////////////NGSpiceWrapper_Exception
