#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    srand(0);
}

double MainWindow::Function(double x){
    double y = x * sin(x);
    return y;
}

double MainWindow::Function(unsigned short shortX){
    double x = double(shortX)*(maxX - minX)/USHRT_MAX + minX;
    double y = double(5) - double(24)*x + double(17)*x*x - double(11)/double(3)*x*x*x + double(1)/double(4)*x*x*x*x;
    return y;
}

QVector<double> MainWindow::getNormVector(QVector<unsigned short> &shortVector){
    QVector<double> result (shortVector.length());
    for(int i = 0; i < shortVector.length(); i++)
        result[i] =  double(shortVector[i])*(maxX - minX)/USHRT_MAX + minX;
    return result;
}

QVector<double> MainWindow::getFitnessVector(QVector<double> &x){
    QVector<double> result(x.length());
    for(int i = 0; i < x.length(); i++)
        result[i] = Function(x[i]);
    return result;
}
void MainWindow::on_pbShowGraph_clicked()
{
    maxX = 8;
    minX = -1;
    QVector<unsigned short> shortX (USHRT_MAX + 1);
    for(int i = 0; i < shortX.length(); i++)
        shortX[i] = i;
    QVector<double> x = getNormVector(shortX);
    QVector<double> y = getFitnessVector(x);

    ui->cpGraph->clearGraphs();
    ui->cpGraph->addGraph();
    ui->cpGraph->graph(0)->setData(x, y);
    ui->cpGraph->graph(0)->setPen(QColor(0,0,255,255));
    ui->cpGraph->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->cpGraph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    ui->cpGraph->xAxis->setLabel("x");
    ui->cpGraph->yAxis->setLabel("y");
    ui->cpGraph->xAxis->setRange(minX - 1, maxX + 1);//Для оси Ox
    ui->cpGraph->yAxis->setRange(-10, 100);//Для оси Oy
    ui->cpGraph->replot();
}

void MainWindow::on_pbCreateGeneration_clicked()
{
    chromosomeAmount = 10;
    firstGeneration.resize(chromosomeAmount);
    for(int i = 0; i < firstGeneration.length(); i++)
        firstGeneration[i] = rand() % USHRT_MAX;

    QVector<double> firstGenX = getNormVector(firstGeneration);
    QVector<double> firstGenY = getFitnessVector(firstGenX);


    if(ui->cpGraph->graphCount() > 1)
        ui->cpGraph->removeGraph(1);
    ui->cpGraph->addGraph();
    ui->cpGraph->graph(1)->setData(firstGenX, firstGenY);
    ui->cpGraph->graph(1)->setPen(QColor(255,0,0,255));
    ui->cpGraph->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->cpGraph->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 6));

    ui->cpGraph->replot();
}

void MainWindow::on_pbStartProcess_clicked()
{
    if(ui->cpGraph->graphCount() < 2)
        return;

    bool isMax = false;
    bool isContinue = true;
    int crossoverPlaceAmount = 3;
    QVector<unsigned short> processGeneration;

    //init process generation with max generation or min generation
    if(isMax){
        if(maxGeneration.isEmpty() || !isContinue)
            maxGeneration = firstGeneration;
        processGeneration = maxGeneration;
    }
    else{
        if(minGeneration.isEmpty() || !isContinue)
            minGeneration = firstGeneration;
        processGeneration = minGeneration;
    }

    //fill first generation to the first part of sequence, another part is for childs
    twoGenerations.resize(2 * processGeneration.length());
    for(int i = 0; i < processGeneration.length(); i++)
        twoGenerations[i] = processGeneration[i];

    for(int i = 0; i < processGeneration.length(); i++)
        twoGenerations[processGeneration.length() + i] = 0;

    for(int g = 0; g < processGeneration.length(); g++){
        
        QVector<int> crossoverPlaces(crossoverPlaceAmount);
        for(int i = 0; i < crossoverPlaces.length(); i++)
            crossoverPlaces[i] = rand() % USHRT_MAX;
        
        //sorting
        for(int j = 0; j < crossoverPlaces.length(); j++){
            bool check = true;
            for(int i = 1; i < crossoverPlaces.length(); i++)
                if(crossoverPlaces[i] > crossoverPlaces[i - 1]){
                    int temp = crossoverPlaces[i];
                    crossoverPlaces[i] = crossoverPlaces[i - 1];
                    crossoverPlaces[i - 1] = temp;
                    check = false;
                }
                else if(crossoverPlaces[i] == crossoverPlaces[i - 1])
                    crossoverPlaces.removeAt(i);
            if(check)
                break;
        }
        
        int father = rand() % processGeneration.length();
        int mother = rand() % processGeneration.length();

        if(crossoverPlaces.length() > 0){

            QVector<unsigned short> fatherParts(crossoverPlaces.length() + 1);
            QVector<unsigned short> motherParts(crossoverPlaces.length() + 1);

            for(int i = 0; i < fatherParts.length(); i++){
                fatherParts[i] = 0;
                motherParts[i] = 0;
            }

            unsigned short fatherTemp = processGeneration[father];
            unsigned short motherTemp = processGeneration[mother];

            //1111|11111 -> 0000|01111 -> 1111|00000
            fatherParts[0] = (fatherTemp >> (USHRT_MAX - crossoverPlaces[0])) << (USHRT_MAX - crossoverPlaces[0]);
            motherParts[0] = (motherTemp >> (USHRT_MAX - crossoverPlaces[0])) << (USHRT_MAX - crossoverPlaces[0]);

            for(int i = 1; i < crossoverPlaces.length(); i++){
                //11|11|11111 -> 00|00|01111 -> 11|11|00000 -> 11|00|00000 -> 00|11|00000
                fatherParts[i] = (((fatherTemp >> (USHRT_MAX - crossoverPlaces[i])) << (USHRT_MAX - crossoverPlaces[i]))
                                        << crossoverPlaces[i - 1]) >> crossoverPlaces[i - 1];
                motherParts[i] = (((motherTemp >> (USHRT_MAX - crossoverPlaces[i])) << (USHRT_MAX - crossoverPlaces[i]))
                                        << crossoverPlaces[i - 1]) >> crossoverPlaces[i - 1];
            }

            //crossingover
            for(int i = 0; i < fatherParts.length(); i++)
                if(i % 2 == 0)
                    twoGenerations[processGeneration.length() + g] |= fatherParts[i];
                else
                    twoGenerations[processGeneration.length() + g] |= motherParts[i];
        }
    }

    //sorting
    for(int j = 0; j < twoGenerations.length(); j++){
        bool check = true;
        for(int i = 1; i < twoGenerations.length(); i++)
            if(Function(twoGenerations[i]) > Function(twoGenerations[i - 1])){
                unsigned short temp = twoGenerations[i];
                twoGenerations[i] = twoGenerations[i - 1];
                twoGenerations[i - 1] = temp;
                check = false;
            }
        if(check)
            break;
    }

    if(isMax)
        for(int i = 0; i < maxGeneration.length(); i++){
            maxGeneration[i] = twoGenerations[i];
            processGeneration[i] = maxGeneration[i];
        }
    else
        for(int i = 0; i < minGeneration.length(); i++){
            minGeneration[i] = twoGenerations[twoGenerations.length() - minGeneration.length() + i];
            processGeneration[i] = minGeneration[i];
        }

    QVector<double> childGenX = getNormVector(processGeneration);
    QVector<double> childGenY = getFitnessVector(childGenX);

    if(ui->cpGraph->graphCount() > 2)
        ui->cpGraph->removeGraph(2);
    ui->cpGraph->addGraph();
    ui->cpGraph->graph(2)->setData(childGenX, childGenY);
    ui->cpGraph->graph(2)->setPen(QColor(0,255,0,255));
    ui->cpGraph->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->cpGraph->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 6));
    ui->cpGraph->replot();

//    QVector<double> twoGenX = getNormVector(twoGenerations);
//    QVector<double> twoGenY = getFitnessVector(twoGenX);

//    if(ui->cpGraph->graphCount() > 2)
//        ui->cpGraph->removeGraph(2);
//    ui->cpGraph->addGraph();
//    ui->cpGraph->graph(2)->setData(twoGenX, twoGenY);
//    ui->cpGraph->graph(2)->setPen(QColor(255,255,0,255));
//    ui->cpGraph->graph(2)->setLineStyle(QCPGraph::lsNone);
//    ui->cpGraph->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCrossSquare, 6));
//    ui->cpGraph->replot();
}
