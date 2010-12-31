#include "optparamseditor.h"
#include "ui_optparamseditor.h"

OptParamsEditor::OptParamsEditor(QWidget *parent, curve *cur) :
    QWidget(parent),
    ui(new Ui::OptParamsEditor)
{
    ui->setupUi(this);
    ui->OPtGlobPar_tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Name"));
    ui->OPtGlobPar_tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Val"));
    ui->OPtGlobPar_tableWidget->horizontalHeader()->setResizeMode( QHeaderView::Stretch);
    ui->OPtGlobPar_tableWidget->setRowCount(25);

    this->cur=cur;

    //заполняем таблицу параметрами... вручную
    ui->OPtGlobPar_tableWidget->setItem(0,0,
                                        new QTableWidgetItem((QString("Simulator Path"))));
    //qDebug()<<"OPE CLASS"<<(QString::fromStdString(cur->spice));

    ui->OPtGlobPar_tableWidget->setItem(0,1,
                                        new QTableWidgetItem((QString::fromStdString(cur->spice))));
    ui->OPtGlobPar_tableWidget->setItem(1,0,
                                        new QTableWidgetItem((QString("Accurancy"))));
    ui->OPtGlobPar_tableWidget->setItem(1,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->accrcy))));
    ui->OPtGlobPar_tableWidget->setItem(2,0,
                                        new QTableWidgetItem((QString("Funcion Tolerance"))));
    ui->OPtGlobPar_tableWidget->setItem(2,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->FTol))));
    ui->OPtGlobPar_tableWidget->setItem(3,0,
                                        new QTableWidgetItem((QString("Gradient Tolerance"))));
    ui->OPtGlobPar_tableWidget->setItem(3,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->Gtols))));
    ui->OPtGlobPar_tableWidget->setItem(4,0,
                                        new QTableWidgetItem((QString("MaxIter"))));
    ui->OPtGlobPar_tableWidget->setItem(4,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->maxiter))));
    ui->OPtGlobPar_tableWidget->setItem(5,0,
                                        new QTableWidgetItem((QString("BacktrackIter"))));
    ui->OPtGlobPar_tableWidget->setItem(5,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->backtrackiter))));
    ui->OPtGlobPar_tableWidget->setItem(6,0,
                                        new QTableWidgetItem((QString("Function Evaluations"))));
    ui->OPtGlobPar_tableWidget->setItem(6,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->Feval))));

    ui->OPtGlobPar_tableWidget->setItem(7,0,
                                        new QTableWidgetItem((QString("LevMar"))));
    ui->OPtGlobPar_tableWidget->setItem(8,0,
                                        new QTableWidgetItem((QString("Scale Factor for Initial \mu"))));
    ui->OPtGlobPar_tableWidget->setItem(8,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->LM_mu))));
    ui->OPtGlobPar_tableWidget->setItem(9,0,
                                        new QTableWidgetItem((QString("Stopping Threshold for ||J^T e||_inf"))));
    ui->OPtGlobPar_tableWidget->setItem(9,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->LM_epsilon1))));
    ui->OPtGlobPar_tableWidget->setItem(10,0,
                                        new QTableWidgetItem((QString("Stopping Threshold for ||Dp||_2"))));
    ui->OPtGlobPar_tableWidget->setItem(10,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->LM_epsilon2))));
    ui->OPtGlobPar_tableWidget->setItem(11,0,
                                        new QTableWidgetItem((QString("Stopping Threshold for ||e||_2"))));
    ui->OPtGlobPar_tableWidget->setItem(11,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->LM_epsilon3))));
    ui->OPtGlobPar_tableWidget->setItem(12,0,
                                        new QTableWidgetItem((QString("Step for Approximation to the Jacobian"))));
    ui->OPtGlobPar_tableWidget->setItem(12,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->LM_JacobianStep))));
    ui->OPtGlobPar_tableWidget->setItem(13,0,
                                        new QTableWidgetItem((QString("LevMar Max Iteration Coutn"))));
    ui->OPtGlobPar_tableWidget->setItem(13,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->LM_iter))));

    ui->OPtGlobPar_tableWidget->setItem(14,0,
                                        new QTableWidgetItem((QString("GA"))));
    ui->OPtGlobPar_tableWidget->setItem(15,0,
                                        new QTableWidgetItem((QString("Population size"))));
    ui->OPtGlobPar_tableWidget->setItem(15,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->popsize))));
    ui->OPtGlobPar_tableWidget->setItem(16,0,
                                        new QTableWidgetItem((QString("Namber of generatopns"))));
    ui->OPtGlobPar_tableWidget->setItem(16,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->ngen))));
    ui->OPtGlobPar_tableWidget->setItem(17,0,
                                        new QTableWidgetItem((QString("Coefficient of Mutation"))));
    ui->OPtGlobPar_tableWidget->setItem(17,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->pmut))));
    ui->OPtGlobPar_tableWidget->setItem(18,0,
                                        new QTableWidgetItem((QString("Coefficient of Crossover"))));
    ui->OPtGlobPar_tableWidget->setItem(18,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->pcross))));
    ui->OPtGlobPar_tableWidget->setItem(19,0,
                                        new QTableWidgetItem((QString("Binary Width of Genotype"))));
    ui->OPtGlobPar_tableWidget->setItem(19,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->bin))));
    ui->OPtGlobPar_tableWidget->setItem(20,0,
                                        new QTableWidgetItem((QString("GA Alg"))));
    ui->OPtGlobPar_tableWidget->setItem(20,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->gaalg))));
    ui->OPtGlobPar_tableWidget->setItem(21,0,
                                        new QTableWidgetItem((QString("GA scaling"))));
    ui->OPtGlobPar_tableWidget->setItem(21,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->gascaling))));

    ui->OPtGlobPar_tableWidget->setItem(22,0,
                                        new QTableWidgetItem((QString("Other"))));
    ui->OPtGlobPar_tableWidget->setItem(23,0,
                                        new QTableWidgetItem((QString("Gradient Evaluation Step"))));
    ui->OPtGlobPar_tableWidget->setItem(23,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->dx))));
    ui->OPtGlobPar_tableWidget->setItem(24,0,
                                        new QTableWidgetItem((QString("Step Size Along Antigradient Direction"))));
    ui->OPtGlobPar_tableWidget->setItem(24,1,
                                        new QTableWidgetItem((QString("%1").arg(cur->lambda))));
}

OptParamsEditor::~OptParamsEditor()
{
    delete ui;
}

void OptParamsEditor::Edit(){
    ChangeCurveParams();
}
void OptParamsEditor::Cansel(){

}

void OptParamsEditor::ChangeCurveParams(){
    curve cur;
    for(int i=0;i<ui->OPtGlobPar_tableWidget->rowCount();i++){
        switch(i){
        case 0:
            cur.spice = ui->OPtGlobPar_tableWidget->item(0,1)->text().toStdString();
        case 1:
            cur.accrcy = ui->OPtGlobPar_tableWidget->item(1,1)->text().toDouble();
        case 2:
            cur.FTol = ui->OPtGlobPar_tableWidget->item(2,1)->text().toDouble();
        case 3:
            cur.Gtols = ui->OPtGlobPar_tableWidget->item(3,1)->text().toDouble();
        case 4:
            cur.maxiter = ui->OPtGlobPar_tableWidget->item(4,1)->text().toInt();
        case 5:
            cur.backtrackiter = ui->OPtGlobPar_tableWidget->item(5,1)->text().toInt();
        case 6:
            cur.Feval = ui->OPtGlobPar_tableWidget->item(6,1)->text().toDouble();
        case 8:
            cur.LM_mu = ui->OPtGlobPar_tableWidget->item(8,1)->text().toDouble();
        case 9:
            cur.LM_epsilon1 = ui->OPtGlobPar_tableWidget->item(9,1)->text().toDouble();
        case 10:
            cur.LM_epsilon2 = ui->OPtGlobPar_tableWidget->item(10,1)->text().toDouble();
        case 11:
            cur.LM_epsilon3 = ui->OPtGlobPar_tableWidget->item(11,1)->text().toDouble();
        case 12:
            cur.LM_JacobianStep = ui->OPtGlobPar_tableWidget->item(12,1)->text().toDouble();
        case 13:
            cur.LM_iter = ui->OPtGlobPar_tableWidget->item(13,1)->text().toInt();
        case 15:
            cur.popsize = ui->OPtGlobPar_tableWidget->item(15,1)->text().toInt();
        case 16:
            cur.ngen = ui->OPtGlobPar_tableWidget->item(16,1)->text().toInt();
        case 17:
            cur.pmut = ui->OPtGlobPar_tableWidget->item(17,1)->text().toDouble();
        case 18:
            cur.pcross = ui->OPtGlobPar_tableWidget->item(18,1)->text().toDouble();
        case 19:
            cur.bin = ui->OPtGlobPar_tableWidget->item(19,1)->text().toInt();
        case 20:
            cur.gaalg = ui->OPtGlobPar_tableWidget->item(20,1)->text().toInt();
        case 21:
            cur.gascaling = ui->OPtGlobPar_tableWidget->item(21,1)->text().toInt();
        case 23:
            cur.dx = ui->OPtGlobPar_tableWidget->item(23,1)->text().toDouble();
        case 24:
            cur.lambda = ui->OPtGlobPar_tableWidget->item(24,1)->text().toDouble();
        }
    }
    emit ChangeCurveParamsSignal(cur);
}
