#include "bomba.h"

Bomba::Bomba(QObject *parent) : QObject(parent)
{
    timer1 = new QTimer;
    filas1 = 0;
    columnas1 = 0;

    pixmap1 = new QPixmap(":/Imagenes/Bomba.png");


    width1 = 60;
    height1 = 60;

    connect(timer1, &QTimer::timeout, this, &Bomba::Cuenta);

    timer1->start(150);
}

QRectF Bomba::boundingRect() const
{
    return QRectF(-width1/2,-height1/2,width1,height1);
}

void Bomba::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-width1/2,-height1/2, *pixmap1, columnas1, filas1, width1, height1);
}

void Bomba::Cuenta()
{
    columnas1 +=60;
    if(columnas1 >= 180){
        columnas1 = 0;
    }
    this->update(-width1/2,-height1/2,width1,height1);
}
