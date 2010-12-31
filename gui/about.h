#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>

namespace Ui {
    class About;
}

class About : public QWidget {
    Q_OBJECT
public:
    explicit About(QWidget *parent = 0);
    ~About();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::About *ui;

signals:
    void SetTitle(QString);
};

#endif // ABOUT_H
