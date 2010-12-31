#ifndef OPTPARAMSEDITOR_H
#define OPTPARAMSEDITOR_H

#include <QWidget>
#include <QDebug>
#include "../util/globals.h"

namespace Ui {
    class OptParamsEditor;
}

class OptParamsEditor : public QWidget
{
    Q_OBJECT

public:
    //explicit OptParamsEditor(QWidget *parent = 0);
    OptParamsEditor(QWidget *parent = 0);
    OptParamsEditor(QWidget *parent = 0, curve *cur = 0);
    ~OptParamsEditor();

private:
    Ui::OptParamsEditor *ui;
    void ChangeCurveParams();
    curve *cur;

private slots:
    void Edit();
    void Cansel();

signals:
    void ChangeCurveParamsSignal(curve);
    void SetTitle(QString);
};

#endif // OPTPARAMSEDITOR_H
