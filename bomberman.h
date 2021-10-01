#ifndef BOMBERMAN_H
#define BOMBERMAN_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsItem>
#include <QRectF>
#include <QDesktopWidget>
#include <QDebug>
#include <vector>
#include <list>
#include <array>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QDebug>
#include "man.h"
#include "bomba.h"
#include "onda.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Bomberman; }
QT_END_NAMESPACE

class Bomberman : public QMainWindow
{
    Q_OBJECT

public:
    Bomberman(QWidget *parent = nullptr);
    ~Bomberman();

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::Bomberman *ui;

    QTimer *Explos = new QTimer(this);

    QGraphicsScene *scena;
    man *hombre;
    Bomba *bomba;
    Onda *onda;
    QGraphicsRectItem *puerta;
    std::vector<QGraphicsRectItem *> Solid;
    std::array<QGraphicsRectItem *,55> Brick;
    std::array<man *, 6> enemigos;
    std::array<Onda *, 5> PosExplosion;


public slots:

    void Colisiones();
    void MovEnemys();
    void Time();
    void Explosion();
};
#endif // BOMBERMAN_H
