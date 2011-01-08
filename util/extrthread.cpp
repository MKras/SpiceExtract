#include "extrthread.h"
#include <QDebug>
#include <QProcess>
//#include <QtConcurrent>
#include <qtconcurrentrun.h>
//#include "gui/texteditor.h"
//#include "editorthread.h"

using NEWMAT::ColumnVector;
using namespace OPTPP;


//ExtrThread::ExtrThread(curve *c, string p, QPushButton *b): QRunnable()
ExtrThread::ExtrThread(curve &c): QThread()
{
    Null=false;
    curvect.append(c);
    
}
ExtrThread::ExtrThread(QVector <curve> &c): QThread()
{
    Null=false;
    for(int i=0;i<c.size();i++){
        curve cur=c.at(i);
        curvect.append(cur);
    }
}

ExtrThread::ExtrThread(): QThread()
{
}

ExtrThread::~ExtrThread()
{
    Null=true;
}

void ExtrThread::run(){
    cansel=false;
    go();
    exec();

}

void ExtrThread::kill(){

    cansel=true;
        spe->kill();
            emit finished();
            emit closing();
            Null=true;
}

void ExtrThread::kill_it(){
    kill();
}


void ExtrThread::go(){
     emit started();
     /*for(int i=0;i<curvect.size();i++){
         for(int k=0;k<=i;k++){
            if(cansel) break;
            eval(k);
         }
         if(cansel) break;
     }*/

     Params params;
     params.prevTask = -1;

     for(int i=0;i<curvect.size();i++)
     {
         if(cansel) break;
         params.curTask = i;
         eval(i);
         params.prevTask = i;
         eval_loop(0,params);
     }
     emit finished();
}

void ExtrThread::AddLine(QString line){
    emit SetLine(line);
}

bool ExtrThread::isNull(){
    return Null;
}

bool ExtrThread::eval_loop(int i, Params &params){

    if(i>params.curTask)
        return false;

    for(int j=0;j<=i;j++)
    {
        if(params.prevTask!=j)
        {
            params.prevTask = j;
            eval(j);
        }
    }
    eval_loop(i+1,params);
}

bool ExtrThread::eval(int i){

    //if(cansel) break;
    spe= new SpiceExtr();
    //connect(spe, SIGNAL(resF(QString)), this, SLOT(AddLine(QString)),Qt::QueuedConnection);
    //connect(spe, SIGNAL(resF(QString)), this, SLOT(AddLine(QString)),Qt::AutoConnection);
    connect(spe, SIGNAL(resF(QString)), this, SLOT(AddLine(QString)));
    //if(cansel) break;
    curve cur=curvect.at(i);
    emit SetLine("");
    emit SetLine(cur.idx.data().toString());
    emit SetLine("");
    core(&cur, spe);
    //if(cansel) break;

    QString res="";
    for(int t=0;t<cur.spiceParam.size();t++){
        res=res+QString::fromStdString(cur.spiceParam.at(t))+"_x("+QString("%1").arg(t)+") = "
            +QString::fromStdString(cur.spiceInit.at(t))
            +" ";
    }
    res=res+QString("Fx = %1").arg(cur.result);
    emit SetLine("");
    emit SetLine("Current Res: "+res);
    emit SetLine("");
    emit SetLine("");

    spe->deleteLater();
    curvect.replace(i,cur);
    if(!cansel) emit stopped(cur);

    return true;
}

