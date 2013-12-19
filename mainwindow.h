#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardItemModel>


#include "util/dutconfig.h"
#include "util/extrthread.h"
#include "util/globals.h"
#include "util/editorthread.h"
#include "core/core.h"
#include "core/spiceextr.h"
#include "gui/plot.h"
#include "gui/manualfit.h"
#include "gui/curveselection.h"
#include "gui/about.h"
#include "gui/optparamseditor.h"
#include <QKeyEvent>




namespace Ui {
    class MainWindow;
}

//TODO: make *_Exception class as template
class MainWindow_Exception : public std::exception
{
private:
   std::string s_;
public:
   const char* what() const throw();
   MainWindow_Exception(std::string s);
   ~MainWindow_Exception()  throw();
};


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool okToContinue();


protected:
    void changeEvent(QEvent *e);
    void parseFile(const QString &fileName);
    void loadFile(QString fileName);

private:
    bool isDevice(QModelIndex idx);
    bool isCurve(QModelIndex idx);
    bool isDUT(QModelIndex idx);
    void fillTables(curve res);
    void InsertDevice(QModelIndex idx);
    bool reBbuildTree();
    curve fillCurve();
    void run(curve cur);
    void plot(curve cur);
    void keyPressEvent(QKeyEvent *event);
    QString spiceValueFromModelFile(QString valName);
    bool eventFilter( QObject *obj, QEvent *evt );
    //void mouseMoveEvent(QMouseEvent* event);
    void construct();


    Ui::MainWindow *ui;
    //QTreeWidgetItem *treeWidget;
    QStandardItemModel *treeModel;
    QStandardItem *item;
    //TreeModel *treeModel;
    DUTconfig *DUT;
    QMenu *treeMenu;
    QString configfileName;
    QString currentModelPath;
    ExtrThread *extr;
    EditorThread *edithr;
    ManualFit *mf;    
    CurveSelection *cursel;
    //Actions
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionEditor;
    QAction *actionConfilg;
    QAction *actionInsert_curve;
    QAction *actionNew_device;
    QAction *actionNew_curve;
    QAction *actionRemove_Curve;
    QAction *actionRemove_device;
    QAction *action_Browse;
    QAction *actionSave_as;
    QAction *action_Change;
    QAction *action_Roll_back;
    QAction *action_Plot;
    QAction *actionExtractAll;
    QAction *actionManualFit;
    QAction *actionSelect;
    //

private slots:
      void open();
      bool save();      
      bool save_as();
      void treeItemSelected(QModelIndex idx);
      void fileButtonClicked();
      void modelButtonClicked();
      void showPopup(const QPoint & iPoint);
      void showFileTablePopup(const QPoint & iPoint);
      void newDev();
      bool remDev();
      void InsertCurve();
      bool remCurve();
      void filesTableClicked();
      void fileButtonRClicked();
      void modelButtonRClicked();
      void rebuildComboBox(QString file);
      void browseFile();
      void calculate();
      void changeCurve();
      void rollBack();
      void plot();
      void plot_single();
      void ConfigEdit();
      void Editor();
      void changeCurve(curve cur);
      void processStarted();
      void processStoped();
      void ExtractAll();
      void manualFit();
      void CurveSelect();
      void GetParamValue(QString);
      void AddLine(QString);
      void EditFile();
      void about();
      void OptParamsEdit();
      void changeOptParams(curve cur);
      void reload_cir_outputs();



signals:
      //void popupActivated(QModelIndex idx);
      void stopCalculation();
      void CurveSignal(curve);
      void SetLine(QString);
      void SetTitle(QString);




};

#endif // MAINWINDOW_H
