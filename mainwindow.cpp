#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QStandardItemModel>
#include "util/duttreemodel.h"
#include "util/extrthread.h"
#include "gui/texteditor.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <QInputDialog>
#include "util/editorthread.h"
#include <qfile.h>
#include <qtextstream.h>
#include "gui/manualfit.h"
#include "gui/curveselection.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextStream out(stdout);

    ui->setupUi(this);

    construct();

    treeModel = new QStandardItemModel(0,1);
    ui->treeView->setModel(treeModel);    
    ui->treeView->setHeaderHidden(true);

     ui->filestableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Cir file"));
     ui->filestableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Output file"));
     ui->filestableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Experiment file"));

     ui->modelparamtableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
     ui->modelparamtableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Min val."));
     ui->modelparamtableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Init val."));
     ui->modelparamtableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Max val."));

     ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

     currentModelPath="";

     DUT = new DUTconfig();
     item = new QStandardItem("DUT");

     //равноперное растяжение столбцов таблицы
     ui->filestableWidget->horizontalHeader()->setResizeMode( QHeaderView::Stretch);
     ui->modelparamtableWidget->horizontalHeader()->setResizeMode( QHeaderView::Stretch);
     //добавляем фильтор для QTableWidget
     ui->filestableWidget->installEventFilter( this );

     ui->filestableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
     QObject::connect(ui->filestableWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
                      this, SLOT(showFileTablePopup(const QPoint &)));

     ui->modelparamtableWidget->installEventFilter( this );
     ui->Outparse_cb_x->installEventFilter(this);
     ui->Outparse_cb_y->installEventFilter(this);
     ui->ErrorFunction->installEventFilter(this);

     reBbuildTree();
}
void MainWindow::construct(){

    actionInsert_curve = new QAction(this);
    actionInsert_curve->setObjectName(QString::fromUtf8("actionInsert_curve"));
    actionInsert_curve->setText(QApplication::translate("MainWindow", "Insert Curve", 0, QApplication::UnicodeUTF8));

    actionNew_device = new QAction(this);
    actionNew_device->setObjectName(QString::fromUtf8("actionNew_device"));
    actionNew_device->setText(QApplication::translate("MainWindow", "New Device", 0, QApplication::UnicodeUTF8));

    actionNew_curve = new QAction(this);
    actionNew_curve->setObjectName(QString::fromUtf8("actionNew_curve"));
    actionNew_curve->setText(QApplication::translate("MainWindow", "New Curve", 0, QApplication::UnicodeUTF8));

    actionRemove_Curve = new QAction(this);
    actionRemove_Curve->setObjectName(QString::fromUtf8("actionRemove_Curve"));
    actionRemove_Curve->setText(QApplication::translate("MainWindow", "Remove Curve", 0, QApplication::UnicodeUTF8));

    actionRemove_device = new QAction(this);
    actionRemove_device->setObjectName(QString::fromUtf8("actionRemove_device"));
    actionRemove_device->setText(QApplication::translate("MainWindow", "Remove Device", 0, QApplication::UnicodeUTF8));

    action_Browse = new QAction(this);
    action_Browse->setObjectName(QString::fromUtf8("action_Browse"));
    action_Browse->setText(QApplication::translate("MainWindow", "Browse", 0, QApplication::UnicodeUTF8));
    action_Browse->setShortcut(QApplication::translate("MainWindow", "Ctrl+K", 0, QApplication::UnicodeUTF8));

    action_Change = new QAction(this);
    action_Change->setObjectName(QString::fromUtf8("action_Change"));
    action_Change->setText(QApplication::translate("MainWindow", "Change", 0, QApplication::UnicodeUTF8));

    action_Roll_back = new QAction(this);
    action_Roll_back->setObjectName(QString::fromUtf8("action_Roll_back"));

    actionExtractAll = new QAction(this);
    actionExtractAll->setObjectName(QString::fromUtf8("actionExtractAll"));

    actionSelect = new QAction(this);
    actionSelect->setObjectName(QString::fromUtf8("actionSelect"));

    QObject::connect(actionNew_device, SIGNAL(activated()), this, SLOT(newDev()));
    QObject::connect(actionRemove_device, SIGNAL(activated()), this, SLOT(remDev()));
    QObject::connect(actionInsert_curve, SIGNAL(activated()), this, SLOT(InsertCurve()));
    QObject::connect(actionRemove_Curve, SIGNAL(activated()), this, SLOT(remCurve()));
    QObject::connect(action_Browse, SIGNAL(activated()), this, SLOT(browseFile()));
    QObject::connect(action_Change, SIGNAL(activated()), this, SLOT(changeCurve()));
    QObject::connect(action_Roll_back, SIGNAL(activated()), this, SLOT(rollBack()));
    QObject::connect(actionExtractAll, SIGNAL(activated()), this, SLOT(ExtractAll()));
    QObject::connect(actionSelect, SIGNAL(activated()), this, SLOT(CurveSelect()));
};

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool MainWindow::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("XML DUT - fies"),
                        tr("The document has been modified.\n"
                           "Do you want to save your changes?"),
                        QMessageBox::Yes | QMessageBox::Default,
                        QMessageBox::No,
                        QMessageBox::Cancel | QMessageBox::Escape);
        if (r == QMessageBox::Yes) {
            return save();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::open()
{
    if (okToContinue()) {
      configfileName = QFileDialog::getOpenFileName(this,
                                 tr("Open XML DUT"), ".",
                                 tr("XML DUT files (*.xml)"));
      if (!configfileName.isEmpty())
          loadFile(configfileName);
        connect(this, SIGNAL(SetTitle(QString)), this, SLOT(setWindowTitle(QString)));
        emit SetTitle("Spice Parameter Extractor - "+configfileName.mid(configfileName.lastIndexOf("/")+1,configfileName.length()));
        disconnect(this, SIGNAL(SetTitle(QString)), this, SLOT(setWindowTitle(QString)));

    }

  }

bool MainWindow::save_as()
{
    if (okToContinue()) {
      configfileName = QFileDialog::getSaveFileName(this,
                                 tr("Save XML DUT file"), ".",
                                 tr("XML DUT files (*.xml)"));
      if (!configfileName.isEmpty()){
          if(DUT->save(configfileName)) return true;
          else {
              QMessageBox::warning(this, "ALARM!", "File "+configfileName+" Not Saved");
              return false;
          }
      }else return false;
    }else return false;

  }

void MainWindow::loadFile(QString fileName){
   QTextStream out(stdout);

    QString fn=fileName;

    QFile file(fn);
    DUT = new DUTconfig(&file);

    item = new QStandardItem("DUT");
    reBbuildTree();
}

bool MainWindow::reBbuildTree(){
    delete item;
    item = new QStandardItem("DUT");

    DUT->getTree(item);
    treeModel->clear();
    //без предыдущей строчки, если дерево не свернуто, то на сл. строке вылетает
    treeModel->setItem(0,0, item->takeChild(0,0));

    return true;
}

bool MainWindow::save(){
    if(DUT->save(configfileName)) return true;
    else {
        QMessageBox::warning(this, "ALARM!", "File "+configfileName+" Not Saved");
        return false;
    }
}

bool MainWindow::isDevice(QModelIndex idx){
    if(idx.parent().data().toString()==QString("DUT")){
        return true;
    }else return false;
}
bool MainWindow::isDUT(QModelIndex idx){
    if(idx.data().toString()==QString("DUT")) return true; // && idx.data().toString()==idx.parent().data().toString()) return true;
    else return false;
}

bool MainWindow::isCurve(QModelIndex idx){
    if(idx.parent().parent().data().toString()==QString("DUT")){
        return true;
    }else return false;
}

void MainWindow::treeItemSelected(QModelIndex idx){
QTextStream out(stdout);
out<<"Index Mouse clicked\n";
curve cur;
if (idx.isValid())
    {
        if(isDevice(idx)){

        }
        if(isCurve(idx)){
            //если выбрана характеристика, а не прибор,
            //формируем структуру с необходимой онформацией
            //для ее представления
            //и заполняем таблицы полученной инфой
            //fillTables(DUT->getCurveInfo(idx));

            try{
            cur = DUT->getCurveInfo(idx);
            }catch(DUTconfig_Exception& dcE)
            {
                qDebug()<<"MainWindow::treeItemSelected got: "<< QString::fromStdString(std::string(dcE.what()));
            }catch(...)
            {
                qDebug()<<"MainWindow::treeItemSelected Unknown exception";
            }
            cur.path = configfileName.toStdString();


            //qDebug()<<"treeItemSelected "<<idx.data().toString();
            fillTables(cur);
            emit CurveSignal(cur);
        }
    }
}

void MainWindow::fillTables(curve res){    

    //bool old_blocked_file = ui->filestableWidget->blockSignals(true);
    //bool old_blocked_model = ui->modelparamtableWidget->blockSignals(true);

    //теперь заполняем таблицы полученной инфой
    //таблица с файлами
    size_t i=0;
    ui->filestableWidget->setRowCount(0);
    for(i=0;i<res.expFile.size();i++){
        ui->filestableWidget->setRowCount(ui->filestableWidget->rowCount()+1);

        ui->filestableWidget->setItem(ui->filestableWidget->rowCount()-1,0, new QTableWidgetItem((QString::fromStdString(res.inFile.at(i)))));
        ui->filestableWidget->setItem(ui->filestableWidget->rowCount()-1,1, new QTableWidgetItem((QString::fromStdString(res.outFile.at(i)))));
        ui->filestableWidget->setItem(ui->filestableWidget->rowCount()-1,2, new QTableWidgetItem((QString::fromStdString(res.expFile.at(i)))));
    }

    //таблица с параметрами
    ui->modelparamtableWidget->setRowCount(0);

    for(i=0;i<res.spiceParam.size();i++){
        ui->modelparamtableWidget->setRowCount(ui->modelparamtableWidget->rowCount()+1);

        ui->modelparamtableWidget->setItem(ui->modelparamtableWidget->rowCount()-1,0,new QTableWidgetItem((QString::fromStdString(res.spiceParam.at(i)))));
        ui->modelparamtableWidget->setItem(ui->modelparamtableWidget->rowCount()-1,1,new QTableWidgetItem((QString::fromStdString(res.spiceMin.at(i)))));
        ui->modelparamtableWidget->setItem(ui->modelparamtableWidget->rowCount()-1,2,new QTableWidgetItem((QString::fromStdString(res.spiceInit.at(i)))));
        ui->modelparamtableWidget->setItem(ui->modelparamtableWidget->rowCount()-1,3,new QTableWidgetItem((QString::fromStdString(res.spiceMax.at(i)))));
    }


    ui->model_path->setText(QString::fromStdString(res.model_path));
    ui->Outparse_cb_x->clear();
    ui->Outparse_cb_x->addItem(QString::fromStdString(res.OutParse.pasrse_x));
    ui->Outparse_cb_y->clear();
    ui->Outparse_cb_y->addItem(QString::fromStdString(res.OutParse.pasrse_y));
    //qDebug()<<"mainWindow fillTables res.OutParse = "<<QString::fromStdString(res.OutParse);
    ui->ErrorFunction->setText(QString("%1").arg(res.ErrorFunction));

    if (currentModelPath!=QString::fromStdString(res.model_path)){
        currentModelPath=QString::fromStdString(res.model_path);        
        rebuildComboBox(currentModelPath);
    }
    //должно вызываться последней, т.к. генерирует сигнал изменения итема и => вызывает слот changeCurve()
    //если будет стоять не последним может вызвать путаницу в последовательности заполнения и сохранения информации
    //костыль(
    ui->comboBox->setCurrentIndex(ui->comboBox->findText(QString::fromStdString(res.AlgOpt)));
    //table_widget->blockSignals(old_blocked);
    //ui->filestableWidget->blockSignals(old_blocked_file);
    //ui->modelparamtableWidget->blockSignals(old_blocked_model);
}


void MainWindow::fileButtonClicked(){
    //добавляем в таблицу пути к файлам
    if(ui->file_cir->text()!="" && ui->file_out->text()!="" && ui->file_exp->text()!=""){
            ui->filestableWidget->setRowCount(ui->filestableWidget->rowCount()+1);
            ui->filestableWidget->setItem(ui->filestableWidget->rowCount()-1,0, new QTableWidgetItem(ui->file_cir->text()));
            ui->filestableWidget->setItem(ui->filestableWidget->rowCount()-1,1, new QTableWidgetItem(ui->file_out->text()));
            ui->filestableWidget->setItem(ui->filestableWidget->rowCount()-1,2, new QTableWidgetItem(ui->file_exp->text()));

            ui->file_cir->setText(tr(""));
            ui->file_out->setText(tr(""));
            ui->file_exp->setText(tr(""));
            changeCurve();
        }else{
            QMessageBox::warning(this, "ALARM!", "File(s) Not Selected");
        }
}

void MainWindow::modelButtonClicked(){
    if(ui->param_name->currentText()!="" && ui->param_min->text()!="" && ui->param_init->text()!="" && ui->param_max->text()!=""){
        ui->modelparamtableWidget->setRowCount(ui->modelparamtableWidget->rowCount()+1);

        ui->modelparamtableWidget->setItem(ui->modelparamtableWidget->rowCount()-1,0, new QTableWidgetItem(ui->param_name->currentText()));
        ui->modelparamtableWidget->setItem(ui->modelparamtableWidget->rowCount()-1,1, new QTableWidgetItem(ui->param_min->text()));
        ui->modelparamtableWidget->setItem(ui->modelparamtableWidget->rowCount()-1,2, new QTableWidgetItem(ui->param_init->text()));
        ui->modelparamtableWidget->setItem(ui->modelparamtableWidget->rowCount()-1,3, new QTableWidgetItem(ui->param_max->text()));

        ui->param_min->setText(tr(""));
        ui->param_init->setText(tr(""));
        ui->param_max->setText(tr(""));
        //changeCurve();

        QModelIndex idx;
        //if(ui->model_path->text()!=QString("")&&(ui->filestableWidget->rowCount()!=0)&&(ui->modelparamtableWidget->rowCount()!=0)){

            idx = ui->treeView->selectionModel()->currentIndex();

            if (!(idx.data().toString()=="")) {
                //qDebug()<<"idx = \"\"";
                changeCurve();
            }

    }else{
            QMessageBox::warning(this, "ALARM!", "Parameter(s) Not Selected");
        }

}


void MainWindow::showPopup(const QPoint & iPoint)
{
    QTreeView * item = 0 ;
  if ( 0 == item )
  {
    QMenu menu(ui->treeView);
    if(isDUT(ui->treeView->indexAt(iPoint))){
        menu.addAction(actionNew_device);
    }
    if(isDevice(ui->treeView->indexAt(iPoint))){
        menu.addAction(actionRemove_device);
        menu.addAction(actionInsert_curve);

    }
    if(isCurve(ui->treeView->indexAt(iPoint))) {
        menu.addAction(actionRemove_Curve);
        menu.addAction(action_Change);

    }

   // Show the popup where you clicked see also QCursor::pos() something like that
     //qDebug()<<":popuprightclickMenu"; //just to see if activated
    menu.exec(ui->treeView->mapToGlobal(iPoint));
  }
  else
  {
    QMenu menu(ui->treeView);
  }
}

void MainWindow::showFileTablePopup(const QPoint & iPoint)
{    
    //qDebug()<<"showFileTablePopup(const QPoint & iPoint)";
    QAction *actionEditFile;
    actionEditFile = new QAction(this);
    actionEditFile->setObjectName(QString::fromUtf8("Edit File"));
    //connect(actionEditFile, SIGNAL(activated()), MainWindow, SLOT(EditFile()));
    connect(actionEditFile, SIGNAL(activated()), this, SLOT(EditFile()));
    //connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    actionEditFile->setText(QApplication::translate("MainWindow", "&Edit File", 0, QApplication::UnicodeUTF8));
    //actionEditFile->setShortcut(QApplication::translate("MainWindow", "Ctrl+E", 0, QApplication::UnicodeUTF8));

    QAction *actionPlotCurve;
    actionPlotCurve = new QAction(this);
    actionPlotCurve->setObjectName(QString::fromUtf8("Plot Curve"));
    actionPlotCurve->setText(QApplication::translate("MainWindow", "Plot Curve", 0, QApplication::UnicodeUTF8));
    connect(actionPlotCurve, SIGNAL(activated()), this, SLOT(plot_single()));

    //if(ui->filestableWidget->currentItem()->text()!=""){
    if(ui->filestableWidget->selectedItems().size()!=0){// currentItem()->text()!=""){
        //qDebug()<<"TEXT: "<<ui->filestableWidget->currentItem()->text();
        QMenu menu(ui->filestableWidget);
        menu.addAction(actionEditFile);
        menu.addAction(actionPlotCurve);

        menu.exec(ui->filestableWidget->mapToGlobal(iPoint));
    }



}

void MainWindow::EditFile(){

    TextEditor *mw = new TextEditor();
    QString str = QDir::currentPath();
    QString file=str+"/"+ui->filestableWidget->currentItem()->text();
            //configfileName=str+"/"+ui->filestableWidget->currentItem()->text();
    //mw->loadFile(configfileName);
    mw->loadFile(file);
    mw->show();
}

/*
QAction *actionEditor;
actionEditor = new QAction(MainWindow);
        actionEditor->setObjectName(QString::fromUtf8("actionEditor"));
QObject::connect(actionEditor, SIGNAL(activated()), MainWindow, SLOT(Editor()));
actionEditor->setText(QApplication::translate("MainWindow", "&Editor", 0, QApplication::UnicodeUTF8));
actionEditor->setShortcut(QApplication::translate("MainWindow", "Ctrl+E", 0, QApplication::UnicodeUTF8));


*/


void MainWindow::newDev(){
 bool ok;
     QString name = QInputDialog::getText(this, tr("Input dialog"),
                                          tr("Enter device name:"), QLineEdit::Normal,
                                          //QDir::home().dirName(), &ok);
                                          QString(""), &ok);    
    if(name!=""){
        if(ui->treeView->selectionModel()->currentIndex().isValid()){
            QModelIndex idx = ui->treeView->selectionModel()->currentIndex();
            if(!DUT->createDevice(treeModel ,idx, name)) QMessageBox::warning(this, "Warning!", "Device is not reated :(...");
        }
         //qDebug()<<"newDev ui->treeView->selectionModel()->currentIndex().isValid()\n";
    }
    //qDebug()<<"newDev Finish\n";
}

bool MainWindow::remDev(){
    QMessageBox msgBox;
            msgBox.setText("Realy Delete?");// .addButton(tr("Connect")
          msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
          switch (msgBox.exec()) {
          case QMessageBox::Yes:{
                  // yes was clicked
                  //удаляем прибор
                  QModelIndex idx = ui->treeView->selectionModel()->currentIndex();
                  if(DUT->remDev(treeModel ,idx)){
                      return true;
                  }
              }
          case QMessageBox::No:
              return false;
          default:
              return false;
          }
}
bool MainWindow::remCurve(){
    QMessageBox msgBox;
            msgBox.setText("Realy Delete?");// .addButton(tr("Connect")
          msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
          switch (msgBox.exec()) {
          case QMessageBox::Yes:{
                  // yes was clicked
                  //удаляем прибор
                  QModelIndex idx = ui->treeView->selectionModel()->currentIndex();
                  if(DUT->remCurve(treeModel ,idx)){
                      return true;
                  }
              }
              //break;
          case QMessageBox::No:
              return false;
          default:
              return false;
          }
}



void MainWindow::InsertCurve(){
    qDebug()<<"MainWindow::InsertCurve()"<<"\n";
    QModelIndex idx = ui->treeView->selectionModel()->currentIndex();

    bool ok;
        QString name = QInputDialog::getText(this, tr("Input dialog"),
                                             tr("Enter curve name:"), QLineEdit::Normal,
                                                 QString(""), &ok);
   if(name!=""){
       qDebug()<<"Curve Name = "<<name<<"\n";
       if(DUT->createCurve(treeModel, idx, name, fillCurve())){
           QMessageBox::warning(this, "ALARM!", "Curve Created!");
       }
   }
}

curve MainWindow::fillCurve(){
    curve res;
    //заполняем структуры данными о ВАХе
    //используемые файлы, параметрв, алгоритм оптимизации
    //путь к файлу с моделью - это временная мера. Потом надо бут переделать
    //чтобы путь к файлу с моделью обрабатывался на уровне DUT

    int i=0;
    //заполняем инфу о файлах
    for(i=0;i<(ui->filestableWidget->rowCount());i++){
        //qDebug()<<"fillCurve File: "<< ui->filestableWidget->item(i,0)->text();
        res.inFile.push_back(ui->filestableWidget->item(i,0)->text().toStdString());
        res.outFile.push_back(ui->filestableWidget->item(i,1)->text().toStdString());
        res.expFile.push_back(ui->filestableWidget->item(i,2)->text().toStdString());
    }
    //заполняем инфу о параметрах
    for(i=0;i<ui->modelparamtableWidget->rowCount();i++){
        //qDebug()<<"fillCurve Param: "<< ui->modelparamtableWidget->item(i,0)->text();
        res.spiceParam.push_back(ui->modelparamtableWidget->item(i,0)->text().toStdString());
        res.spiceMin.push_back(ui->modelparamtableWidget->item(i,1)->text().toStdString());
        res.spiceInit.push_back(ui->modelparamtableWidget->item(i,2)->text().toStdString());
        res.spiceMax.push_back(ui->modelparamtableWidget->item(i,3)->text().toStdString());
    }
    //OutParce и ErroFunc
    //res.OutParse=ui->OutParce->text().toStdString();

    if(ui->Outparse_cb_x->currentText().size() > 0)
        res.OutParse.pasrse_x=ui->Outparse_cb_x->currentText().toStdString();
    else
        throw MainWindow_Exception("Outparse_cb_x is empty");

    if(ui->Outparse_cb_y->currentText().size() > 0)
        res.OutParse.pasrse_y=ui->Outparse_cb_y->currentText().toStdString();
    else
        throw MainWindow_Exception("Outparse_cb_y is empty");

    //qDebug()<<"ui->OutParce->text() = "<<ui->OutParce->text();
    res.ErrorFunction=ui->ErrorFunction->text().toInt();
    //Алгоритм
    res.AlgOpt=ui->comboBox->currentText().toStdString();
    //и путь к моделе
    res.model_path=ui->model_path->text().toStdString();

    res.path = configfileName.toStdString();

    //Вроде всю инфу собрали. Возвращаем
    //qDebug()<<"fillCurve\n";
    //DUT->getGlobalParams(&res);
    return res;
}

void MainWindow::filesTableClicked(){
    ui->fileButton->setText("Remove");
};

void MainWindow::fileButtonRClicked(){
    ui->filestableWidget->removeRow(ui->filestableWidget->selectionModel()->currentIndex().row());
    changeCurve();
}
void MainWindow::modelButtonRClicked(){
    ui->modelparamtableWidget->removeRow(ui->modelparamtableWidget->selectionModel()->currentIndex().row());
    changeCurve();
}


void MainWindow::rebuildComboBox(QString fileName){
    //qDebug()<<"REBUILD COMBOBOX\n";


    QString str = QDir::currentPath();
    QStringList list_par, list_val;
    //qDebug()<<"str = "<<str<<"\n";

    fileName=str+"/"+fileName;
    //qDebug()<<"fileName = "<<fileName<<"\n";
      QFile file( fileName );
      if ( file.open( QIODevice::ReadOnly| QIODevice::ReadWrite) ) {
          QTextStream stream( &file );
          QString line;//, tmpstr;
          //int i = 1;

          QRegExp m;
          m.setPattern(string("[+]?\\s*([\\w*\\d*]*)\\s*=\\s*([[-+]?\\d*\\.?\\d*[eE]?[-+]?\\d*)").c_str());

          while ( !stream.atEnd() ) {
              line = stream.readLine(); // line of text excluding '\n'

              if (line.indexOf("*")!=0){
                int pos = 0;//m.indexIn(line);
                while ((pos = m.indexIn(line, pos)) != -1) {
                     list_par << m.cap(1);
                     list_val << m.cap(2);
                     pos += m.matchedLength();
                 }
            }


          }
          file.close();
          //qDebug()<<"LAST\n";
          //for(int i = 0; i < list_par.size(); ++i){
          //   qDebug()<<list_par.at(i)<<" = "<<list_val.at(i)       ;
          //   }

      }else{
        qDebug()<<"ERROR\n";
      }
      ui->param_name->clear();
      ui->param_name->addItems(list_par);
}


void MainWindow::browseFile(){

    int id=0;

    if(ui->file_cir->hasFocus()) id=1;
    if(ui->file_exp->hasFocus()) id=2;
    if(ui->file_out->hasFocus()) id=3;
    if(ui->model_path->hasFocus()) id=4;    

    QString file = QFileDialog::getOpenFileName(this,
                               tr("Open Spreadsheet"), ".",
                               tr("Spreadsheet files (*.*)"));

    if (!file.isEmpty()){

        QString str = QDir::currentPath();
        QString res=file.mid(str.size()+1, file.size());

        if(id==1) ui->file_cir->setText(res);
        if(id==2) ui->file_exp->setText(res);
        if(id==3) ui->file_out->setText(res);

        if(id==4) {
            ui->model_path->setText(res);
            currentModelPath=res;
            rebuildComboBox(currentModelPath);
        }

    }
}


void MainWindow::calculate(){
    //fillTables(DUT->getCurveInfo(idx));
    if(ui->pushButton->text()!=tr("Stop")){
    QModelIndex idx = ui->treeView->currentIndex();
    curve cur;

    if (idx.isValid())
        {
            if(isDevice(idx)){

            }
            if(isCurve(idx)){
                //если выбрана характеристика, а не прибор,
                //формируем структуру с необходимой онформацией
                //для ее представления
                //и заполняем таблицы полученной инфой

                //qDebug()<<"MainWindow::calculate "<<QString::fromStdString(cur.model_path)<<"\n";
                cur = DUT->getCurveInfo(idx);
                cur.path = configfileName.toStdString();
                run(cur);
            }
        }
}else{
    extr->kill();
    //edithr->kill_extr();
    }

}

void MainWindow::run(curve cur){
    //где - то в этой ф-и кроется причина вылетов программы

                //qDebug()<<"MainWindow::run "<<QString::fromStdString(cur.model_path)<<"\n";


                extr = new ExtrThread(cur, configfileName);

                qRegisterMetaType<curve>("curve");
                //connect(extr, SIGNAL(started()), this, SLOT(processStarted()),Qt::QueuedConnection);
                //connect(extr, SIGNAL(stopped(curve)), this, SLOT(changeCurve(curve)),Qt::QueuedConnection);
                //connect(extr, SIGNAL(finished()), this, SLOT(processStoped()),Qt::QueuedConnection);
                connect(extr, SIGNAL(started()), this, SLOT(processStarted()));
                connect(extr, SIGNAL(stopped(curve)), this, SLOT(changeCurve(curve)));
                connect(extr, SIGNAL(finished()), this, SLOT(processStoped()));

                /*edithr = new EditorThread(cur);
                qRegisterMetaType<curve>("curve");
                connect(edithr, SIGNAL(started()), this, SLOT(processStarted()),Qt::QueuedConnection);
                connect(edithr, SIGNAL(stopped(curve)), this, SLOT(changeCurve(curve)),Qt::QueuedConnection);
                connect(edithr, SIGNAL(finished()), this, SLOT(processStoped()),Qt::QueuedConnection);*/

                //edithr = new EditorThread();
                //connect(extr, SIGNAL(SetLine(QString)),edithr, SLOT(AddLine(QString)),Qt::QueuedConnection);
                //connect(extr, SIGNAL(SetLine(QString)),edithr, SLOT(AddLine(QString)),Qt::AutoConnection);
                //connect(extr, SIGNAL(SetLine(QString)),edithr, SLOT(AddLine(QString)));
                //connect(extr, SIGNAL(SetLine(QString)),this, SLOT(AddLine(QString)));
                //connect(this, SIGNAL(SetLine(QString)),edithr, SLOT(AddLine(QString)));



                //qDebug()<<"MainWindow::calculate()\n";

                //edithr->start();

                TextEditor *edtr = new TextEditor();
                connect(extr, SIGNAL(SetLine(QString)),edtr, SLOT(AddLine(QString)));//,Qt::QueuedConnection);

                connect(this, SIGNAL(SetTitle(QString)), edtr, SLOT(setWindowTitle(QString)));
                emit SetTitle("Extraction");

                edtr->show();

                extr->start();
}

void MainWindow::AddLine(QString line){
    emit SetLine(line);
}

void MainWindow::manualFit(){
    if(ui->pushButton->text()!=tr("Stop")){
    QModelIndex idx = ui->treeView->currentIndex();
    curve cur;

    if (idx.isValid())
        {
            if(isDevice(idx)){

            }
            if(isCurve(idx)){
                cur = DUT->getCurveInfo(idx);
                cur.path = configfileName.toStdString();


                QString str = QDir::currentPath();

                QVector<QString> fileOut;
                QVector<QString> fileExp;

                for(size_t i=0; i<cur.inFile.size();i++){
                    fileOut.append(QString::fromStdString(cur.get_prefix()+cur.outFile.at(i)));
                    fileExp.append(QString::fromStdString(cur.get_prefix()+cur.expFile.at(i)));
                }

                Plot *plot = new Plot( fileOut, fileExp, "X Axis","Y Axis");
                mf = new ManualFit(cur);

                connect(mf, SIGNAL(redrow()),plot, SLOT(reDrow()),Qt::QueuedConnection);
                connect(mf, SIGNAL(res(curve)),this, SLOT(changeCurve(curve)));

                plot->show(this);
                mf->show();
            }
        }
    }
}


void MainWindow::changeCurve(){
    //пока очень криво(
    qDebug()<<"changeCurve CATCHTEST ";
    bool error=false;
    QModelIndex idx;
    if(ui->model_path->text()!=QString("")&&(ui->filestableWidget->rowCount()!=0)&&(ui->modelparamtableWidget->rowCount()!=0)){

        idx = ui->treeView->selectionModel()->currentIndex();

        if (idx.data().toString() =="") {
            //qDebug()<<"idx = \"\"";
            error=true;
        }
        if(!error){
            //qDebug()<<"changeCurve "<<QString("%1").arg(ui->filestableWidget->rowCount())<<" "<<QString("%1").arg(ui->modelparamtableWidget->rowCount());
            DUT->changeCurve(idx, fillCurve());
        }

    }    
}

void MainWindow::changeCurve(curve cur){    
    //qDebug()<<"void MainWindow::changeCurve(curve cur)\n";
    if(ui->model_path->text()!=QString("")&&(ui->filestableWidget->rowCount()!=0)&&(ui->modelparamtableWidget->rowCount()!=0)){
        //qDebug()<<"changeCurve "<<QString("%1").arg(ui->filestableWidget->rowCount())<<" "<<QString("%1").arg(ui->modelparamtableWidget->rowCount());
        DUT->changeCurve(cur.idx, cur);
    }
    //qDebug()<<"DUT->changeCurve(treeModel,cur.idx, cur); done\n";
    //ui->treeView->selectionModel()->setCurrentIndex(&cur.idx,QItemSelectionModel::Select); //->currentIndex();
    ui->treeView->selectionModel()->setCurrentIndex(cur.idx, QItemSelectionModel::SelectCurrent);   
    //qDebug()<<"ui->treeView->selectionModel()->setCurrentIndex(cur.idx, QItemSelectionModel::SelectCurrent);\n";
    treeItemSelected(cur.idx);
    //qDebug()<<"void MainWindow::changeCurve(curve cur)\n";
    //qDebug()<<"result = "<<cur.result<<"\n";
}

void MainWindow::rollBack(){
    //вернем начальное значение, если не нравится новое
    curve cur;
    cur = DUT->getCurveInfo(ui->treeView->currentIndex());
    for(size_t i=0;i<cur.spiceInit.size();i++){
        cur.spiceInit.at(i)=cur.spiceOldVal.at(i);
    }
    fillTables(cur);
}


void MainWindow::plot()
{
    //if(ui->pushButton->text()!=tr("Stop")){
    QModelIndex idx = ui->treeView->currentIndex();
    curve cur;

    if (idx.isValid())
        {
            if(isDevice(idx)){

            }
            if(isCurve(idx)){
                //если выбрана характеристика, а не прибор,
                //формируем структуру с необходимой онформацией
                //для ее представления
                //и заполняем таблицы полученной инфой
                cur = DUT->getCurveInfo(idx);
                plot(cur);
            }
        }
    //}
}

void MainWindow::plot_single(){

    plot();
}

void MainWindow::plot(curve cur){
    QString str = QDir::currentPath();

    QVector<QString> fileOut;
    QVector<QString> fileExp;
    cur.path = configfileName.toStdString();


    for(size_t i=0; i<cur.inFile.size();i++){
        fileOut.append(QString::fromStdString(cur.get_prefix()+cur.outFile.at(i)));
        fileExp.append(QString::fromStdString(cur.get_prefix()+cur.expFile.at(i)));
    }

    Plot *plot = new Plot( fileOut, fileExp, "X Axis","Y Axis");
    plot->show(this);
}


void MainWindow::ConfigEdit(){
    qDebug()<<"MainWindow::ConfigEdit()\n";
    TextEditor *mw = new TextEditor();
    //mw->resize( 700, 800 );
    //mw->loadFile("opt.conf");
    mw->show();
}

void MainWindow::Editor(){
    qDebug()<<"MainWindow::Editor()\n";
    TextEditor *mw = new TextEditor();
    //mw->resize( 700, 800 );
    mw->show();

}

void MainWindow::processStarted(){
    ui->pushButton->setText("Stop");
}
void MainWindow::processStoped(){
    ui->pushButton->setText("Run");
}

void MainWindow::ExtractAll(){
    qDebug()<<"MainWindow::ExtractAll\n";
    bool ok=false;

    QModelIndex idx = ui->treeView->currentIndex();

    if(isDevice(idx)){
        ok=true;        
        qDebug()<<idx.parent().data().toString()<<"\n";
        idx=idx.parent();
    }else{
        if(isCurve(idx)){
            ok=true;
            qDebug()<<idx.parent().parent().data().toString()<<"\n";
            idx=idx.parent().parent();
        }
    }
    if(ok){
        int d=0;
        QVector <curve> curvect;
        while(idx.child(d,0).isValid()){
            int c=0;
            QModelIndex dev=idx.child(d,0);
            //qDebug()<<"Dev: "<<idx.child(d,0).data().toString()<<"\n";
            while(dev.child(c,0).isValid()){
                //qDebug()<<"Cur: "<<dev.child(c,0).data().toString()<<"\n";
                QModelIndex curv = dev.child(c,0);
                curvect.append(DUT->getCurveInfo(curv));
                c++;
            }
            d++;
        }
        if(ui->pushButton->text()!=tr("Stop")){

            extr = new ExtrThread(curvect);

            qRegisterMetaType<curve>("curve");
            connect(extr, SIGNAL(started()), this, SLOT(processStarted()));
            connect(extr, SIGNAL(stopped(curve)), this, SLOT(changeCurve(curve)));
            connect(extr, SIGNAL(finished()), this, SLOT(processStoped()));

            /*edithr = new EditorThread();
            connect(extr, SIGNAL(SetLine(QString)),edithr, SLOT(AddLine(QString)),Qt::QueuedConnection);

            edithr->start();*/

            TextEditor *edtr = new TextEditor();
            connect(extr, SIGNAL(SetLine(QString)),edtr, SLOT(AddLine(QString)),Qt::QueuedConnection);
            connect(this, SIGNAL(SetTitle(QString)), edtr, SLOT(setWindowTitle(QString)));
            emit SetTitle("Extraction");

            //edtr->resize( 700, 800 );
            edtr->show();

            extr->start();
        }
    }
}



void MainWindow::CurveSelect(){
    cursel = new CurveSelection();

    qRegisterMetaType<curve>("curve");
    connect(this,SIGNAL(CurveSignal(curve)), cursel, SLOT(Selected(curve)),Qt::QueuedConnection);
    //connect(this,SIGNAL(CurveSignal(curve)), cursel, SLOT(Selected(curve)));
    connect(cursel, SIGNAL(SetCurve(curve)),this, SLOT(changeCurve(curve)),Qt::QueuedConnection);

    /*edithr = new EditorThread();
    connect(cursel, SIGNAL(SetLine(QString)),edithr, SLOT(AddLine(QString)),Qt::QueuedConnection);
    edithr->start();*/
    cursel->show();
}

void MainWindow::GetParamValue(QString str){
    //qDebug()<<"MainWindow::GetParamValue"<<str;
    QVector<QString> vals = DUT->getSpiceVals(str);
    if(vals.size()!=0){
            ui->param_min->setText(vals.at(0));
            ui->param_init->setText(vals.at(1));
            ui->param_max->setText(vals.at(2));
        }else{
            ui->param_min->setText("");
            //ui->param_init->setText("");
            ui->param_init->setText(spiceValueFromModelFile(str));
            ui->param_max->setText("");
        }
}

QString MainWindow::spiceValueFromModelFile(QString valName){
    //qDebug()<<"REBUILD COMBOBOX\n";

    QString res="";
    QString str = QDir::currentPath();
    QString fileName=ui->model_path->text();

    fileName=str+"/"+fileName;
    //qDebug()<<"fileName = "<<fileName<<"\n";
      QFile file( fileName );
      if(file.exists(fileName)){
      if ( file.open( QIODevice::ReadOnly| QIODevice::ReadWrite) ) {
          QTextStream stream( &file );
          QString line;
          //int i = 1;
          while ( !stream.atEnd() ) {
              line = stream.readLine(); // line of text excluding '\n'
              if ((line.indexOf("+")==0)&&(line.indexOf(valName)>0)) {
                  res=line.mid(line.indexOf("=")+1, line.length());
                  if(res.indexOf(" ")!=-1) res=res.mid(0, res.indexOf(" "));
                  //qDebug()<<"line.lastIndexOf"<<res.lastIndexOf(" ")<<" res = "<<res;
              }
          }
          file.close();
      }else{
        qDebug()<<"ERROR\n";
      }
  }
      return res;
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
switch (event->key()) {
case Qt::Key_Return:// Key_Home:
if (event->modifiers() & Qt::ControlModifier) {
//goToBeginningOfDocument();
    qDebug()<<"Return event->modifiers() & Qt::ControlModifier";
} else {
//goToBeginningOfLine();
    qDebug()<<"Return event->modifiers() ";
}
break;
case Qt::Key_End:
//•••
default:
QWidget::keyPressEvent(event);
}
}

void MainWindow::about(){
    About *about = new About();
    about->show();
}

void MainWindow::OptParamsEdit(){
    curve cur;
    DUT->getGlobalParams(&cur);
    OptParamsEditor *ope = new OptParamsEditor(NULL, &cur);

    qRegisterMetaType<curve>("curve");
    connect(this, SIGNAL(SetTitle(QString)), ope, SLOT(setWindowTitle(QString)));
    emit SetTitle("Parameter Configurator");
    disconnect(this, SIGNAL(SetTitle(QString)), this, SLOT(setWindowTitle(QString)));

    connect(ope,SIGNAL(ChangeCurveParamsSignal(curve)), this, SLOT(changeOptParams(curve)),Qt::QueuedConnection);

    ope->show();
    //if(ope->isEdited()) DUT->changeConfig(cur);
}

void MainWindow::changeOptParams(curve cur){
    DUT->changeConfig(cur);
}

bool MainWindow::eventFilter( QObject *obj, QEvent *evt )
  {
    if(evt->type() == QEvent::KeyRelease)
    {

        QKeyEvent *k = static_cast< QKeyEvent * >( evt );

        switch (k->key()) {
        case Qt::Key_Return: //qDebug()<<"Return Pressed";
            if(ui->filestableWidget->hasFocus()){
                //qDebug()<<"TEXT filestableWidget: "<<ui->filestableWidget->currentItem()->text();
                changeCurve();
            }
            if(ui->modelparamtableWidget->hasFocus()){
                //qDebug()<<"TEXT modelparamtableWidget: "<<ui->modelparamtableWidget->currentItem()->text();
                changeCurve();
            }
            if(ui->Outparse_cb_x->hasFocus()){
                //qDebug()<<"TEXT modelparamtableWidget: "<<ui->modelparamtableWidget->currentItem()->text();
                changeCurve();
            }
            if(ui->Outparse_cb_y->hasFocus()){
                //qDebug()<<"TEXT modelparamtableWidget: "<<ui->modelparamtableWidget->currentItem()->text();
                changeCurve();
            }
            if(ui->ErrorFunction->hasFocus()){
                //qDebug()<<"TEXT modelparamtableWidget: "<<ui->modelparamtableWidget->currentItem()->text();
                changeCurve();
            }
            break;
        case Qt::Key_Control: //qDebug()<<"Control Pressed";
            break;
        default:

            break;
        }


    }    
    /*
QMouseEvent *m = static_cast< QMouseEvent * >( evt );
      QWidget *w = QApplication::widgetAt( m->globalPos() );
*/

    /*if(evt->type() == QEvent::MouseButtonPress){
        qDebug()<<"QEvent::MouseButtonPress) ";

        QMouseEvent *m = static_cast< QMouseEvent * >( evt );
        showFileTablePopup(m->globalPos());
        mouseMoveEvent(m);
        //QPoint
        //const QPoint & QMouseEvent::globalPos () const
    }*/

    return QMainWindow::eventFilter( obj, evt );
  }

//MainWindow_Exception/////////////////////////////////
const char* MainWindow_Exception::what() const throw() { return s_.c_str(); }
MainWindow_Exception::MainWindow_Exception(std::string s) : s_(s) {}
MainWindow_Exception::~MainWindow_Exception()  throw() {}
/////////////////////////////////MainWindow_Exception


