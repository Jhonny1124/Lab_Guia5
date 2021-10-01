#ifndef BOMBA_H
#define BOMBA_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPixmap>
#include <QPainter>

class Bomba : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Bomba(QObject *parent = nullptr);

    QTimer *timer1;
    QPixmap *pixmap1;

    float imageActual1;

    float filas1, columnas1;

    float width1, height1, maxi = 1;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:
    void Cuenta();
};

#endif // BOMBA_H
