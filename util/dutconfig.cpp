#include "dutconfig.h"
#include <QMessageBox>
#include <QtGui>
#include <QTreeView>
#include <QStandardItemModel>

#include <stdlib.h>
#include <iostream>
#include <fstream>


DUTconfig::DUTconfig(QIODevice *device)
{
QTextStream out(stdout);
    QString errorStr;
  int errorLine;
  int errorColumn;

  if (!doc.setContent(device, true, &errorStr, &errorLine,
                      &errorColumn)) {
      std::cout<<"setContent(): "<<endl;
      QMessageBox::warning(0, QObject::tr("DOM Parser"),
                           QObject::tr("Parse error at line %1, "
                                       "column %2:\n%3")
                           .arg(errorLine)
                           .arg(errorColumn)
                           .arg(errorStr));
      return;
  }
  root = doc.documentElement();
  if (root.tagName() != "DUT")
      return;

   QDomElement DUT = doc.documentElement();
   DUT.appendChild(getConfig(root));
   DUT.appendChild(getDut(root));
   doc.appendChild(DUT);     
}

DUTconfig::DUTconfig(QIODevice *device, QString configfileName)
    : device_(device)
{
    configfileName_ = configfileName;
    if(NULL != device_ )
        DUTconfig(device_);
}

DUTconfig::DUTconfig(){    
    QString filename="z_out.xml";
    const int Indent = 4;

    root = doc.createElement("DUT");
    config = root.toDocument().createElement("Config");
    QDomElement modelpath = config.toDocument().createElement("SpiceModel");
    config.appendChild(modelpath);
    dut = root.toDocument().createElement("DUT");

    //QDomElement modelpath = config.toDocument().createElement("SpiceModel");
    //config.appendChild(modelpath);

    QDomElement spice = doc.createElement("Spice");
    spice.appendChild(doc.createTextNode("spice -b"));
    config.appendChild(spice);
    QDomElement accrcy = doc.createElement("Accrcy");
    accrcy.appendChild(doc.createTextNode("1e-6"));
    config.appendChild(accrcy);
    QDomElement FTol = doc.createElement("FTol");
    FTol.appendChild(doc.createTextNode("1e-6"));
    config.appendChild(FTol);
    QDomElement Gtols = doc.createElement("Gtols");
    Gtols.appendChild(doc.createTextNode("1e-8"));
    config.appendChild(Gtols);
    QDomElement maxiter = doc.createElement("Maxiter");
    maxiter.appendChild(doc.createTextNode("600"));
    config.appendChild(maxiter);
    QDomElement backtrackiter = doc.createElement("Backtrackiter");
    backtrackiter.appendChild(doc.createTextNode("10000"));
    config.appendChild(backtrackiter);
    QDomElement Feval = doc.createElement("Feval");
    Feval.appendChild(doc.createTextNode("20000"));
    config.appendChild(Feval);

    QDomElement LM_mu = doc.createElement("LM_mu");
    LM_mu.appendChild(doc.createTextNode("1e-5"));
    config.appendChild(LM_mu);
    QDomElement LM_epsilon1 = doc.createElement("LM_epsilon1");
    LM_epsilon1.appendChild(doc.createTextNode("1.0e-10"));
    config.appendChild(LM_epsilon1);
    QDomElement LM_epsilon2 = doc.createElement("LM_epsilon2");
    LM_epsilon2.appendChild(doc.createTextNode("1.0e-10"));
    config.appendChild(LM_epsilon2);
    QDomElement LM_epsilon3 = doc.createElement("LM_epsilon3");
    LM_epsilon3.appendChild(doc.createTextNode("1.0e-10"));
    config.appendChild(LM_epsilon3);
    QDomElement LM_JacobianStep = doc.createElement("LM_JacobianStep");
    LM_JacobianStep.appendChild(doc.createTextNode("1.0e-8"));
    config.appendChild(LM_JacobianStep);
    QDomElement LM_iter = doc.createElement("LM_iter");
    LM_iter.appendChild(doc.createTextNode("50"));
    config.appendChild(LM_iter);

    QDomElement dx = doc.createElement("Dx");
    dx.appendChild(doc.createTextNode("1e-6"));
    config.appendChild(dx);
    QDomElement lambda = doc.createElement("Lambda");
    lambda.appendChild(doc.createTextNode("1e-8"));
    config.appendChild(lambda);

    QDomElement popsize = doc.createElement("Popsize");
    popsize.appendChild(doc.createTextNode("20"));
    config.appendChild(popsize);
    QDomElement ngen = doc.createElement("Ngen");
    ngen.appendChild(doc.createTextNode("50"));
    config.appendChild(ngen);
    QDomElement pmut = doc.createElement("Pmut");
    pmut.appendChild(doc.createTextNode("0.02"));
    config.appendChild(pmut);
    QDomElement pcross = doc.createElement("Pcross");
    pcross.appendChild(doc.createTextNode("0.9"));
    config.appendChild(pcross);
    QDomElement bin = doc.createElement("Bin");
    bin.appendChild(doc.createTextNode("32"));
    config.appendChild(bin);
    QDomElement gaalg = doc.createElement("Gaalg");
    gaalg.appendChild(doc.createTextNode("2"));
    config.appendChild(gaalg);
    QDomElement gascaling = doc.createElement("Gascaling");
    gascaling.appendChild(doc.createTextNode("3"));
    config.appendChild(gascaling);

    doc.appendChild(root);
    root.appendChild(config);
    root.appendChild(dut);

}

bool DUTconfig::changeConfig(curve cur){
    //config.firstChildElement("SpiceModel").firstChild()

    //config = getConfig();

    QDomElement spice = doc.createElement("Spice");
    spice.appendChild(doc.createTextNode(QString::fromStdString(cur.spice)));
    //qDebug()<<"TEST1 = "<<config.firstChildElement("Spice").text();
    //qDebug()<<"TEST1/2 = "<<config.firstChildElement("Spice").text();
    config.replaceChild(spice,config.firstChildElement("Spice"));
    //qDebug()<<"TEST2 = "<<config.firstChildElement("Spice").text();
    QDomElement accrcy = doc.createElement("Accrcy");
    accrcy.appendChild(doc.createTextNode(QString("%1").arg(cur.accrcy)));
    config.replaceChild(accrcy,config.firstChildElement("Accrcy"));
    QDomElement FTol = doc.createElement("FTol");
    FTol.appendChild(doc.createTextNode(QString("%1").arg(cur.FTol)));
    config.replaceChild(FTol, config.firstChildElement("FTol"));
    QDomElement Gtols = doc.createElement("Gtols");
    Gtols.appendChild(doc.createTextNode(QString("%1").arg(cur.Gtols)));
    config.replaceChild(Gtols,config.firstChildElement("Gtols"));
    QDomElement maxiter = doc.createElement("Maxiter");
    maxiter.appendChild(doc.createTextNode(QString("%1").arg(cur.maxiter)));
    config.replaceChild(maxiter,config.firstChildElement("Maxiter"));
    QDomElement backtrackiter = doc.createElement("Backtrackiter");
    backtrackiter.appendChild(doc.createTextNode(QString("%1").arg(cur.backtrackiter)));
    config.replaceChild(backtrackiter, config.firstChildElement("Backtrackiter"));
    QDomElement Feval = doc.createElement("Feval");
    Feval.appendChild(doc.createTextNode(QString("%1").arg(cur.Feval)));
    config.replaceChild(Feval,config.firstChildElement("Feval"));


    QDomElement LM_mu = doc.createElement("LM_mu");
    LM_mu.appendChild(doc.createTextNode(QString("%1").arg(cur.LM_mu)));
    config.replaceChild(LM_mu,config.firstChildElement("LM_mu"));
    QDomElement LM_epsilon1 = doc.createElement("LM_epsilon1");
    LM_epsilon1.appendChild(doc.createTextNode(QString("%1").arg(cur.LM_epsilon1)));
    config.replaceChild(LM_epsilon1,config.firstChildElement("LM_epsilon1"));
    QDomElement LM_epsilon2 = doc.createElement("LM_epsilon2");
    LM_epsilon2.appendChild(doc.createTextNode(QString("%1").arg(cur.LM_epsilon2)));
    config.replaceChild(LM_epsilon2,config.firstChildElement("LM_epsilon2"));
    QDomElement LM_epsilon3 = doc.createElement("LM_epsilon3");
    LM_epsilon3.appendChild(doc.createTextNode(QString("%1").arg(cur.LM_epsilon3)));
    config.replaceChild(LM_epsilon3,config.firstChildElement("LM_epsilon3"));
    QDomElement LM_JacobianStep = doc.createElement("LM_JacobianStep");
    LM_JacobianStep.appendChild(doc.createTextNode(QString("%1").arg(cur.LM_JacobianStep)));
    config.replaceChild(LM_JacobianStep,config.firstChildElement("LM_JacobianStep"));
    QDomElement LM_iter = doc.createElement("LM_iter");
    LM_iter.appendChild(doc.createTextNode(QString("%1").arg(cur.LM_iter)));
    config.replaceChild(LM_iter,config.firstChildElement("LM_iter"));

    QDomElement dx = doc.createElement("Dx");
    dx.appendChild(doc.createTextNode(QString("%1").arg(cur.dx)));
    config.replaceChild(dx,config.firstChildElement("Dx"));
    QDomElement lambda = doc.createElement("Lambda");
    lambda.appendChild(doc.createTextNode(QString("%1").arg(cur.lambda)));
    config.replaceChild(lambda, config.firstChildElement("Lambda"));

    QDomElement popsize = doc.createElement("Popsize");
    popsize.appendChild(doc.createTextNode(QString("%1").arg(cur.popsize)));
    config.replaceChild(popsize,config.firstChildElement("Popsize"));
    QDomElement ngen = doc.createElement("Ngen");
    ngen.appendChild(doc.createTextNode(QString("%1").arg(cur.ngen)));
    config.replaceChild(ngen,config.firstChildElement("Ngen"));
    QDomElement pmut = doc.createElement("Pmut");
    pmut.appendChild(doc.createTextNode(QString("%1").arg(cur.pmut)));
    config.replaceChild(pmut,config.firstChildElement("Pmut"));
    QDomElement pcross = doc.createElement("Pcross");
    pcross.appendChild(doc.createTextNode(QString("%1").arg(cur.pcross)));
    config.replaceChild(pcross, config.firstChildElement("Pcross"));
    QDomElement bin = doc.createElement("Bin");
    bin.appendChild(doc.createTextNode(QString("%1").arg(cur.bin)));
    config.replaceChild(bin,config.firstChildElement("Bin"));
    QDomElement gaalg = doc.createElement("Gaalg");
    gaalg.appendChild(doc.createTextNode(QString("%1").arg(cur.gaalg)));
    config.replaceChild(gaalg,config.firstChildElement("Gaalg"));
    QDomElement gascaling = doc.createElement("Gascaling");
    gascaling.appendChild(doc.createTextNode(QString("%1").arg(cur.gascaling)));
    config.replaceChild(gascaling, config.firstChildElement("Gascaling"));

    return true;
};
/*QDomElement newcurve = doc.createElement("curve"); //dev.toDocument().createElement("curve");
        newcurve.setAttribute("name", name);
        dev.replaceChild(newcurve, curve);*/

QDomNode DUTconfig::getDut(QDomNode root){
  dut = root.firstChild();
  while (!dut.isNull()) {
      if (dut.toElement().tagName()=="DUT") return dut.toElement();
      dut = dut.nextSibling();
  }
}

QDomNode DUTconfig::getConfig(QDomNode root){
  config = root.firstChild();
  while (!config.isNull()) {
      if (config.toElement().tagName()=="Config") return config.toElement();
      config = config.nextSibling();
  }
}

void DUTconfig::parseNode(const QDomElement &element,
                            QStandardItem *parent)
{
    QTextStream out(stdout);

    QString name;
    //мы уже знаем где находится DUT
    if (element.tagName()!="DUT"){
            name = element.attribute("name");            
    //мы уже знаем где находится DUT --- getDUT()
    }else {
        name=element.tagName();
    }

    QStandardItem *item = new QStandardItem(name);
    //qDebug()<<"DUTconfig::parseNode Item Name: "<<name<<"\n";

    parent->appendRow(item);
    QDomNode node = element.firstChild();

    while (!node.isNull()) {
        if (node.toElement().tagName() == "device"||node.toElement().tagName() == "curve") {
            //qDebug()<<"DUTconfig::parseNode node: "<<node.toElement().attribute("name")<<"\n";
             parseNode(node.toElement(), item);
        }
        node = node.nextSibling();
    }
}


bool DUTconfig::createDevice(QStandardItemModel* model, QModelIndex idx, QString name){
    //добавляем узел в DUT
    //QDomElement dev = getDevice(idx.data().toString());
    QDomElement dev = getDevice(name);
    //если прибор с таким именем не существует
    if(dev.attribute("name")!=name){
        QDomElement node = dut.toElement().toDocument().createElement("device");
        node.setAttribute("name", name);
        dut.appendChild(node);
        //qDebug()<<"Create child \n";//<<idx.data().toString()<<"\n";
        save();
        //проверим появился ли девайс
        if(!getDevice(name).isNull()){
            //добавляем потомка в дерево
            QStandardItem *child = new QStandardItem(name);
            QStandardItem *itm = model->itemFromIndex(idx);
            itm->insertRow(itm->rowCount(),child);

            //qDebug()<<"DUTconfig::createDevice getDevice("<<name<<") is not Null "<<getDevice(name).tagName()<<" parent = "<<getDevice(name).parentNode().toElement().tagName()<<"\n";
            return true;
        }else {
            qDebug()<<"Same-thing wrong\n";
        }
    }
    return false;
}


bool DUTconfig::createCurve(QStandardItemModel* model, QModelIndex idx, QString name, curve cur){
    int i=0;
    //добавляем узел в device
    //idx - название прибора
    QDomElement dev = getDevice(idx.data().toString());
    //qDebug()<<"Parent Name = "<<dev.attribute("name")<<"\n";
    QDomElement curve = getCurve(dev, name);
    //qDebug()<<"Curve Name "<<curve.attribute("name")<<"\n";

    //если ВАХ с таким именем не существует
    if(curve.attribute("name")!=name){
        //добавляем ВАХ
        //формируем ВАХ
        QDomElement newcurve = dev.toDocument().createElement("curve");
        newcurve.setAttribute("name", name);
        dev.appendChild(newcurve);
        //записываем алгоритм
        QDomElement alg = newcurve.toDocument().createElement("Alg");
        alg.appendChild(alg.toDocument().createTextNode(QString::fromStdString(cur.AlgOpt)));
        newcurve.appendChild(alg);
        //записываем путь к файлу параметрами модели
        config.firstChildElement("SpiceModel").removeChild(config.firstChildElement("SpiceModel").firstChild());
        config.firstChildElement("SpiceModel").appendChild(config.firstChildElement("SpiceModel").toDocument().createTextNode(QString::fromStdString(cur.model_path)));
        //qDebug()<<"DUTconfig::createCurve "<<QString::fromStdString(cur.model_path)<<"\n";

        //формируем данные - добавляем файлы и
        //имена параметров
        for(i=0;i<cur.inFile.size();i++){
            QDomElement file = newcurve.toDocument().createElement("file");
            file.setAttribute("num",QString::number(i+1));
            newcurve.appendChild(file);
            QDomElement cir = file.toDocument().createElement("Cir");
            //cir.toDocument().createTextNode(QString::fromStdString(cur.inFile.at(i)));
            cir.appendChild(cir.toDocument().createTextNode(QString::fromStdString(cur.inFile.at(i))));
            file.appendChild(cir);
            QDomElement out = file.toDocument().createElement("Out");
            out.appendChild(out.toDocument().createTextNode(QString::fromStdString(cur.outFile.at(i))));
            file.appendChild(out);
            QDomElement exp = file.toDocument().createElement("Exp");
            exp.appendChild(exp.toDocument().createTextNode(QString::fromStdString(cur.expFile.at(i))));
            file.appendChild(exp);
            //qDebug()<<"newcurve.appendChild(file);\n";
            //qDebug()<<newcurve.firstChildElement("file").attribute("num")<<"\n";
        }
        //добавляем названия параметров
        for(i=0;i<cur.spiceParam.size();i++){
            QDomElement spiceName = newcurve.toDocument().createElement("SpiceName");
            spiceName.appendChild(spiceName.toDocument().createTextNode(QString::fromStdString(cur.spiceParam.at(i))));
            newcurve.appendChild(spiceName);
            //теперь работаем со списком параметров и их значениями
            //в теге config
            //QString example = QObject::tr("Example");
            QDomElement sn = config.firstChildElement("SpiceParam");
            bool done=false;
            while(!sn.isNull()){
                if(sn.attribute("Name")==QString::fromStdString(cur.spiceParam.at(i))){
                    sn.removeChild(sn.firstChildElement("MinVal"));
                    sn.removeChild(sn.firstChildElement("InitVal"));
                    sn.removeChild(sn.firstChildElement("MaxVal"));

                    QDomNode min = sn.toDocument().createElement("MinVal");
                    min.appendChild(min.toDocument().createTextNode(QString::fromStdString(cur.spiceMin.at(i))));
                    sn.appendChild(min);
                    QDomNode init = sn.toDocument().createElement("InitVal");
                    init.appendChild(init.toDocument().createTextNode(QString::fromStdString(cur.spiceInit.at(i))));
                    sn.appendChild(init);
                    QDomNode max = sn.toDocument().createElement("MaxVal");
                    max.appendChild(max.toDocument().createTextNode(QString::fromStdString(cur.spiceMax.at(i))));
                    sn.appendChild(max);
                    done=true;

                }
                sn=sn.nextSiblingElement("SpiceParam");
            }
            //просто удалим существующий элемент и моздадим новый

            //если параметра с таким именеи нету,
            //то надо его добавить
            if(!done){
                QDomElement param = config.toElement().toDocument().createElement("SpiceParam");
                param.toDocument().createAttribute("Name");
                param.setAttribute("Name",QString::fromStdString(cur.spiceParam.at(i)));
                config.toElement().appendChild(param);

                QDomNode min = param.toDocument().createElement("MinVal");
                min.appendChild(min.toDocument().createTextNode(QString::fromStdString(cur.spiceMin.at(i))));
                param.appendChild(min);
                QDomNode init = param.toDocument().createElement("InitVal");
                init.appendChild(init.toDocument().createTextNode(QString::fromStdString(cur.spiceInit.at(i))));
                param.appendChild(init);
                QDomNode max = param.toDocument().createElement("MaxVal");
                max.appendChild(max.toDocument().createTextNode(QString::fromStdString(cur.spiceMax.at(i))));
                param.appendChild(max);
            }
         }
        //вроде все добавили)
        //save();        
        //добавляем параметры экстракции, относящиеся к curve
        // ErrorFunction=2; и OutParse="{1};{2}";
        config.firstChildElement("OutParse").removeChild(config.firstChildElement("OutParse")).firstChild();
        //config.firstChildElement("OutParse").appendChild(config.firstChildElement("OutParse").toDocument().createTextNode(QString::fromStdString(cur.OutParse)));
        config.firstChildElement("OutParse").appendChild(config.firstChildElement("OutParse").toDocument().createTextNode("{1};{2}"));
        config.firstChildElement("ErrorFunction").removeChild(config.firstChildElement("ErrorFunction")).firstChild();
        //config.firstChildElement("ErrorFunction").appendChild(config.firstChildElement("ErrorFunction").toDocument().createTextNode(QString("%1").arg(QString::fromStdString(cur.OutParse))));
        config.firstChildElement("ErrorFunction").appendChild(config.firstChildElement("ErrorFunction").toDocument().createTextNode(QString("%1").arg(2)));
        //qDebug()<<"QStandardItem *child = new QStandardItem("<<name<<");\n";
        //добавляем элемент в tree
        QStandardItem *child = new QStandardItem(name);
        QStandardItem *itm = model->itemFromIndex(idx);
        itm->insertRow(itm->rowCount(),child);        
        return true;
    }else return false;
    //return false;
}

bool DUTconfig::changeCurve(QModelIndex idx, curve cur){
    try{
    int i=0;
    //добавляем узел в device
    //idx - название прибора
    QDomElement dev = getDevice(idx.parent().data().toString());
    //qDebug()<<"Parent Name = "<<dev.attribute("name")<<"\n";
    QDomElement curve = getCurve(dev, idx.data().toString());
    //qDebug()<<"Curve Name "<<curve.attribute("name")<<"\n";

    QString name = idx.data().toString();
    //qDebug()<<"Name "<<name<<"\n";

    //если ВАХ с таким именем существует
    //if(curve.attribute("name")==name){
        //Не добавляем, а заменяем
        //формируем ВАХ
        QDomElement newcurve = doc.createElement("curve"); //dev.toDocument().createElement("curve");
        newcurve.setAttribute("name", name);
        dev.replaceChild(newcurve, curve);
        //qDebug()<<"after Replace";
        //записываем алгоритм
        QDomElement alg = doc.createElement("Alg"); //newcurve.toDocument().createElement("Alg");
        //alg.appendChild(alg.toDocument().createTextNode(QString::fromStdString(cur.AlgOpt)));
        alg.appendChild(doc.createTextNode(QString::fromStdString(cur.AlgOpt)));
        newcurve.appendChild(alg);
        //добавляем параметры экстракции, относящиеся к curve
        // ErrorFunction=2; и OutParse="{1};{2}";
        QDomElement OutParse = doc.createElement("OutParse");
        OutParse.appendChild(doc.createTextNode(QString::fromStdString(cur.OutParse)));
        newcurve.appendChild(OutParse);
        //qDebug()<<"changeCurveInfo OutParse = "<<QString::fromStdString(cur.OutParse);
        QDomElement ErrorFunction = doc.createElement("ErrorFunction");
        ErrorFunction.appendChild(doc.createTextNode(QString("%1").arg(cur.ErrorFunction)));
        newcurve.appendChild(ErrorFunction);
        //записываем путь к файлу параметрами модели
        config.firstChildElement("SpiceModel").removeChild(config.firstChildElement("SpiceModel").firstChild());
        config.firstChildElement("SpiceModel").appendChild(config.firstChildElement("SpiceModel").toDocument().createTextNode(QString::fromStdString(cur.model_path)));
        //qDebug()<<"DUTconfig::changeCurve "<<QString::fromStdString(cur.model_path)<<"\n";

        //формируем данные - добавляем файлы и
        //имена параметров
        for(i=0;i<cur.inFile.size();i++){
            //QDomElement file = newcurve.toDocument().createElement("file");
            QDomElement file = doc.createElement("file");
            file.setAttribute("num",QString::number(i+1));
            newcurve.appendChild(file);

            //QDomElement cir = file.toDocument().createElement("Cir");
            QDomElement cir = doc.createElement("Cir");
            //cir.appendChild(cir.toDocument().createTextNode(QString::fromStdString(cur.inFile.at(i))));
            cir.appendChild(doc.createTextNode(QString::fromStdString(cur.inFile.at(i))));
            file.appendChild(cir);            
            //qDebug()<<"cir.text() : "<<(cir.text())<<" cur.inFile.at(i): "<<QString::fromStdString(cur.inFile.at(i));

            //QDomElement out = file.toDocument().createElement("Out");
            QDomElement out = doc.createElement("Out");
            //out.appendChild(out.toDocument().createTextNode(QString::fromStdString(cur.outFile.at(i))));
            out.appendChild(doc.createTextNode(QString::fromStdString(cur.outFile.at(i))));
            file.appendChild(out);
            //qDebug()<<"out.text() : "<<(out.text())<<" cur.outFile.at(i): "<<QString::fromStdString(cur.outFile.at(i));

            //QDomElement exp = file.toDocument().createElement("Exp");
            QDomElement exp = doc.createElement("Exp");
            //exp.appendChild(exp.toDocument().createTextNode(QString::fromStdString(cur.expFile.at(i))));
            exp.appendChild(doc.createTextNode(QString::fromStdString(cur.expFile.at(i))));
            file.appendChild(exp);
            //qDebug()<<newcurve.firstChildElement("file").attribute("num")<<"\n";
        }
        //qDebug()<<"after for(i=0;i<cur.inFile.size();i++)";
        //добавляем названия параметров
        for(i=0;i<cur.spiceParam.size();i++){
            //QDomElement spiceName = newcurve.toDocument().createElement("SpiceName");
            QDomElement spiceName = doc.createElement("SpiceName");
            //spiceName.appendChild(spiceName.toDocument().createTextNode(QString::fromStdString(cur.spiceParam.at(i))));
            spiceName.appendChild(doc.createTextNode(QString::fromStdString(cur.spiceParam.at(i))));
            newcurve.appendChild(spiceName);
            //теперь работаем со списком параметров и их значениями
            //в теге config
            //QString example = QObject::tr("Example");
            QDomElement sn = config.firstChildElement("SpiceParam");
            bool done=false;
            while(!sn.isNull()){
                if(sn.attribute("Name")==QString::fromStdString(cur.spiceParam.at(i))){
                    sn.removeChild(sn.firstChildElement("MinVal"));
                    sn.removeChild(sn.firstChildElement("InitVal"));
                    sn.removeChild(sn.firstChildElement("MaxVal"));

                    //QDomNode min = sn.toDocument().createElement("MinVal");
                    QDomNode min = doc.createElement("MinVal");
                    //qDebug()<<"min.appenfChild: "<<QString::fromStdString(cur.spiceMin.at(i));
                    //min.appendChild(min.toDocument().createTextNode(QString::fromStdString(cur.spiceMin.at(i))));
                    min.appendChild(doc.createTextNode(QString::fromStdString(cur.spiceMin.at(i))));
                    sn.appendChild(min);
                    //QDomNode init = sn.toDocument().createElement("InitVal");
                    QDomNode init = doc.createElement("InitVal");
                    //init.appendChild(init.toDocument().createTextNode(QString::fromStdString(cur.spiceInit.at(i))));
                    init.appendChild(doc.createTextNode(QString::fromStdString(cur.spiceInit.at(i))));
                    sn.appendChild(init);
                    //QDomNode max = sn.toDocument().createElement("MaxVal");
                    QDomNode max = doc.createElement("MaxVal");
                    //max.appendChild(max.toDocument().createTextNode(QString::fromStdString(cur.spiceMax.at(i))));
                    max.appendChild(doc.createTextNode(QString::fromStdString(cur.spiceMax.at(i))));
                    sn.appendChild(max);
                    done=true;

                }
                sn=sn.nextSiblingElement("SpiceParam");
            }
            //qDebug()<<"after for(i=0;i<cur.spiceParam.size();i++)";
            //если параметра с таким именеи нету,
            //то надо его добавить
            if(!done){
                //QDomElement param = config.toElement().toDocument().createElement("SpiceParam");
                QDomElement param = doc.createElement("SpiceParam");
                param.toDocument().createAttribute("Name");
                param.setAttribute("Name",QString::fromStdString(cur.spiceParam.at(i)));
                config.toElement().appendChild(param);

                //QDomNode min = param.toDocument().createElement("MinVal");
                QDomNode min = doc.createElement("MinVal");
                //min.appendChild(min.toDocument().createTextNode(QString::fromStdString(cur.spiceMin.at(i))));
                min.appendChild(doc.createTextNode(QString::fromStdString(cur.spiceMin.at(i))));
                param.appendChild(min);

                //QDomNode init = param.toDocument().createElement("InitVal");
                QDomNode init = doc.createElement("InitVal");
                //init.appendChild(init.toDocument().createTextNode(QString::fromStdString(cur.spiceInit.at(i))));
                init.appendChild(doc.createTextNode(QString::fromStdString(cur.spiceInit.at(i))));
                param.appendChild(init);

                //QDomNode max = param.toDocument().createElement("MaxVal");
                QDomNode max = doc.createElement("MaxVal");
                //max.appendChild(max.toDocument().createTextNode(QString::fromStdString(cur.spiceMax.at(i))));
                max.appendChild(doc.createTextNode(QString::fromStdString(cur.spiceMax.at(i))));
                param.appendChild(max);
            }
         }
    }catch(...){
        qDebug()<<"DUTconfig::changeCurve catched";
    }

        //вроде все добавили)    
        save();
}

bool DUTconfig::remDev(QStandardItemModel* model, QModelIndex idx){
    QDomElement dev = getDevice(idx.data().toString());
    //qDebug()<<"dev: "<<dev.tagName()<<" "<<dev.attribute("name")<<"\n";
    //если прибор с таким именем существует
    if(!dev.isNull()){
        //удаляем прибор их XML дерева
        QDomNode node = dev.parentNode().firstChild();
        if(node.isNull()) qDebug()<<"NODE IS NULL\n";
            while (!node.isNull()) {
                //qDebug()<<"Attr Name: "<<node.toElement().attribute("name")<<"\n";
                if (node.toElement().attribute("name")==QString(idx.data().toString())){
                    //qDebug()<<"NAMEs is th SAME: "<<node.toElement().attribute("name")<<"\n";
                    //node.parentNode().toDocument().removeChild(dev);
                    node.parentNode().removeChild(dev); //removes
                    break;
                }
                node = node.nextSibling();
            }
            if(dev.parentNode().isNull()){
                //после того как убедились, что устройство удалено их XML
                //удаляем его из дерева
                //удаляем прибор из дерева
                QStandardItem *parent = model->itemFromIndex(idx.parent());
                for(int i=0; i<parent->rowCount();i++){
                    //qDebug()<<"Parent Data Name:"<<idx.parent().child(i,0).data().toString()<<"\n";
                    if(idx.parent().child(i,0).data().toString()==idx.data().toString()){
                        parent->removeRow(i);
                    }
                }
                qDebug()<<"Dev Is NULL"<<"\n";
            }else{               
                qDebug()<<"Some-thing Wrong(...\n";
                return false;
            }
    }
    //save();//сохраняем пока прото так
    //qDebug()<<"Parent IDX DATA: "<<idx.parent().data().toString()<<"\n";
    return true;
}

bool DUTconfig::remCurve(QStandardItemModel* model, QModelIndex idx){

    QDomElement dev = getDevice(idx.parent().data().toString());
    //qDebug()<<"Parent Name = "<<dev.attribute("name")<<"\n";
    QDomElement curve = getCurve(dev, idx.data().toString());

    //qDebug()<<"dev: "<<dev.tagName()<<" "<<dev.attribute("name")<<"\n";
    //если ВАХ с таким именем существует
    if(!curve.isNull()){
        //удаляем ВАХ их XML дерева
        QDomNode node = curve.parentNode().firstChild();
        //if(node.isNull()) qDebug()<<"NODE IS NULL\n";
            while (!node.isNull()) {
                //qDebug()<<"Attr Name: "<<node.toElement().attribute("name")<<"\n";
                if (node.toElement().attribute("name")==QString(idx.data().toString())){
                    //qDebug()<<"NAMEs is th SAME: "<<node.toElement().attribute("name")<<"\n";
                    node.parentNode().removeChild(curve); //removes
                    break;
                }
                node = node.nextSibling();
            }
            if(curve.parentNode().isNull()){
                //после того как убедились, что устройство удалено их XML
                //удаляем его из дерева
                //удаляем прибор из дерева
                QStandardItem *parent = model->itemFromIndex(idx.parent());
                for(int i=0; i<parent->rowCount();i++){
                    //qDebug()<<"Parent Data Name:"<<idx.parent().child(i,0).data().toString()<<"\n";
                    if(idx.parent().child(i,0).data().toString()==idx.data().toString()){
                        parent->removeRow(i);
                    }
                }
                qDebug()<<"cur Is NULL"<<"\n";
            }else{
                //qDebug()<<"cur Is NOT NULL "<<dev.attribute("name")<<"\n";
                //qDebug()<<dev.parentNode().toElement().attribute("name")<<" TagName = "
                  //      <<dev.parentNode().toElement().tagName()<<"\n";
                qDebug()<<"Some-thing Wrong(...\n";
                return false;
            }
    }
    //save();//сохраняем пока прото так
    //qDebug()<<"Parent IDX DATA: "<<idx.parent().data().toString()<<"\n";
    return true;
}

//создаем дерево из приборов и их характеристик
void DUTconfig::getTree(QStandardItem *item){
    QTextStream out(stdout);
    
    //qDebug()<<"DUTconfig::getTree DUT is: "<<dut.toElement().tagName()<<"\n";
    parseNode(dut.toElement(), item);
}

QDomElement DUTconfig::getDevice(QString name){
//список приборов
    QDomElement child = dut.toElement().firstChildElement("device");
    while(!child.isNull()){
        //out<<"Child: "<<child.attribute("name")<<"\n";
        if(child.attribute("name")==name){
        //нашли наш прибор
        return child;
        break;
        }
        child=child.nextSiblingElement("device");
    }
    return QDomElement();
}

QDomElement DUTconfig::getCurve(QDomElement parent, QString name){
QDomElement child=parent.firstChildElement("curve");
    //QDomElement child=child.firstChildElement("curve");
    while(!child.isNull()){
        if(child.attribute("name")==name){
            //нашли нашу характеристику
        return child;
        }
        child=child.nextSiblingElement("curve");
    }
    return QDomElement();
}

//формируем структулу с инфой о конкретном шаге экстракции
curve DUTconfig::getCurveInfo(QModelIndex cur){
    curve res;
    QTextStream out(stdout);

    //ищем переданный device и его curve
    //пока device:
    QString dev_str = cur.parent().data().toString();
    QString cur_str = cur.data().toString();

    //qDebug()<<"dev_str: "<<dev_str<<" cur_str: "<<cur_str;
    //список приборов
    QDomElement child = dut.toElement().firstChildElement("device");
    while(!child.isNull()){
        //out<<"Child: "<<child.attribute("name")<<"\n";
        if(child.attribute("name")==dev_str){
        //нашли наш прибор
        break;
        }
        child=child.nextSiblingElement("device");
    }

    //список характеристик требуемого прибора
    child=child.firstChildElement("curve");

    while(!child.isNull()){
        if(child.attribute("name")==cur_str){
            //нашли нашу характеристику
        break;
        }
        child=child.nextSiblingElement("curve");
    }

    //какой же алгоритм оптимизации используется?
    res.AlgOpt=child.firstChildElement("Alg").text().toStdString();
    //а где наша модель находится?
    res.model_path=getConfig(root).firstChildElement("SpiceModel").text().toStdString();
    //qDebug()<<"MODEL PATH: "<<getConfig(root).firstChildElement("SpiceModel").text()<<"\n";
    //qDebug()<<"child.firstChildElement(OutParse).text().toStdString() = "<<child.firstChildElement("OutParse").text();

    res.OutParse=child.firstChildElement("OutParse").text().toStdString();
    if(QString::fromStdString(res.OutParse)=="")
        res.OutParse=QString("{1};{2}").toStdString();
    //qDebug()<<"getCurveInfo OutParse = "<<QString::fromStdString(res.OutParse);
    QString tErrorFunc = child.firstChildElement("ErrorFunction").text();
    if(tErrorFunc=="")
        res.ErrorFunction=2;
    else res.ErrorFunction=tErrorFunc.toInt();

    //начинаем работать с файлами для нашей х-ки
    //cir out exp
    QDomElement inf = child.firstChildElement("file");    
    while(!inf.isNull()){
        //qDebug()<<inf.attribute("num")<<"\n";

        res.inFile.push_back(inf.firstChildElement("Cir").text().toStdString());
        //qDebug()<<"inf.firstChildElement(Cir).text().toStdString(): "<<inf.firstChildElement("Cir").text()<<"\n";
        res.outFile.push_back(inf.firstChildElement("Out").text().toStdString());
        res.expFile.push_back(inf.firstChildElement("Exp").text().toStdString());

        inf = inf.nextSiblingElement("file");
    }

    //теперь запишем название SPICE параметров...
    inf=child.firstChildElement("SpiceName");
    while(!inf.isNull()){        
        //out<<inf.toElement().text()<<"\n";
        res.spiceParam.push_back(inf.text().toStdString());
        
        //... и их значения (mni, init, max)
        QDomElement spice = config.firstChildElement("SpiceParam");
        while(!spice.isNull()){
            if(spice.attribute("Name")==inf.text()){
                //qDebug()<<"SpiceName: "<<inf.text();
                res.spiceInit.push_back((std::string)spice.firstChildElement("InitVal").text().toStdString());
                res.spiceMax.push_back((std::string)spice.firstChildElement("MaxVal").text().toStdString());
                res.spiceMin.push_back((std::string)spice.firstChildElement("MinVal").text().toStdString());
                res.spiceOldVal.push_back((std::string)spice.firstChildElement("InitVal").text().toStdString());
            }
            spice=spice.nextSiblingElement("SpiceParam");
        }
        inf = inf.nextSiblingElement("SpiceName");
    }   
  //вроде всю необходимую инфу собрали
    //нет, еще не всю
    //запишем еще и тндекс. Так, на всякий)
    res.idx = cur;    
    getGlobalParams(&res);

  //возвращаем структуру в MainWindow
    //save();
  return res;
}

void DUTconfig::getGlobalParams(curve* res){
    //возвращаем глобальные параметры
    res->spice = QString(config.firstChildElement("Spice").text()).toStdString();
    res->accrcy = QString(config.firstChildElement("Accrcy").text()).toDouble();
    res->FTol = QString(config.firstChildElement("FTol").text()).toDouble();
    res->Gtols = QString(config.firstChildElement("Gtols").text()).toDouble();
    res->maxiter = QString(config.firstChildElement("Maxiter").text()).toInt();
    res->backtrackiter = QString(config.firstChildElement("Backtrackiter").text()).toInt();
    res->Feval = QString(config.firstChildElement("Feval").text()).toInt();

    res->LM_mu = QString(config.firstChildElement("LM_mu").text()).toDouble();
    res->LM_epsilon1 = QString(config.firstChildElement("LM_epsilon1").text()).toDouble();
    res->LM_epsilon2 = QString(config.firstChildElement("LM_epsilon2").text()).toDouble();
    res->LM_epsilon3 = QString(config.firstChildElement("LM_epsilon3").text()).toDouble();
    res->LM_JacobianStep = QString(config.firstChildElement("LM_JacobianStep").text()).toDouble();
    res->LM_iter = QString(config.firstChildElement("LM_iter").text()).toInt();

    res->dx = QString(config.firstChildElement("Dx").text()).toDouble();
    res->lambda = QString(config.firstChildElement("Lambda").text()).toDouble();

    res->popsize = QString(config.firstChildElement("Popsize").text()).toInt();
    res->ngen = QString(config.firstChildElement("Ngen").text()).toInt();
    res->pmut = QString(config.firstChildElement("Pmut").text()).toDouble();
    res->pcross = QString(config.firstChildElement("Pcross").text()).toDouble();
    res->bin = QString(config.firstChildElement("Bin").text()).toInt();
    res->gaalg = QString(config.firstChildElement("Gaalg").text()).toInt();
    res->gascaling = QString(config.firstChildElement("Gascaling").text()).toInt();
}

bool DUTconfig::save(){
    //qDebug()<<"DUTconfig::save()\n";
QFile file( "tmp4log_config.xml" );
file.remove("tmp4log_config.xml");
if( !file.open( QIODevice::ReadWrite ) )
    return false;
  QTextStream ts( &file );
  //qDebug()<<"QTextStream ts( &file );\n";
  //qDebug()<<doc.toString();
  ts << doc.toString();
  qDebug()<<"tmp tmp4log_config.xml saved!\n";
  file.close();
  return true;
}

bool DUTconfig::save(QString filename){
    qDebug()<<"DUTconfig::save(QString file)\n";
    /*QTextStream out(&file);
    doc.save(out, 4);*/
    QFile file( filename );
    file.remove(filename);
    if( !file.open( QIODevice::ReadWrite ) )
        return false;
      QTextStream ts( &file );
      qDebug()<<"QTextStream ts( &file );\n";
      //qDebug()<<doc.toString();
      ts << doc.toString();
      qDebug()<<"ts << doc.toString();\n";
      file.close();
    return true;
}

/*void DUTconfig::modelpathchanged(){
}*/

QVector<QString> DUTconfig::getSpiceVals(QString name){
    QVector<QString>res;
    //значения Spice параметров (mni, init, max)
    QDomElement spice = config.firstChildElement("SpiceParam");
    while(!spice.isNull()){
        if(spice.attribute("Name")==name){
            res.append(spice.firstChildElement("MinVal").text());
            res.append(spice.firstChildElement("InitVal").text());
            res.append(spice.firstChildElement("MaxVal").text());
        }
        spice=spice.nextSiblingElement("SpiceParam");
    }
    return res;
}
