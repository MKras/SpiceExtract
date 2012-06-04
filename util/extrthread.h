#ifndef EXTRTHREAD_H
#define EXTRTHREAD_H
#include <qthread.h>
#include <QPushButton>
#include <QRunnable>
#include <QThread>
#include <QMutex>
#include "globals.h"
#include "core/core.h"
#include "core/spiceextr.h"
//#include "editorthread.h"

//using NEWMAT::ColumnVector;
//using namespace OPTPP;

class ExtrThread: public QThread
//class ExtrThread: public QRunnable
{
    Q_OBJECT
public:
    ExtrThread(curve &c);
    ExtrThread(curve &c, QString configfileName);
    ExtrThread(QVector <curve> &c);
    ExtrThread();
    ~ExtrThread();

    void kill();
    void go();
    bool isNull();

protected:
    void run();
private:
    typedef struct Params
    {
        int prevTask;
        int curTask;
    };

    QString configfileName_;
    bool execute();
    bool endOfStream;
    //curve cur;
    bool isRuning;
    SpiceExtr *spe;
    bool yesToKill;
    bool cansel;
    QVector <curve> curvect;
    //EditorThread *editor;
    //void calculate(curve cur);
    bool Null;
    bool eval(int i);
    //bool eval_loop(int i);
    bool eval_loop(int i, Params &params);

signals:
    void started();
    void stopped(curve);
    void finished();
    void gostart();
    void gokill();
    void SetLine(QString);
    void closing();


    //обязятельно надо прописать qmake
    /*after adding classes that inherit QObject and have Q_OBJECT functionalities (like signals and slots)
    you have to manually launch "qmake" (without any parameter), because it has to regenerate the makefile
    adding the calls to MOC files for the new classes.*/

private slots:
    //void MEdStarted();
    //void MEdStoped();
    void AddLine(QString);
    void kill_it();

};

#endif // EXTRTHREAD_H
