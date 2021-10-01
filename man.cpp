#include "man.h"

man::man(short int a, QObject *parent) : QObject(parent)
{
    timer = new QTimer;

    columnas = 0;
    if(a == 0){
        pixmap = new QPixmap(":/Imagenes/Bomberman.png");
        filas = 60;
    }
    else{
        pixmap = new QPixmap(":/Imagenes/Enemigo.png");
        filas = 0;
    }

    width = 60;
    height = 60;

    connect(timer, &QTimer::timeout, this, &man::Movimiento);

    timer->start(150);
}

QRectF man::boundingRect() const
{
    return QRectF(-width/2,-height/2,width,height);
}

void man::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-width/2,-height/2, *pixmap, columnas, filas, width, height);
}

void man::direccion(int a)
{
    if(a == 0){
        filas = 180;
    }
    else if(a == 1){
        filas = 0;
    }
    else if(a == 2){
        filas = 60;
    }
    else if(a == 3){
        filas = 120;
    }
}

void man::Movimiento()
{
    columnas +=60;
    if(columnas >= 180){
        columnas = 0;
    }
    this->update(-width/2,-height/2,width,height);
}
