#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPixmap>
#include<datawindow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //code for image
    QPixmap pix("C:/Users/Ujjawal/OneDrive/Desktop/tk.jpg");
    ui->label_3->setPixmap(pix);

    //code for leds
    myLedIndicator = new LedIndicator();
    myLedIndicator2 = new LedIndicator();
    myLedIndicator3 = new LedIndicator();
    ui->gridLayout->addWidget(myLedIndicator,1,0);
    ui->gridLayout->addWidget(myLedIndicator2,2,0);
    ui->gridLayout->addWidget(myLedIndicator3,3,0);
    ui->spinBox->setValue(20);
}

MainWindow::~MainWindow()
{
    delete ui;
}

///////////////////////////////////
void MainWindow::on_spinBox_editingFinished()
{
    myLedIndicator->setLedSize(ui->spinBox->value());
    myLedIndicator2->setLedSize(ui->spinBox->value());
    myLedIndicator3->setLedSize(ui->spinBox->value());
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    myLedIndicator->setLedSize(arg1);
    myLedIndicator2->setLedSize(arg1);
    myLedIndicator3->setLedSize(arg1);


}
//////////////////////////////////////////
void MainWindow::on_pushButton_clicked()
{

    if(ui->pushButton->isChecked())
       { ui->display->setText("Command 1 sent");
        myLedIndicator->toggle(1);
    }
    else
    {ui->display->setText("Command Status");
        myLedIndicator->toggle(0);}


}

void MainWindow::on_pushButton_2_clicked()
{

    if(ui->pushButton_2->isChecked())
    {ui->display->setText("Command 2 sent");
    myLedIndicator2->toggle(1);}
    else
    {ui->display->setText("Command Status");
        myLedIndicator2->toggle(0);}
}
void MainWindow::on_pushButton_3_clicked()
{

    if(ui->pushButton_3->isChecked())
    {ui->display->setText("Command 3 sent");
    myLedIndicator3->toggle(1);
    }
    else {ui->display->setText("Command Status");
        myLedIndicator3->toggle(0);}
    }
//////////////////////////////////////////////


void MainWindow::on_pushButton_4_clicked()
{
    ui->tabWidget->addTab(new datawindow(), "Data Window");
}
