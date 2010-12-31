#ifndef DUTCONFIG_H
#define DUTCONFIG_H

 #include <QObject>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <QDomElement>
#include <QStandardItemModel>
#include <string>
#include <vector>
#include "globals.h"

class DUTconfig
{
    //Q_OBJECT


public:    
    DUTconfig();
    DUTconfig(QIODevice *device);
    void getTree(QStandardItem *item);
    //void getTreeItems(QStandardItem *item, QDomElement *dom);
    //QStandardItem* getTree();

    curve getCurveInfo(QModelIndex idx);
    bool createDevice(QStandardItemModel* model, QModelIndex idx, QString name);
    bool createCurve(QStandardItemModel* model, QModelIndex idx, QString name, curve cur);
    bool changeCurve(QModelIndex idx, curve cur);
    bool remDev(QStandardItemModel* model, QModelIndex idx);
    bool remCurve(QStandardItemModel* model, QModelIndex idx);
    bool save(QString file);
    bool changeConfig(curve cur);
    void getGlobalParams(curve* res);
    QVector<QString> getSpiceVals(QString name);

//signals:
    //void modelpathchanged();

private:
    QDomNode getDut(QDomNode root);
    //QDomElement getDut(QDomElement root);
    QDomNode getConfig(QDomNode root);
    //QDomElement getConfig(QDomElement root);
    void parseNode(const QDomElement &element, QStandardItem *parent);
    //void parseNode(const QDomElement &element);
    QDomElement getCurve(QDomElement parent, QString name);
    QDomElement getDevice(QString name);
    bool save();


    QDomDocument doc;    
    QDomElement root;
    QDomNode dut;
    //QDomElement dut;
    QDomNode config;
    //QDomNode modelpath;
    //QDomElement config;



};

#endif // DUTCONFIG_H
