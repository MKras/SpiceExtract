#ifndef CURVESELECTION_H
#define CURVESELECTION_H

#include <QWidget>
#include<QModelIndex>
#include "util/globals.h"
#include "util/extrthread.h"
#include "util/editorthread.h"

namespace Ui {
    class CurveSelection;
}

class CurveSelection : public QWidget {
    Q_OBJECT
public:
    CurveSelection(QWidget *parent = 0);
    ~CurveSelection();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CurveSelection *ui;
    QVector<curve> curvect;
    ExtrThread *extr;
    EditorThread *edithr;
    bool run;

private slots:
    void Selected(curve);
    void DeleteItem();
    void OK();
    void GetLine(QString);
    void finished(curve);
    void finished();
    void stop();

signals:
    void res(QVector<curve>);
    void SetLine(QString);
    void SetCurve(curve);
    void SetTitle(QString);
};

#endif // CURVESELECTION_H
