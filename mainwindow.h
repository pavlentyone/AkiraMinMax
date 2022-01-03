#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <math.h>
#include <random>
#include <QVector>
#include <QDebug>
#include <algorithm>
#include <QtMath>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbShowGraph_clicked();

    void on_pbCreateGeneration_clicked();

    void on_pbStartProcess_clicked();

private:
    Ui::MainWindow *ui;
    double maxX;
    double minX;
    double Function(double x);
    double Function(unsigned short x);
    QVector<double> getNormVector(QVector<unsigned short> &shortVector);
    QVector<double> getFitnessVector(QVector<double> &x);
    int chromosomeAmount;
    QVector<unsigned short> firstGeneration;
    QVector<unsigned short> minGeneration;
    QVector<unsigned short> maxGeneration;
    QVector<unsigned short> twoGenerations;
    int fromVector(QVector<bool> vec);
};

#endif // MAINWINDOW_H
