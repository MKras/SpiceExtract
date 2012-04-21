#include "manualfit.h"
//#include "ui_manualfit.h"
#include "util/globals.h"
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QDebug>

#include <qwt/qwt_scale_engine.h>
#include "QScienceSpinBox.h"
#include "util/extrthread.h"



ManualFit::ManualFit(QWidget *parent) :
    QWidget(parent)//,
    //ui(new Ui::ManualFit)
{
    //ui->setupUi(this);
}

ManualFit::~ManualFit()
{
    //delete ui;
}


ManualFit::ManualFit(curve &c, QWidget *parent): QWidget(parent){
    mainVBL = new QVBoxLayout(this);

    cur = c;


    for(int i=0; i<cur.spiceParam.size();i++){
        mainVBL->addLayout(createOne(&cur, i));
    };

    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *ok= new QPushButton("OK");
    QPushButton *cancel= new QPushButton("Cancel");
    buttons->addWidget(ok);
    buttons->addWidget(cancel);

    connect(ok, SIGNAL(clicked()), this, SLOT(ok()));

    mainVBL->addLayout(buttons);


    this->setLayout(mainVBL);

}
//first element start with 0 !!!
QHBoxLayout* ManualFit::createOne(curve *cur, int i){

    QHBoxLayout *layout = new QHBoxLayout();
    QLabel *label = new QLabel(QString::fromStdString(cur->spiceParam.at(i)));
    QwtSlider *slider = new QwtSlider(this,Qt::Horizontal, QwtSlider::NoScale, QwtSlider::BgTrough);
    /*QwtSlider *slider = new QwtSlider(this,Qt::Horizontal,
                                      QwtSlider::autoScale(200,
                                                           QString("%1").arg(QString::fromStdString(cur->spiceMin.at(i))).toDouble(),
                                                           QString("%1").arg(QString::fromStdString(cur->spiceMax.at(i))).toDouble(),
                                                           abs(QString("%1").arg(QString::fromStdString(cur->spiceMax.at(i))).toDouble()
                                                                                     -QString("%1").arg(QString::fromStdString(cur->spiceMin.at(i))).toDouble()))/1e-4
                                                           ), QwtSlider::BgTrough);*/





    slider->setRange(QString("%1").arg(QString::fromStdString(cur->spiceMin.at(i))).toDouble(),
                     QString("%1").arg(QString::fromStdString(cur->spiceMax.at(i))).toDouble(),
                     abs(QString("%1").arg(QString::fromStdString(cur->spiceMin.at(i))).toDouble())*1e-8,
                     100);

    slider->setScale(QString("%1").arg(QString::fromStdString(cur->spiceMin.at(i))).toDouble(),
                     QString("%1").arg(QString::fromStdString(cur->spiceMax.at(i))).toDouble(),
                     //QString("%1").arg(QString::fromStdString(cur->spiceMin.at(i))).toDouble()*1e-8
                     //TRUE
                     (abs(QString("%1").arg(QString::fromStdString(cur->spiceMax.at(i))).toDouble()
                          -QString("%1").arg(QString::fromStdString(cur->spiceMin.at(i))).toDouble()))/1e-4
                     );
    //slider->setAutoScale();

    slider->setValue( QString("%1").arg(QString::fromStdString(cur->spiceInit.at(i))).toDouble());

    //QDoubleSpinBox *spinBox = new QDoubleSpinBox;
    QScienceSpinBox *spinBox = new QScienceSpinBox;
    spinBox->setRange(QString("%1").arg(QString::fromStdString(cur->spiceMin.at(i))).toDouble(),
                      QString("%1").arg(QString::fromStdString(cur->spiceMax.at(i))).toDouble());
    //spinBox->setValue(abs(QString("%1").arg(QString::fromStdString(cur->spiceInit.at(i))).toDouble()));
    spinBox->setValue(QString("%1").arg(QString::fromStdString(cur->spiceInit.at(i))).toDouble());


    QObject::connect(spinBox, SIGNAL(valueChanged(double)),
                    slider, SLOT( setValue(double)));
    QObject::connect(slider, SIGNAL(valueChanged(double)),
                     spinBox, SLOT(setValue(double)));

    QObject::connect(slider, SIGNAL(sliderReleased()),
                     this, SLOT(plot()));




    layout->addWidget(label);
    layout->addWidget(spinBox);
    layout->addWidget(slider);

    SpinBox.append(spinBox);
    Layout.append(layout);
    Label.append(label);
    Slider.append(slider);

    return layout;
}


void ManualFit::plot(){

    old = QString::fromStdString(cur.AlgOpt);

    qDebug()<<"VALUE:: \n";
    for(int i=0;i<cur.spiceInit.size();i++){
        cur.spiceInit.at(i)=QString("%1").arg(SpinBox.at(i)->value()).toStdString();
    }


    cur.AlgOpt="Evaluate";

    extr = new ExtrThread(cur);
    qRegisterMetaType<curve>("curve");

    connect(extr, SIGNAL(stopped(curve)), this, SLOT(finished()));

    extr->start();    
    cur.AlgOpt=QString(old).toStdString();
}

void ManualFit::finished(){
    emit redrow();
}

void ManualFit::ok(){
    emit res(cur);
}


