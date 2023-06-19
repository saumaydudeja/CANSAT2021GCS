#include <QtGui>
#include <bar.h>

bar::bar(QWidget *parent)
    :QWidget(parent)
{
//    img->scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    imgb->load(":/pics/Images/bar1-06.png");


}

void bar::paintEvent(QPaintEvent *)
{

    QPointF pts[3] = {
        QPointF(35,val1),
        QPointF(13,val1+10),
        QPointF(13,val1-10)

};
  QPainter paint(this);

       paint.setRenderHint(QPainter::Antialiasing);
        paint.setPen(Qt::red);
        paint.setBrush(QColor(46,27,58));


       paint.save();

        paint.drawConvexPolygon(pts , 3);
        paint.drawPixmap(0,0,150,250,*imgb);

        paint.restore();

}

void bar::getpos(double *val)
{
    k= *val-80000;
    val1= 250-(250*(k)/21400)+10;
    this->update();
}
