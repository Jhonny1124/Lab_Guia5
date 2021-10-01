#include "bomberman.h"
#include "ui_bomberman.h"

#define TamBloque 70
#define image_Bloque "../Bomberman/Imagenes/Bloque.jpg"
#define image_Ladrillo "../Bomberman/Imagenes/Ladrillo.jpg"

static int pos_x = 105, pos_y = 105;
short int cont =-1, cont1 = -1;
short int cop_pos_x = pos_x, cop_pos_y = pos_y;
short int posEnemigos[6][2], prueba[6];
short int tiempo = 200;

Bomberman::Bomberman(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Bomberman)
{
    ui->setupUi(this);
    scena = new QGraphicsScene;
    hombre = new man();
    bomba = new Bomba();
    puerta = new QGraphicsRectItem;

    QPen pen(QColor(0,0,0));
    QImage SolidBlock(image_Bloque);
    QImage BrickBlock(image_Ladrillo);
    QImage Door("../Bomberman/Imagenes/Puerta.jpg");

    short int nivel [13][31] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,2,0,0,0,0,0,2,2,2,0,3,0,0,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,1},
        {1,0,1,0,1,2,1,0,1,0,1,0,1,2,1,0,1,0,1,0,1,0,1,0,1,2,1,0,1,2,1},
        {1,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,2,0,0,0,1},
        {1,2,1,2,1,0,1,0,1,0,1,2,1,0,1,0,1,0,1,0,1,0,1,2,1,0,1,0,1,0,1},
        {1,2,0,0,0,0,0,0,0,2,0,0,0,2,0,2,0,0,0,2,0,0,0,0,0,0,2,0,0,0,1},
        {1,2,1,2,1,2,1,0,1,2,1,0,1,0,1,0,1,2,1,0,1,0,1,0,1,0,1,0,1,2,1},
        {1,0,3,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,2,0,0,0,0,0,2,2,0,0,0,1},
        {1,0,1,2,1,0,1,2,1,0,1,0,1,0,1,2,1,0,1,0,1,2,1,0,1,0,1,2,1,0,1},
        {1,0,0,0,2,0,0,2,0,2,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0,0,0,0,0,1},
        {1,2,1,0,1,2,1,0,1,0,1,2,1,0,1,0,1,2,1,0,1,0,1,0,1,0,1,2,1,0,1},
        {1,0,2,0,0,0,0,0,0,2,0,0,0,0,0,2,0,0,0,0,0,3,0,0,0,0,2,0,0,3,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    //Posicionar la purta aleatoriamente//
    int ran_x, ran_y;
    while(true){
        ran_x = QRandomGenerator::global()->bounded(32);
        ran_y = QRandomGenerator::global()->bounded(13);
        if(nivel[ran_y][ran_x] == 2){
            puerta = scena->addRect((ran_x*TamBloque)+5,(ran_y*TamBloque)+5,TamBloque-10,TamBloque-10, pen, Door);
            break;
        }
    }
    //**********************************//

    //Posicionar bloques y enemigos//
    for(int y = 0; y < 13; y++){
        for(int x = 0; x < 31; x++){
            if(nivel[y][x] == 1){
                Solid.push_back(scena->addRect(x*TamBloque,y*TamBloque,TamBloque,TamBloque, pen, SolidBlock));
            }
            if(nivel[y][x] == 2){
                cont++;
                Brick.at(cont) = scena->addRect(x*TamBloque,y*TamBloque,TamBloque,TamBloque, pen, BrickBlock);
            }
            if(nivel[y][x] == 3){
                cont1++;
                enemigos.push_back(new man(1));
                enemigos.back()->setPos((x*TamBloque)+(TamBloque/2),(y*TamBloque)+(TamBloque/2));
                posEnemigos[cont1][0] = (x*TamBloque)+(TamBloque/2);
                posEnemigos[cont1][1] = (y*TamBloque)+(TamBloque/2);
                prueba[cont] = 0;
                scena->addItem(enemigos.back());
            }
        }
    }
    //***************************************//

    //Agregar fondo y personaje principal a la escena//
    scena->setBackgroundBrush(QColor(0,148,0));
    scena->addItem(hombre);
    hombre->setPos(105,105);
    //**********************************************//


    ui->graphicsView->setSceneRect(105,105,70,70);
    ui->graphicsView->setScene(scena);
    ui->lcdNumber_2->display(0);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Colisiones()));
    timer->start(1);

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(MovEnemys()));
    timer2->start(150);

    QTimer *time = new QTimer(this);
    connect(time, SIGNAL(timeout()), this, SLOT(Time()));
    time->start(1000);

    ui->graphicsView->show();
}

Bomberman::~Bomberman()
{
    delete ui;
}

void Bomberman::keyPressEvent(QKeyEvent *e)
{
    // Movimiento del personaje con el teclado //
    switch (e->key()) {
    case Qt::Key_A:
        hombre->setPos(pos_x-=5, pos_y);
        hombre->direccion(1);
        break;
    case Qt::Key_S:
        hombre->setPos(pos_x, pos_y+=5);
        hombre->direccion(2);
        break;
    case Qt::Key_D:
        hombre->setPos(pos_x+=5, pos_y);
        hombre->direccion(3);
        break;
    case Qt::Key_W:
        hombre->setPos(pos_x, pos_y-=5);
        hombre->direccion(0);
        break;
    case Qt::Key_Space:
        scena->addItem(bomba);
        bomba->setPos(pos_x, pos_y);
        break;
    default:
        break;
    }
   ui->graphicsView->setSceneRect(pos_x,pos_y,70,70);
}

void Bomberman::Colisiones()
{
    // Interaccines entre los objetos de la escena //

    for(int i = 0; i < int(Solid.size()); i++){
        if(hombre->collidesWithItem(Solid.at(i))){
            if(pos_x < cop_pos_x){
                pos_x += 5;
                cop_pos_x = pos_x;
                break;
            }
            else if(pos_x > cop_pos_x){
                pos_x -= 5;
                cop_pos_x = pos_x;
                break;
            }
            else if(pos_y < cop_pos_y){
                pos_y += 5;
                cop_pos_y = pos_y;
                break;
            }
            else if(pos_y > cop_pos_y){
                pos_y -= 5;
                cop_pos_y = pos_y;
                break;
            }
        }
    }
    for(int i = 0; i < int(Brick.size()); i++){
        if(hombre->collidesWithItem(Brick.at(i))){
            if(pos_x < cop_pos_x){
                pos_x += 5;
                cop_pos_x = pos_x;
                break;
            }
            else if(pos_x > cop_pos_x){
                pos_x -= 5;
                cop_pos_x = pos_x;
                break;
            }
            else if(pos_y < cop_pos_y){
                pos_y += 5;
                cop_pos_y = pos_y;
                break;
            }
            else if(pos_y > cop_pos_y){
                pos_y -= 5;
                cop_pos_y = pos_y;
                break;
            }
        }
    }
    for(int e = 0; e < int(enemigos.size()); e++){
         if(hombre->collidesWithItem(enemigos.at(e))){
             pos_x = 105, cop_pos_x = 105;
             pos_y = 105, cop_pos_y = 105;
             hombre->vidas--;
         }
    }
    if(tiempo == 0){
        tiempo = 200;
        pos_x = 105, cop_pos_x = 105;
        pos_y = 105, cop_pos_y = 105;
        hombre->vidas--;
    }
    if(hombre->vidas == 0){
        exit(1);
    }
    ui->lcdNumber->display(hombre->vidas);
    hombre->setPos(pos_x, pos_y);

}

void Bomberman::MovEnemys()
{

    // Movimiento de los enemigos //

    for(int e = 0; e < int(enemigos.size()); e++){
        if(prueba[e] == 0){
            enemigos.at(e)->setPos(posEnemigos[e][0]-5,posEnemigos[e][1]);
            posEnemigos[e][0] -= 5;
        }
        else{
            enemigos.at(e)->setPos(posEnemigos[e][0]+5,posEnemigos[e][1]);
            posEnemigos[e][0] += 5;
        }
        for(int i = 0; i < int(Brick.size()); i++){
            if(enemigos.at(e)->collidesWithItem(Brick.at(i))){
                if(prueba[e] == 0){
                    prueba[e] = -1;
                }
                else{
                    prueba[e] = 0;
                }
                break;
            }
        }
        for(int i = 0; i < int(Solid.size()); i++){
            if(enemigos.at(e)->collidesWithItem(Solid.at(i))){
                if(prueba[e] == 0){
                    prueba[e] = -1;
                }
                else{
                    prueba[e] = 0;
                }
                break;
            }
         }
    }
}

void Bomberman::Time()
{
    // Timer del juego //

    ui->lcdNumber_3->display(tiempo--);
}
