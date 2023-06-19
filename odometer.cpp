#include <QtGui>
#include <odometer.h>
#include <math.h>



meter::meter(QWidget *parent)
    :QWidget(parent)
{
//    img->scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    img->load("C:/meter.png");


}


void meter::getpos(double *val)
{
    val1=*val;
    this->update();
}
void meter::getpos(int *val)
{
    val1=*val;
    this->update();
}


void meter::paintEvent(QPaintEvent *)
{

    QPointF pts[3] = {
        QPointF(-5,70),
        QPointF(5,70),
        QPointF(65*qCos(2.0944+(0.017*val1)),65*qSin(2.0944+(0.017*val1)))
        //QPointF(0,0)

};
  QPainter paint(this);

          paint.translate(125,120);
       paint.setRenderHint(QPainter::Antialiasing);
        paint.setPen(Qt::red);
        paint.setBrush(QColor(Qt::green));


       paint.save();

        paint.drawConvexPolygon(pts , 3);
        paint.drawPixmap(-125,-120,250,250,*img);
        paint.restore();

}
