#ifndef EDITORTHREAD_H
#define EDITORTHREAD_H
#include <QThread>
#include "gui/texteditor.h"
#include "util/extrthread.h"
#include "util/globals.h"


class EditorThread: public QThread
{
    Q_OBJECT
public:
    EditorThread();
    EditorThread(curve &c);
    void kill_extr();
    void kill();

protected:
    void run();

private:
    TextEditor *ed;
    curve cur;
    ExtrThread *extr;
    QVector <QString> strlist;


private slots:
    void AddLine(QString);
    /*void toclose();
    void processStarted();
    void changeCurve(curve);
    void processStoped();*/

signals:
    /*void started();
    void stopped(curve c);
    void finished();
    void extr_kill();*/

};

#endif // EDITORTHREAD_H
