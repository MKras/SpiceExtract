#include "curveselection.h"
#include "ui_curveselection.h"
#include "util/extrthread.h"
#include "util/editorthread.h"
#include "gui/texteditor.h"
#include <QDebug>

CurveSelection::CurveSelection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurveSelection)
{
    ui->setupUi(this);
    run=false;
    connect(this, SIGNAL(SetTitle(QString)), this, SLOT(setWindowTitle(QString)));
    emit SetTitle("Selection");
    disconnect(this, SIGNAL(SetTitle(QString)), this, SLOT(setWindowTitle(QString)));
}

CurveSelection::~CurveSelection()
{
    delete ui;
}

void CurveSelection::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CurveSelection::Selected(curve cur){
    if(!run){
        ui->listWidget->addItem(cur.idx.data().toString());
        curvect.append(cur);
    }
}

void CurveSelection::DeleteItem(){    
    curvect.remove(ui->listWidget->currentRow());
    ui->listWidget->takeItem(ui->listWidget->currentRow());
    for(int i=0;i<curvect.size();i++) qDebug()<<curvect.at(i).idx.data().toString();
}

void CurveSelection::OK(){
    run=true;
    if(curvect.size()!=0){

        extr = new ExtrThread(curvect);
        qRegisterMetaType<curve>("curve");
        connect(extr, SIGNAL(stopped(curve)), this, SLOT(finished(curve)));
        connect(extr, SIGNAL(SetLine(QString)),this, SLOT(GetLine(QString)));
        connect(extr, SIGNAL(finished()),this, SLOT(finished()));        

        /*edithr = new EditorThread();
        connect(this, SIGNAL(SetLine(QString)),edithr, SLOT(AddLine(QString)),Qt::QueuedConnection);

        edithr->start();*/

        TextEditor *edtr = new TextEditor();
        connect(extr, SIGNAL(SetLine(QString)),edtr, SLOT(AddLine(QString)),Qt::QueuedConnection);
        connect(this, SIGNAL(SetTitle(QString)), edtr, SLOT(setWindowTitle(QString)));

        emit SetTitle("Selection Run");
        //edtr->resize( 700, 800 );
        edtr->show();

        extr->start();
    }
}

void CurveSelection::GetLine(QString s){
    emit SetLine(s);
}

void CurveSelection::finished(curve c){
    qDebug()<<"CurveSelection::finished(curve c)";
    emit SetCurve(c);
}

void CurveSelection::finished(){
    run=false;
}

void CurveSelection::stop(){
    run=false;
    extr->kill();
}

