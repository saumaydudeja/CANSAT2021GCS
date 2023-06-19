#include "mainwindow.h"
#include <QtWidgets/QMainWindow>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QLCDNumber>
#include <QApplication>
#include <QLabel>
#include <gmap.h>
#include <bar.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    gmap locator;

    w.show();
    return a.exec();
}
