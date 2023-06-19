#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<gmap.h>
#include <ledc.h>
#include <QMainWindow>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QQuickWidget>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QtQuick/QQuickView>
#include <QtMqtt/QtMqtt>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void connectSerial();
    void disconnectSerial();
    bool configureSerial();
    bool checkArduinoConnection();
    void plotdata();
    void writearduino(QString send);
    void setupmqtt();
    void updateled(QString state);
    void connectmqtt();
    void mqttpub(QStringList dat);
    void blinkled(ledc *led);

    ~MainWindow();




private slots:
    void readSerial();

    void sendsimp();

    void updatemqtt();

    void on_toolButton_7_clicked();


    void on_toolButton_5_clicked();


    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_6_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_8_clicked();

    void on_toolButton_4_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    bool cx = false;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;

//    static const quint16 arduino_uno_vendor_id = 6790;
//    static const quint16 arduino_uno_product_id = 29987;

    QString arduino_uno_port_name;
    QSerialPort *arduino;
    QTimer *timer;

    // for mqtt connection;
    QString topic = "teams/3394";
    bool mqttconnected=false;

    // for serial data collection
    QString serialBuffer;
    QByteArray serial;
    QString telemetry;
    QStringList sensorData;

    // simulation mode
//    bool simEnable = false;
//    bool simActivate = false;
//    bool getCsv = false;
//    QList<QString> simpList;
//    int simpIndex = 0;

    // payload variables
//    bool SP1X = false;
//    bool SP2X = false;

//    QVector<double> CAltitude, CVoltage, SP1Altitude, SP2Altitude, SP1Pressure, SP2Pressure, SP1Rotation, SP2Rotation, SP1TEMP, SP2TEMP;
//    double pressure;
//    QVector<double> graphTime;
//    QVector<double> GPSALTITUDE;
//    double count = 1;

    bool bytesavail;
    bool arduinoavail=false;

    QStringList plotTest;

    QVector<double> x;
    QVector<double> yd;
    QVector<double> yd1;
    QVector<double> yd2;
    QVector<double> yd3;
    QVector<double> yd4;
    QVector<double> yd5;
    QVector<double> yd6;
    QVector<double> yd7;
    QVector<double> yd8;
    QVector<double> yd9;
    QVector<double> ydf;
    QVector<double> ydf2;


    QFile f{"C:/maps/dat.txt"};

    QFile wr{"C:/maps/write.csv"};

    double l=0; //variable for graphs' x coordinate

//    QString timestring;
    QString track;
    QString timetele;
    QString cxtele{"CMD,3394,CX,"};
    QString simstring{"CMD,3394,SIM,"};
    QString px1string{"CMD,3394,SP1X,"};
    QString px2string{"CMD,3394,SP2X,"};
    bool px1enabled = false;
    bool px2enabled = false;
    QString simptele;
    QFile simpdata{"C:/maps/simpdata.csv"};
    QMqttClient *mqttclient;
    QString data;
    QString datasp1;
    QString datasp2;
    QString spdata;
    bool publishsp1=false;
    bool publishsp2=false;
    QString waste;
};
#endif // MAINWINDOW_H
