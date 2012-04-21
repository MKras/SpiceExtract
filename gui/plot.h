#ifndef PLOT_H
#define PLOT_H

#include <QFile>
#include <QFont>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <qapplication.h>
#include <math.h>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_plot_panner.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_scale_engine.h>

#define MAX_GRAPHICS 32101

class Plot : public QwtPlot
{
    Q_OBJECT
public:
    Plot(QVector<QString> out, QVector<QString> exp,
         QString XAxisName, QString YAxisName);

    void addPlot(QVector<QPointF> *dataExp,  QVector<QPointF> *dataSim,
                 bool renderAntialiased = false,
                 QString captionExp = "",  QString captionSim = "",
                 QColor colorExp = Qt::blue, QColor colorSim = Qt::red);
    bool openOutFile(QString *fileName, QVector<QPointF> *dataForWrite);
    void erase();
    void show(QWidget *parentWindow);

private:
    bool setPlot(int i)
    {
        if(i < MAX_GRAPHICS)return true;
        else return false;
    }

    int curve_no;
    QVector<QPointF> dataOut,dataExp;
    QVector<QString> out;
    QVector<QString> exp;

    QwtPlotPanner *panner;
    QwtPlotZoomer *plotZoomer;

    QwtPlotCurve *curveExp[MAX_GRAPHICS];
    QwtPlotCurve *curveSim[MAX_GRAPHICS];
    QPen penExp, penSim;
    QFont fontAxis,fontAxisText;
    QAction *action_changeFontAxis;
    QAction *action_changeFontAxisText;
    QAction *action_hideShowAxisText;
    QAction *action_changeAxisXText;
    QAction *action_changeAxisYText;
    QAction *action_YLogScale;
    QAction *action_XLogScale;
    QString XAxisName,YAxisName;

    bool ylog;
    bool xlog;

    void mousePressEvent(QMouseEvent *);    
    void createActions(QWidget *parent);
    bool saveFont(QFont *font,QFont *font2);
    bool loadFont(QFont *font,QFont *font2);
    void changeTextAxis(QString newXAxisName="", QString newYAxisName="");

private slots:
        void reDrow();

public slots:
    void changeFontAxisSlot();
    void changeFontAxisTextSlot();
    void hideShowAxisTextSlot();
    void showDialogChangeAxisXTextSlot();
    void showDialogChangeAxisYTextSlot();
    void changeYScaleSlot();
    void changeXScaleSlot();


};

#endif // PLOT_H






























