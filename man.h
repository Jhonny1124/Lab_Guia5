#ifndef MAN_H
#define MAN_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPixmap>
#include <QPainter>

class man : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit man(short int a = 0, QObject *parent = nullptr);

    QTimer *timer;
    QPixmap *pixmap;

    float imageActual;

    float filas, columnas;

    float width, height, vidas = 3;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void direccion(int);

signals:

public slots:
    void Movimiento();

};

#endif // MAN_H
