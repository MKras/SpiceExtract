#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>

namespace Ui {
    class TextEditor;
}

class TextEditor : public QMainWindow {
//class TextEditor : public QWidget {
    Q_OBJECT
public:
    TextEditor(QWidget *parent = 0);
    ~TextEditor();
    bool loadFile(const QString &f);
    void addLine(QString);
    /*void show(QWidget *parentWindow);
    void show();*/

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private:
    //bool realySave();
    //void fileNew();
    QString fileName;
    Ui::TextEditor *ui;
    void setCurrentFileName(const QString &fileName);
    //bool loadFile(const QString &f);
    //bool fileSave();
    bool fileSaveAs();

    QAction *actionSave,
        *actionTextBold,
        *actionTextUnderline,
        *actionTextItalic,
        *actionTextColor,
        *actionAlignLeft,
        *actionAlignCenter,
        *actionAlignRight,
        *actionAlignJustify,
        *actionUndo,
        *actionRedo,
        *actionCut,
        *actionCopy,
        *actionPaste;

private slots:
    //void fileNew();
    void fileOpen();
    bool fileSave();
    void AddLine(QString);
    void reload();
    //void closed();
signals:
    void closing();
};

#endif // TEXTEDITOR_H
