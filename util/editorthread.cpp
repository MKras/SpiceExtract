#include "editorthread.h"
#include "gui/texteditor.h"
#include <QDebug>
#include "util/extrthread.h"

EditorThread::EditorThread(): QThread()
{
    ed = new TextEditor();
    ed->resize( 700, 800 );
    //connect(ed, SIGNAL(closing()), this, SLOT(toclose()),Qt::QueuedConnection);
    connect(ed, SIGNAL(closing()), this, SLOT(toclose()));
}

/*EditorThread::EditorThread(curve &c): QThread()
{
    ed = new TextEditor();
    //ed->resize( 700, 800 );
    cur=c;
    //connect(ed, SIGNAL(closing()), this, SLOT(toclose()),Qt::QueuedConnection);
    connect(ed, SIGNAL(closing()), this, SLOT(toclose()));
    //ed->show();
}*/



void EditorThread::run(){

    /*ed = new TextEditor();
    ed->resize( 700, 800 );
    //cur=c;
    //connect(ed, SIGNAL(closing()), this, SLOT(toclose()),Qt::QueuedConnection);
    connect(ed, SIGNAL(closing()), this, SLOT(toclose()));*/
    ed->show();

    /*extr = new ExtrThread(cur, "opt.conf");

    qRegisterMetaType<curve>("curve");
    connect(extr, SIGNAL(started()), this, SLOT(processStarted()),Qt::QueuedConnection);
    connect(extr, SIGNAL(stopped(curve)), this, SLOT(changeCurve(curve)),Qt::QueuedConnection);
    //connect(extr, SIGNAL(stopped(curve)), this, SLOT(processStoped()));
    connect(extr, SIGNAL(finished()), this, SLOT(processStoped()),Qt::QueuedConnection);
    connect(extr, SIGNAL(SetLine(QString)),this, SLOT(AddLine(QString)),Qt::QueuedConnection);
    connect(this, SIGNAL(extr_kill()),extr, SLOT(kill_it()),Qt::QueuedConnection);

    extr->start();*/
    //exec();
}

/*void EditorThread::processStarted(){
    emit started();
}*/

/*void EditorThread::changeCurve(curve c){
    emit stopped(c);
}*/

/*void EditorThread::processStoped(){
    emit finished();
}*/

void EditorThread::AddLine(QString line){
    ed->addLine(line);
}

/*void EditorThread::kill(){
    ed->~TextEditor();
}*/

/*void EditorThread::toclose(){
    //qDebug()<<"EditorThread::toclose()\n";
};*/
/*void EditorThread::kill_extr(){
    emit extr_kill();
}*/
