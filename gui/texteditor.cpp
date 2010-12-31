#include "texteditor.h"
#include "ui_texteditor.h"
#include <QMessageBox>
#include <QtGui>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QFileDialog>
#include <QFocusEvent>

TextEditor::TextEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TextEditor)
{
    ui->setupUi(this);
    this->resize( 700, 700 );
}

TextEditor::~TextEditor()
{
    delete ui;
}

void TextEditor::changeEvent(QEvent *e)
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

bool TextEditor::loadFile(const QString &f)
{
    qDebug()<<"LAOD\n";
    if (!QFile::exists(f))
        return false;
    QFile file(f);
        if ( file.open( QIODevice::ReadOnly| QIODevice::ReadWrite) ) {
            QTextStream stream( &file );
            QString line, tmpstr;
            QString str;
            while ( !stream.atEnd() ) {
                line = stream.readLine(); // line of text excluding '\n'
                     //qDebug()<<line<<"\n";
                     ui->textEdit->append(line);
            }
            file.close();
        }else{            
          qDebug()<<"ERROR\n";
          return false;
        }


    setCurrentFileName(f);
    return true;
}

void TextEditor::setCurrentFileName(const QString &fileName)
{
    this->fileName = fileName;
    ui->textEdit->document()->setModified(false);

    QString shownName;
    if (fileName.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(fileName).fileName();

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Rich Text")));
    setWindowModified(false);
}

bool TextEditor::fileSave()
{
    if (fileName.isEmpty())
        return fileSaveAs();

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly))
        return false;
    QTextStream ts(&file);
    ts << ui->textEdit->document()->toPlainText();// ->toHtml("UTF-8");
    ui->textEdit->document()->setModified(false);
    return true;
}
bool TextEditor::fileSaveAs()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                              QString(), tr("Text-Files (*.txt *.conf);;All Files (*)"));
    if (fn.isEmpty())
        return false;
    setCurrentFileName(fn);
    return fileSave();
}

void TextEditor::fileOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              QString(), tr("All Files (*);;Text-Files (*.txt *.conf *.xml)"));
    if (!fn.isEmpty())
        loadFile(fn);
}
void TextEditor::addLine(QString line){
    ui->textEdit->append(line);
}
void TextEditor::closeEvent(QCloseEvent *event)
{
    emit closing();
    event->accept();

}

void TextEditor::AddLine(QString line){
    addLine(line);
}

void TextEditor::reload(){
    if(fileName!="") {
        ui->textEdit->document()->clear();
        loadFile(fileName);
    }
}
/*void TextEditor::show(QWidget *parentWindow){
    this->show();
}

void TextEditor::show(){
    this->show();
}*/
