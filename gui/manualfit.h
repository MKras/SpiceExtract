#ifndef MANUALFIT_H
#define MANUALFIT_H

#include "util/globals.h"
#include <QWidget>
#include <QHBoxLayout>
#include <qwt_slider.h>
#include <QLabel>


#include "QScienceSpinBox.h"
#include "util/extrthread.h"

/*namespace Ui {
    class ManualFit;
}*/

class ManualFit : public QWidget {
    Q_OBJECT
public:
    ManualFit(QWidget *parent = 0);
    ManualFit(curve &c, QWidget *parent = 0);
    ~ManualFit();

signals:
    void redrow();
    void res(curve);

protected:
    //void changeEvent(QEvent *e);

private:
    //Ui::ManualFit *ui;
    QHBoxLayout* createOne(curve *cur, int i);
    QVBoxLayout *mainVBL;
    QString old;


    QVector <QHBoxLayout*> Layout;
    QVector <QLabel*> Label;
    QVector <QwtSlider*> Slider;
    QVector <QScienceSpinBox*> SpinBox;
    ExtrThread *extr;
    QVector <curve> curvect;
    curve cur;



private slots:
    void plot();
    void finished();
    void ok();
};

#endif // MANUALFIT_H

