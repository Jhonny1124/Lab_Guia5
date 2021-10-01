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
#include <array>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QDebug>
#include "man.h"
#include "bomba.h"

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

    QGraphicsScene *scena;
    man *hombre;
    Bomba *bomba;
    QGraphicsRectItem *puerta;
    std::vector<QGraphicsRectItem *> Solid;
    std::array<QGraphicsRectItem *,55> Brick;
    std::vector<man *> enemigos;

public slots:

    void Colisiones();
    void MovEnemys();
    void Time();
};
#endif // BOMBERMAN_H
