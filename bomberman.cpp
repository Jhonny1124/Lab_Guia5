#include "bomberman.h"
#include "ui_bomberman.h"

#define TamBloque 70
#define image_Bloque "../Bomberman/Imagenes/Bloque.jpg"
#define image_Ladrillo "../Bomberman/Imagenes/Ladrillo.jpg"

static int pos_x = 105, pos_y = 105;
short int cont =-1, cont1 = -1, cont2 = 0;
short int cop_pos_x = pos_x, cop_pos_y = pos_y;
short int PosBomX, PosBomY, bomba1 = 0;
short int posEnemigos[6][2], prueba[6];
short int tiempo = 300, puntos = 0;

Bomberman::Bomberman(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Bomberman)
{
    ui->setupUi(this);
    connect(Explos, SIGNAL(timeout()), this, SLOT(Explosion()));
    scena = new QGraphicsScene;
    hombre = new man();
    bomba = new Bomba();
    onda = new Onda();
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
                enemigos.at(cont1) = new man(1);
                enemigos.at(cont1)->setPos((x*TamBloque)+(TamBloque/2),(y*TamBloque)+(TamBloque/2));
                posEnemigos[cont1][0] = (x*TamBloque)+(TamBloque/2);
                posEnemigos[cont1][1] = (y*TamBloque)+(TamBloque/2);
                prueba[cont1] = 0;
                scena->addItem(enemigos.at(cont1));
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
        if(bomba1 == 0){
            PosBomX = pos_x, PosBomY = pos_y;
            scena->addItem(bomba);
            bomba->setPos(PosBomX, PosBomY);
            Explos->start(1000);
        }
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

    if(hombre->collidesWithItem(puerta)){
        exit(1);
    }
    hombre->setPos(pos_x, pos_y);

}

void Bomberman::MovEnemys()
{

    // Movimiento de los enemigos //

    for(int e = 0; e < int(enemigos.size()); e++){
        if(enemigos.at(e) != NULL){
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
            if(enemigos.at(e)->collidesWithItem(bomba)){
                if(prueba[e] == 0){
                    prueba[e] = -1;
                }
                else{
                    prueba[e] = 0;
                }
            }
        }
    }
}

void Bomberman::Time()
{
    // Timer del juego //

    ui->lcdNumber_3->display(tiempo--);
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
}

void Bomberman::Explosion()
{
    //Animacion y acciones de la bomba//
        cont2++;
        bomba1++;
        if(cont2 == 3){
            scena->removeItem(bomba);
            for(int i = 0; i < 5; i++){
                PosExplosion.at(i) = new Onda();
                if(i == 0){
                    PosExplosion.at(i)->setPos(PosBomX, PosBomY);
                }
                else if(i == 1){
                    PosExplosion.at(i)->setPos(PosBomX+60, PosBomY);
                }
                else if(i == 2){
                    PosExplosion.at(i)->setPos(PosBomX-60, PosBomY);
                }
                else if(i == 3){
                    PosExplosion.at(i)->setPos(PosBomX, PosBomY+60);
                }
                else if(i == 4){
                    PosExplosion.at(i)->setPos(PosBomX, PosBomY-60);
                }
                scena->addItem(PosExplosion.at(i));
            }
            for(int i = 0; i < 5; i++){
                if(PosExplosion.at(i)->collidesWithItem(hombre)){
                    hombre->vidas--;
                    pos_x = 105, pos_y = 105;
                    hombre->setPos(pos_x,pos_y);
                    ui->graphicsView->setSceneRect(pos_x,pos_y,70,70);
                    if(hombre->vidas == 0){
                        exit(1);
                    }
                    ui->lcdNumber->display(hombre->vidas);
                }
                for(int b = 0; b < int(Brick.size()); b++){
                    if(PosExplosion.at(i)->collidesWithItem(Brick.at(b))){
                        scena->removeItem(Brick.at(b));
                        Brick.at(b) = NULL;
                        puntos += 100;
                    }
                }
                for(int e = 0; e < int(enemigos.size()); e++){
                    if(PosExplosion.at(i)->collidesWithItem(enemigos.at(e))){
                        scena->removeItem(enemigos.at(e));
                        enemigos.at(e) = NULL;
                        puntos +=800;
                    }
                }

            }
        }
        if(cont2 == 4){
            for(int i = 0; i < 5; i++){
                scena->removeItem(PosExplosion.at(i));
                PosExplosion.at(i) = NULL;
                cont2 = 0;
                bomba1 = 0;
                Explos->stop();
            }
        }
        ui->lcdNumber_2->display(puntos);

}
