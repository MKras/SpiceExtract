#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    connect(this, SIGNAL(SetTitle(QString)), this, SLOT(setWindowTitle(QString)));
    emit SetTitle("About");
    disconnect(this, SIGNAL(SetTitle(QString)), this, SLOT(setWindowTitle(QString)));
}

About::~About()
{
    delete ui;
}

void About::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
