 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include <odometer.h>
#include <QPainter>
#include <QPointF>
#include <QLCDNumber>
#include <QtQuick/QQuickView>
#include <QtMqtt/QtMqtt>
#include <QQuickWidget>
#include <QtQuick/QQuickItem>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QtMath>
#include <QColor>
#include <QPixmap>
#include <QLineEdit>
#include <gmap.h>
#include <ledc.h>
#include <QQmlContext>
#include <QtMqtt/QtMqtt>
#include <QtMqtt/QMqttClient>
#include <bar.h>
#include <QFile>

gmap locator;


void MainWindow::updatemqtt()
{
    if(mqttclient->state()==QMqttClient::ClientState::Connected)

        mqttconnected=true;
    else
    {
        mqttconnected=false;
        qDebug()<<"mqtt error"<<mqttclient->error();
    }
    mqttconnected?ui->mqttconnled->toggle(1):ui->mqttconnled->toggle(0);
}

void MainWindow::blinkled(ledc *led)
{
//    QTimer *ledtimer = new QTimer(this);
//    ledtimer->setInterval(300);
//    ledtimer->setSingleShot(true);
    led->toggle(1);
    QTimer::singleShot(200, led, SLOT(turnoff()));// call a single shot timer to turn on the led for 300 ms , indicating that data has been published to mqtt broker
//    connect(ledtimer,SIGNAL(timeout()),led,SLOT(turnoff()));
//    ledtimer->start();

}

void MainWindow::updateled(QString state)
{
    if(state=="LAUNCH_WAIT")
    {
        ui->stateled->toggle(1);
        ui->stateled_2->toggle(0);
        ui->stateled_3->toggle(0);
        ui->stateled_4->toggle(0);
        ui->stateled_5->toggle(0);
        ui->stateled_6->toggle(0);
        ui->stateled_8->toggle(0);
    }
    if(state=="ASCENT")
    {
        ui->stateled->toggle(0);
        ui->stateled_2->toggle(1);
        ui->stateled_3->toggle(0);
        ui->stateled_4->toggle(0);
        ui->stateled_5->toggle(0);
        ui->stateled_6->toggle(0);
        ui->stateled_8->toggle(0);
    } if(state=="ROCKET_SEPERATION")
    {
        ui->stateled->toggle(0);
        ui->stateled_2->toggle(0);
        ui->stateled_3->toggle(1);
        ui->stateled_4->toggle(0);
        ui->stateled_5->toggle(0);
        ui->stateled_6->toggle(0);
        ui->stateled_8->toggle(0);
    } if(state=="DESCENT")
    {
        ui->stateled->toggle(0);
        ui->stateled_2->toggle(0);
        ui->stateled_3->toggle(0);
        ui->stateled_4->toggle(1);
        ui->stateled_5->toggle(0);
        ui->stateled_6->toggle(0);
        ui->stateled_8->toggle(0);
    } if(state=="5")
    {
        ui->stateled->toggle(0);
        ui->stateled_2->toggle(0);
        ui->stateled_3->toggle(0);
        ui->stateled_4->toggle(0);
        ui->stateled_5->toggle(1);
        ui->stateled_6->toggle(0);
        ui->stateled_8->toggle(0);
    } if(state=="LANDED")
    {
        ui->stateled->toggle(0);
        ui->stateled_2->toggle(0);
        ui->stateled_3->toggle(0);
        ui->stateled_4->toggle(0);
        ui->stateled_5->toggle(0);
        ui->stateled_6->toggle(0);
        ui->stateled_8->toggle(1);
    } if(state=="7")
    {
        ui->stateled->toggle(0);
        ui->stateled_2->toggle(0);
        ui->stateled_3->toggle(0);
        ui->stateled_4->toggle(0);
        ui->stateled_5->toggle(0);
        ui->stateled_6->toggle(1);
        ui->stateled_8->toggle(0);
    }


}

void MainWindow::setupmqtt()
{
    mqttclient->setProtocolVersion(QMqttClient::MQTT_3_1_1);
    mqttclient->setPort(1883);
//    mqttclient->setHostname("localhost");
    mqttclient->setHostname(QString("cansat.info"));
    mqttclient->setClientId("Team G.A.R.U.D.A");
    mqttclient->setUsername("3394");
    mqttclient->setPassword("Jeorgifa191!");
}

void MainWindow::connectmqtt()
{
    mqttclient->connectToHost();
    if(mqttclient->state()==QMqttClient::ClientState::Connected||mqttclient->state()==QMqttClient::ClientState::Connecting)

        mqttconnected=true;

    else
        qDebug()<<"mqtt error"<<mqttclient->error();

    mqttconnected?ui->mqttconnled->toggle(1):ui->mqttconnled->toggle(0);

}

void MainWindow::mqttpub(QStringList dat)
{
    publishsp1=false;
    publishsp2=false;
    if (dat.at(5)=="R")
    {
        publishsp1=true;
        datasp1=dat.at(19);
        for(int i=20;i<=25;i++)

            datasp1= datasp1+","+dat.at(i);
    }

    if(dat.at(6)=="R")
    {
        publishsp2=true;
        datasp2=dat.at(26);
        for(int i=27;i<=32;i++)
            datasp2=datasp2+","+dat.at(i);
    }

    data=dat.at(0);
    for(int i=1;i<=18;i++)
        data= data+","+dat.at(i);


    if(mqttclient->publish(topic,data.toUtf8())==-1)
       qDebug()<<"Error publishing";
    else
    {
    qDebug()<<"sent to mqtt broker"<<endl;
    blinkled(ui->mqttled);
    }

    if(publishsp1)
    {
        spdata=datasp1;
        if(mqttclient->publish(topic,spdata.toUtf8())==-1)
           qDebug()<<"Error publishing";
        else
        {
        qDebug()<<"sent to mqtt broker"<<endl;
        blinkled(ui->mqttled_2);
        }
    }

    if(publishsp2)
    {
        if(mqttclient->publish(topic,datasp2.toUtf8())==-1)
           qDebug()<<"Error publishing";
        else
        {
        qDebug()<<"sent to mqtt broker"<<endl;
        blinkled(ui->mqttled_3);
        }
    }
}

void MainWindow::sendsimp()
{
    qDebug()<<"starting sendsimp";
    simptele = simpdata.readLine();
    writearduino(simptele.toLocal8Bit());
    if(simpdata.atEnd())
    {

        qDebug()<<"Stopping SIMP timer";
        timer->stop();
    }
}

void init(QCustomPlot *graph)
{
    QColor bgcol = QColor(46,27,58);
    QColor graphbg = QColor(233,250,255);
    graph->addGraph();
    graph->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    graph->xAxis->setRange(0,300);
    graph->xAxis->setLabel("time");
    graph->xAxis->setLabelColor(bgcol);
    graph->yAxis->setLabelColor(bgcol);
    graph->setInteraction(QCP::iRangeDrag,true) ;
    graph->setInteraction(QCP::iRangeZoom,true) ;
    graph->axisRects().at(0)->setRangeZoom(Qt::Horizontal);
    graph->xAxis->setTickLabelColor(bgcol);
    graph->xAxis->setBasePen(QPen(bgcol));
    graph->yAxis->setTickLabelColor(bgcol);
    graph->yAxis->setBasePen(QPen(bgcol));
    graph->yAxis->grid()->setPen(Qt::NoPen);
    graph->xAxis->grid()->setPen(Qt::NoPen);
//    graph->graph(0)->setPen(QPen(bgcol));
    graph->setBackground(graphbg);
    graph->xAxis->grid()->setSubGridVisible(false);
    graph->yAxis->grid()->setSubGridVisible(false);

}

bool MainWindow::checkArduinoConnection()
{
    bool arduino_is_available = false;
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        //  check if the serialport has both a product identifier and a vendor identifier
        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
            //  check if the product ID and the vendor ID match those of the arduino uno
            if((serialPortInfo.productIdentifier() ==  arduino_uno_product_id)
                    && (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)){
                arduino_is_available = true; //    arduino uno is available on this port
                arduino_uno_port_name = serialPortInfo.portName();
            }

        }
    }
    return arduino_is_available;
}

bool MainWindow::configureSerial()
{
   if(checkArduinoConnection())
    {
    arduinoavail=true;
    qDebug() << "Found the arduino port...\n";
    arduino->setPortName(arduino_uno_port_name);
    arduino->open(QSerialPort::ReadWrite);
    arduino->setBaudRate(QSerialPort::Baud9600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setFlowControl(QSerialPort::NoFlowControl);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setReadBufferSize(5000);
    qDebug()<<"done configuring arduino";
    return arduinoavail;
    }
    else {
    arduinoavail=false;
    QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to arduino.");
    return arduinoavail;

    }
}

void MainWindow::writearduino(QString send)
{
    if(arduinoavail)
    {
        arduino->write(send.toLocal8Bit());
    }

}

void MainWindow::plotdata()
{
             ui->quickWidget->update();


             QString line = serial;
             QString echo;
             QByteArray buffer;
//             wr.seek(0);
             buffer.append(line.toUtf8());
//             wr.write(buffer);

             QStringList dd = line.split(",");

             ui->label_10->setText(dd.at(1));
             ui->label_16->setText(dd.at(1));
             ui->label_9->setText(dd.at(0));
             ui->label_15->setText(dd.at(3));
             //to determine fsw state during sp deployment
              if(dd.at(15)=="SP1_RELEASE")
              track="5";
              else if(dd.at(15)=="SP2_RELEASE")
              track="7";
              else
              track=dd.at(15);

             ui->label_22->setText(dd.at(7));
             ui->label_18->setText(QString(dd.at(18))); //adding up cmd echo parts(seperated by comma)
             ui->label_11->setText(dd.at(2));
             ui->label_19->setText(dd.at(14));
            yd.append(dd.at(23).toDouble()); // sp1 altitude
            yd1.append(dd.at(24).toDouble());// sp1 temp
            yd2.append(dd.at(25).toDouble()); //sp1 rotation
            yd3.append(dd.at(20).toDouble());
            yd4.append(dd.at(30).toDouble()); //sp2 altitude
            yd5.append(dd.at(31).toDouble()); //sp2 temp
            yd6.append(dd.at(32).toDouble()); //sp2 rotation
            yd8.append(dd.at(9).toDouble());//cont voltage
            yd9.append(dd.at(7).toDouble());//cont altitude
            ydf.append(101325*std::pow((1-(dd.at(23).toDouble()/44330)),5.33));
            ydf2.append(101325*std::pow((1-(dd.at(30).toDouble()/44330)),5.33));
            x.append(l++);

    updateled(track);//update fsw led
    locator.setData(dd.at(11),dd.at(12));

    ui->label_20->setText(dd.at(11));
    ui->label_21->setText(dd.at(12));

    ui->label_12->setText(dd.at(5));
    ui->label_13->setText(dd.at(6));
    ui->label_14->setText(dd.at(15));
    ui->label_24->setText(dd.at(17));
    ui->label_25->setText(dd.at(16));
    ui->voltage->setText(dd.at(9));


    ui->customp->graph(0)->setData(x,yd);
    ui->customp->xAxis->setRange(std::fmax(0,l-22),l+3);
    ui->customp->replot();
    ui->label_28->setText(dd.at(23));

    ui->customp->yAxis->setRange(dd.at(23).toFloat()-20,500);

        ui->customp2->graph(0)->setData(x,ydf);
        ui->customp2->xAxis->setRange(std::fmax(0,l-22),l+3);
        ui->customp2->yAxis->setRange(ydf.last()-1000,ydf.last()+1000);
        ui->customp2->replot();
        dd.at(5)=="Y"?ui->label_26->setText(QString::number(101325*std::pow((1-(dd.at(23).toDouble()/44330)),5.33))):ui->label_26->setText("0");
//        ui->customp2->yAxis->setRange(*ydf.data()-10000,101325);
        ui->horizontalWidget->getpos(&ydf.last());


        ui->customp3->graph(0)->setData(x,yd1);
        ui->customp3->xAxis->setRange(std::fmax(0,l-22),l+3);
        ui->customp3->replot();
        ui->label_27->setText(dd.at(24));
        ui->customp3->yAxis->setRange(*yd1.data()-20,*yd1.data()+20);

        ui->customp1->graph(0)->setData(x,yd2);
        ui->customp1->xAxis->setRange(std::fmax(0,l-22),l+5);
        ui->label_29->setText(dd.at(25));
        ui->customp1->yAxis->setRange(dd.at(25).toFloat()-20,dd.at(25).toFloat()+20);
        ui->customp1->replot();

        ui->customp4->graph(0)->setData(x,ydf2);
        ui->customp4->xAxis->setRange(std::fmax(0,l-22),l+3);
        ui->customp4->replot();
       dd.at(6)=="Y"?ui->label_30->setText(QString::number(101325*std::pow((1-(dd.at(30).toDouble()/44330)),5.33))):ui->label_30->setText("0");
        ui->customp4->yAxis->setRange(ydf2.last()-1000,ydf2.last()+1000);
        ui->horizontalWidget_2->getpos(&ydf2.last());


        ui->customp5->graph(0)->setData(x,yd5);
        ui->customp5->xAxis->setRange(std::fmax(0,l-22),l+3);
        ui->customp5->replot();
        ui->label_31->setText(dd.at(31));

        ui->customp5->yAxis->setRange(*yd5.data()-20,*yd5.data()+20);


        ui->customp6->graph(0)->setData(x,yd4);
        ui->customp6->xAxis->setRange(std::fmax(0,l-22),l+3);
        ui->customp6->replot();
        ui->label_32->setText(dd.at(30));

        ui->customp6->yAxis->setRange(*yd4.data()-20,500);


        ui->customp7->graph(0)->setData(x,yd6);
        ui->customp7->xAxis->setRange(std::fmax(0,l-22),l+3);
        ui->customp7->replot();
        ui->label_46->setText(dd.at(32));
        ui->customp7->yAxis->setRange(*yd6.data()-20,*yd6.data()+20);
        ui->customp7->yAxis->setRange(*yd6.data()-20,*yd6.data()+20);

        ui->customp5_4->graph(0)->setData(x,yd8);
        ui->customp5_4->xAxis->setRange(std::fmax(0,l-22),l+3);
        ui->customp5_4->replot();
        ui->customp5_4->yAxis->setRange(*yd8.data()-4,*yd8.data()+4);

        ui->customp5_5->graph(0)->setData(x,yd9);
        ui->customp5_5->xAxis->setRange(std::fmax(0,l-22),l+3);
        ui->customp5_5->replot();
        ui->customp5_5->yAxis->setRange(*yd9.data()-20,500);


}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->quickWidget->setSource(QUrl::fromLocalFile("/maps/main.qml"));

    ui->quickWidget->rootContext()->setContextProperty("gmap",&locator);
    ui->quickWidget->show();

    //resize output csv to 0 ie clear it
    wr.open(QIODevice::Append);
    wr.resize(0);
    wr.close();

    //substantive part of MainWindow , initialises arduino and mqtt clients and configures them
    arduino = new QSerialPort;
    mqttclient = new QMqttClient;
    configureSerial();
    arduino->clear();
    setupmqtt();
    connectmqtt();
    connect(mqttclient,SIGNAL(stateChanged()),this,SLOT(updatemqtt()));
    locator.setData("28.60897", "77.03826");

    ui->cansat->setAttribute(Qt::WA_TranslucentBackground);
    ui->kalpana->setAttribute(Qt::WA_TranslucentBackground);
    ui->panel->setAttribute(Qt::WA_TranslucentBackground);


    track=1;
    ui->panel->setScaledContents(true);
    ui->panel->setPixmap(QPixmap(":/pics/Images/panel.png"));
    ui->cansat->setScaledContents(true);
    ui->cansat->setPixmap(QPixmap(":/pics/Images/cansat.png"));
    ui->kalpana->setScaledContents(true);
    ui->kalpana->setPixmap(QPixmap(":/pics/Images/kalpana.png"));


       ui->customp7->yAxis->setLabel("SP_2 Rotation rate");
       ui->customp->yAxis->setLabel("SP_1 altitude");
       ui->customp1->yAxis->setLabel("SP_1 rotation rate");
       ui->customp2->yAxis->setLabel("SP_1 Pressure");
       ui->customp3->yAxis->setLabel("SP_1 Temp");
       ui->customp4->yAxis->setLabel("SP_2 Pressure");
       ui->customp5->yAxis->setLabel("SP_2 Temp");
       ui->customp5_4->yAxis->setLabel("Container Voltage");
       ui->customp5_5->yAxis->setLabel("container Altitude");

       ui->customp6->yAxis->setLabel("SP_2 Altitude");

    init (ui->customp);
    init(ui->customp1);
    init(ui->customp2);
    init(ui->customp3);
    init(ui->customp4);
    init(ui->customp5);
    init(ui->customp6);
    init(ui->customp7);

    init(ui->customp5_4);
    init(ui->customp5_5);




}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSerial()
{
    qDebug()<<"starting readserial"<<endl;
    bytesavail = arduino->canReadLine();
    qDebug()<<"can read line? : "<<bytesavail;
    if(bytesavail){
    serial = arduino->readLine();
//    wr.seek(0);
    wr.open(QIODevice::Append);
     wr.write(serial);
     wr.close();
     ui->label_74->setText(serial);



    qDebug()<<serial;


        // clearing buffer;
        serialBuffer = "";
        telemetry = serial;
        sensorData = telemetry.split(",");
        qDebug()<<" telem len: "<<sensorData.length();
        if(sensorData.length()>32)
        {
            mqttpub(sensorData);
            plotdata();
        }

    }



}

void MainWindow::connectSerial()
{
    arduino->clear();   //used to clear out data in the pipeline before starting to read arduino data
    waste = arduino->readAll(); //sameasabove
    QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
}

void MainWindow::disconnectSerial()
{
    QObject::disconnect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
}

void MainWindow::on_toolButton_7_clicked()
{
    writearduino(simstring+"ACTIVATE");
}

void MainWindow::on_toolButton_5_clicked()
{

    if(arduinoavail) {
    if(!cx)
    {
        writearduino(cxtele+"ON");
        connectSerial();
        cx=true;
        ui->toolButton_5->setText("TURN CX OFF");
        qDebug()<<"connected serial";
    }

    else if(cx)
    {
        writearduino(cxtele+"OFF");
        disconnectSerial();
        wr.close();
        cx=false;
        ui->toolButton_5->setText("TURN CX ON");
        qDebug()<<"disconnected serial";

    }        }


    else
    {
        if(configureSerial())
        {
          writearduino(cxtele+"ON");
          connectSerial();
          cx=true;
          ui->toolButton_5->setText("TURN CX OFF");
          qDebug()<<"connected serial";
        }
        else
            QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to arduino.");

    }



}

void MainWindow::on_toolButton_clicked()
{
    QDateTime time = QDateTime::currentDateTimeUtc();
//    timestring = time.time().toString();
    timetele="CMD,3394,ST," + time.time().toString();
    qDebug()<<"time tele string:"<<timetele;
    writearduino(timetele);

}

void MainWindow::on_toolButton_2_clicked()
{
    writearduino(simstring+"ENABLE\n");
}

void MainWindow::on_toolButton_6_clicked()
{
    writearduino(simstring+"DISABLE");
}

void MainWindow::on_toolButton_3_clicked()
{
    if(!px1enabled)
    {
        writearduino(px1string+"ON");
        px1enabled = true;
        ui->toolButton_3->setText("TURN SP1X OFF");
    }

    else
    {
        writearduino(px1string+"OFF");
        px1enabled= false;
        ui->toolButton_3->setText("TURN SP1X ON");
    }
}

void MainWindow::on_toolButton_8_clicked()
{
    if(!px2enabled)
    {
        writearduino(px2string+"ON");
        px2enabled = true;
        ui->toolButton_8->setText("TURN SP2X OFF");
    }

    else
    {
        writearduino(px2string+"OFF");
        px2enabled= false;
        ui->toolButton_8->setText("TURN SP2X ON");
    }
}

void MainWindow::on_toolButton_4_clicked()
{
    if(!simpdata.open(QIODevice::ReadOnly))
      qDebug()<<"error opening SIMP file";
    else
    {
//  simptele = simpdata.readLine();
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()),this,SLOT(sendsimp()));
    timer->start();
    }
}



void MainWindow::on_pushButton_clicked()
{
    updatemqtt();
}
