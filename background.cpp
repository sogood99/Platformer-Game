#include "background.h"

Background::Background(): QGraphicsPixmapItem(){

    QPixmap pixmap;
    pixmap.load(":/Pictures/BackgroundPicture/BackgroundPic/Background2.png");
    pixmap = pixmap.scaled(2000,1000);
    setPixmap(pixmap);

    moveBy(-1000,-500);
}
