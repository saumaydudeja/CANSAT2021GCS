#ifndef meter_H
#define meter_H
#include <QWidget>




class meter : public QWidget
{
Q_OBJECT

public:
QPixmap *img = new QPixmap;
double val1=0;
void getpos(int *val);
void getpos(double *val);




meter(QWidget *parent=nullptr);

virtual ~meter()

{};


protected:
void paintEvent(QPaintEvent *event) ;
};


#endif // meter_H
