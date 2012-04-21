#include "plot.h"
#include <cmath>
#include <QtGui>


Plot::Plot(QVector<QString> out, QVector<QString> exp,
           QString XAxisName, QString YAxisName)
{
    curve_no = -1;
    this->out=out;
    this->exp=exp;

    penExp.setWidth(5);
    penExp.setCapStyle(Qt::RoundCap);
    //penSim = default QPen

    setMinimumSize(QSize(640,480));
    setPalette(QPalette(Qt::white));
    setCursor(QCursor(Qt::CrossCursor));

    //insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    panner = new QwtPlotPanner(canvas());
    panner->setMouseButton(Qt::MidButton);

    plotZoomer = new QwtPlotZoomer(canvas());
    plotZoomer->setMousePattern(QwtEventPattern::MouseSelect1,Qt::LeftButton);
    plotZoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::NoButton);
    plotZoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::NoButton);
    plotZoomer->setMousePattern(QwtEventPattern::MouseSelect4,Qt::NoButton);
    plotZoomer->setMousePattern(QwtEventPattern::MouseSelect5,Qt::NoButton);
    plotZoomer->setMousePattern(QwtEventPattern::MouseSelect6,Qt::NoButton);

    Plot::XAxisName = XAxisName;
    Plot::YAxisName = YAxisName;

    if(loadFont(&fontAxis,&fontAxisText))    {
        setAxisFont(xBottom, fontAxis);
        setAxisFont(yLeft, fontAxis);
        QwtText XX,YY;
        XX.setFont(fontAxisText);
        YY.setFont(fontAxisText);
        XX.setText(XAxisName);
        YY.setText(YAxisName);
        setAxisTitle(xBottom, XX);
        setAxisTitle(yLeft,   YY);
    }

    //setAxisTitle(xBottom, XX);
    //setAxisTitle(yLeft,   YY);

    //QVector<QPointF> dataOut,dataExp;
    xlog=false;
    ylog=false;

    reDrow();    
}

void Plot::addPlot(QVector<QPointF> *dataExp, QVector<QPointF> *dataSim,
                   bool renderAntialiased,
                   QString captionExp,        QString captionSim,
                   QColor colorExp,           QColor colorSim )
{
    QVector <double> *xExp = new QVector <double>;
    QVector <double> *yExp = new QVector <double>;

    QVector <double> *xSim = new QVector <double>;
    QVector <double> *ySim = new QVector <double>;

    for(int i=0; i< dataExp->count(); i++)
    {
        xExp->append(dataExp->at(i).x());
        yExp->append(dataExp->at(i).y());
    }

    for(int i=0; i< dataSim->count(); i++)
    {
        xSim->append(dataSim->at(i).x());
        ySim->append(dataSim->at(i).y());
    }

    if( setPlot(++curve_no)) {

        //set EXPERIMENTAL points
        penExp.setColor(colorExp);
        curveExp[curve_no] = new QwtPlotCurve(captionExp);
        if(renderAntialiased)
        curveExp[curve_no]->setRenderHint(QwtPlotItem::RenderAntialiased);
        curveExp[curve_no]->setPen(penExp);
        curveExp[curve_no]->setStyle(QwtPlotCurve::Dots);
        curveExp[curve_no]->setData( *xExp , *yExp );
        curveExp[curve_no]->attach(this);

        //set SIMULATED line
        penSim.setColor(colorSim);
        curveSim[curve_no] = new QwtPlotCurve(captionSim);
        if(renderAntialiased)
        curveSim[curve_no]->setRenderHint(QwtPlotItem::RenderAntialiased);
        curveSim[curve_no]->setPen(penSim);
        curveSim[curve_no]->setStyle(QwtPlotCurve::Lines);
        curveSim[curve_no]->setData( *xSim , *ySim );
        curveSim[curve_no]->attach(this);

        plotZoomer->setZoomBase(true);

    }else
        QMessageBox::warning(this,"Warning",
           "Not enough space to build graphic n."+
            QString::number(curve_no),
            QMessageBox::Ok);
}

void Plot::erase(){

    curve_no = -1;
    if( setPlot(++curve_no)) {
        //delete curveSim[curve_no];
        curveSim[curve_no] = NULL;
    }
     clear();
}

void Plot::reDrow(){
    erase();
    int i=0;
    while( exp.count() != i || out.count() != i )
    {
        QString outS = out[i];
        QString expS = exp[i];
        bool Outd = openOutFile( &outS , &dataOut );
        bool Expd = openOutFile( &expS , &dataExp );
        //addPlot( &dataExp, &dataOut, true );
        //на случай, если модеруется вложенный анализ
        // типа .DC VD 0 2.5 0.05 VG 0.5 1.51 0.2
        // в таком случае в одном файле хранится набор точек для нескольких кривых
        //т.е. значения по Х повторяются периодически.
        // для того, чтобы избежать соединения последних точек предыдущей и
        //последующей кривой вносим такие вот изменения
        QVector<QPointF> Exp;
        QVector<QPointF> Sim;
        //if ((dataOut.size()>0)||dataExp.size()>0){
        //if ((openOutFile( &outS , &dataOut )&&(openOutFile( &expS , &dataExp )))){
        //на случай равномерного шага
        //double x1 = dataExp.at(0).x();
        //double x2 = dataExp.at(1).x();
        //double delta_init=std::fabs(x2-x1);
        //qDebug()<<"DELTA1 = "<<delta_init;
        //delta_init = sqrt((delta_init)*(delta_init));

        //qDebug()<<"delta_init = "<<delta_init<<" = "<<"x2-x1 = "<<x2<<" - "<<x1;
        int k=0;
        for(k=0;k<dataExp.size()-1;k++){
            //смотрим, закончится ли набор точек для текущей кривой
            double x1 = dataExp.at(k).x();
            double x2 = dataExp.at(k+1).x();
            if(((std::fabs(x1))<(std::fabs(x2)))
                ||(((std::fabs(x1))>(std::fabs(x2)))&&((x1<=0)&&(x2<=0)))
                ){
                //qDebug()<<"TRUE delta = "<<std::fabs(x2-x1)<<" = "<<"x2-x1 = "<<x2<<" - "<<x1;
                if(Expd) Exp.append(dataExp.at(k));
                if(Outd) Sim.append(dataOut.at(k));
                if(k==dataExp.size()-2){
                    if(Expd) Exp.append(dataExp.at(k+1));
                    if(Outd) Sim.append(dataOut.at(k+1));
                    addPlot( &Exp, &Sim, true );
                }
            }else{
                if(Expd) Exp.append(dataExp.at(k));
                if(Expd) Sim.append(dataOut.at(k));
                addPlot( &Exp, &Sim, true );

                Exp.clear();
                Sim.clear();
            }
        }
        i++;
    }
}

void Plot::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        //plotZoomer = new QwtPlotZoomer(canvas());
        plotZoomer->zoom(0);
    }
}

bool Plot :: openOutFile(QString *fileName, QVector<QPointF> *dataForWrite)
{
    dataForWrite->clear();
    QFile file;
    file.setFileName(*fileName);
    if(file.open(QFile::ReadOnly))
    {

        //QByteArray all = file.readAll();
        //all.append("/n");
        QByteArray line;

        QString x,y;
        while( ! (line = file.readLine()).isNull() ) {
            x.clear();
            y.clear();
            //line = file.readLine();
            for(int i=0; i<line.count(); i++) {
                if(line.at(i) != ' ')
                    x.append(line.at(i));
                else {
                    for(int j=i+1; j<line.count(); j++)
                        y.append(line.at(j));
                    break;
                }
            }
            dataForWrite->append(QPointF(x.toDouble(),y.toDouble()));
        }

        if (dataForWrite->size()==0)  return false;
        file.close();
        return true;
    }
    else return false;
}

void Plot::createActions(QWidget *parent)
{
    action_changeFontAxis = new QAction("Change Axis Numbers Font",parent);
    //action_changeFontAxis->setIcon(QIcon("img/Untitled-1 copy.png"));
    connect(action_changeFontAxis, SIGNAL(triggered()), this, SLOT(changeFontAxisSlot()));

    action_changeFontAxisText = new QAction("Change Axis Title Font",parent);
    connect(action_changeFontAxisText, SIGNAL(triggered()),this,SLOT(changeFontAxisTextSlot()));

    action_hideShowAxisText = new QAction("Hide/Show Axis Text",parent);
    //action_hideShowAxisText->setCheckable(true);
    //action_hideShowAxisText->setChecked(true);
    connect(action_hideShowAxisText, SIGNAL(triggered()),this,SLOT(hideShowAxisTextSlot()));

    action_changeAxisXText = new QAction("Change Axis X Text",parent);
    connect(action_changeAxisXText,SIGNAL(triggered()),this,SLOT(showDialogChangeAxisXTextSlot()));

    action_changeAxisYText = new QAction("Change Axis Y Text",parent);
    connect(action_changeAxisYText,SIGNAL(triggered()),this,SLOT(showDialogChangeAxisYTextSlot()));

    action_YLogScale = new QAction("Y Log10",parent);
    connect(action_YLogScale,SIGNAL(triggered()),this,SLOT(changeYScaleSlot()));

    action_XLogScale = new QAction("X Log10",parent);
    connect(action_XLogScale,SIGNAL(triggered()),this,SLOT(changeXScaleSlot()));

}

void Plot::changeFontAxisSlot()
{
    bool ok;
    QFont font = QFontDialog::getFont( &ok, fontAxis, this);
    if (ok) {
        setAxisFont(xBottom, font);
        setAxisFont(yLeft, font);
        fontAxis = font;
        saveFont(&fontAxis,&fontAxisText);
        }
}

void Plot::changeFontAxisTextSlot()
{
    bool ok;
    QFont font = QFontDialog::getFont( &ok, fontAxisText, this);
    if (ok) {
        QwtText XX,YY;
        XX.setFont(font);
        YY.setFont(font);
        XX.setText(XAxisName);
        YY.setText(YAxisName);
        setAxisTitle(xBottom, XX);
        setAxisTitle(yLeft,   YY);
        fontAxisText = font;
        saveFont(&fontAxis,&fontAxisText);
    }
}

void Plot::show(QWidget *parentWindow)
{
    QMainWindow *w = new QMainWindow(parentWindow);
    createActions(w);
    w->setCentralWidget(this);

    QMenuBar *menubar = new QMenuBar(w);

    QMenu *menuTools = new QMenu("Tools",menubar);
    QMenu *menuFile = new QMenu("File",menubar);
    QMenu *menuFont = new QMenu("Font",menuTools);
    QMenu *menuScale = new QMenu("Scales",menuTools);
    QMenu *menuChangeAxisText = new QMenu("Change axis text");
    menubar->addMenu(menuFile);
    menubar->addMenu(menuTools);
    menuTools->addMenu(menuFont);
    menuTools->addMenu(menuScale);

    menuFont->addAction(action_changeFontAxis);
    menuFont->addAction(action_changeFontAxisText);

    menuTools->addMenu(menuChangeAxisText);
    menuChangeAxisText->addAction(action_changeAxisXText);
    menuChangeAxisText->addAction(action_changeAxisYText);

    menuTools->addAction(action_hideShowAxisText);

    menuScale->addAction(action_YLogScale);
    menuScale->addAction(action_XLogScale);

    w->setMenuBar(menubar);

    w->resize(800,600);
    w->show();
}

bool Plot::saveFont(QFont *font,QFont *font2)
{
    QFile file;
    file.setFileName("Font.font");
    if(file.open(QFile::WriteOnly)) {
        QByteArray ByteArray;
        ByteArray.append(font->toString());
        ByteArray.append(0xd);
        ByteArray.append(0xa);
        ByteArray.append(font2->toString());
        file.write(ByteArray);
        file.close();
        return true;
    }
    else return false;
}

bool Plot::loadFont(QFont *font,QFont *font2)
{
    QFile file;
    file.setFileName("Font.font");
    if(file.open(QFile::ReadOnly))    {

        if(font->fromString(file.readLine()) &&
           font2->fromString(file.readLine())) {
            file.close();
            return true;
        }
        else return false;
    }
    else return false;
}


void Plot::hideShowAxisTextSlot()
{
    if(axisTitle(xBottom).isNull())
        changeTextAxis(XAxisName,axisTitle(yLeft).text());
    else changeTextAxis(NULL,axisTitle(yLeft).text());

    if(axisTitle(yLeft).isNull())
        changeTextAxis(axisTitle(xBottom).text(),YAxisName);
    else changeTextAxis(axisTitle(xBottom).text(),NULL);

    //changeTextAxis(XAxisName,YAxisName);


}

void Plot::showDialogChangeAxisXTextSlot()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("New naming"),
                                              tr("X axis new name"), QLineEdit::Normal,
                                              XAxisName, &ok);
    if (ok){
        XAxisName = text;
        changeTextAxis(XAxisName,YAxisName);
    }
}

void Plot::showDialogChangeAxisYTextSlot()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("New naming"),
                                              tr("Y axis new name"), QLineEdit::Normal,
                                              YAxisName, &ok);
    if (ok){
        YAxisName = text;
        changeTextAxis(XAxisName,YAxisName);
    }
}

void Plot::changeTextAxis(QString newXAxisName, QString newYAxisName)
{
    QwtText XX,YY;
    XX.setFont(fontAxisText);
    YY.setFont(fontAxisText);
    XX.setText(newXAxisName);
    YY.setText(newYAxisName);
    setAxisTitle(xBottom, XX);
    setAxisTitle(yLeft,   YY);
}

void Plot::changeYScaleSlot(){
    if(!ylog) {
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLog10ScaleEngine());
        ylog=true;
    }
    else {
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine() );
        ylog=false;
    }
    reDrow();
}

void Plot::changeXScaleSlot(){
    if(!xlog) {
        setAxisScaleEngine(QwtPlot::xBottom, new QwtLog10ScaleEngine());
        xlog=true;
    }
    else{
         setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine() );
         xlog=false;
    }
    reDrow();
}
