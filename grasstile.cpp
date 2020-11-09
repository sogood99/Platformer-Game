#include "grasstile.h"

GrassTile::GrassTile(QPoint p):Tiles(p){
    QPixmap pixmap(":/Pictures/TilePictures/TilePictures/GrassTile.png");
    pixmap = pixmap.scaled(80,80);
    setPixmap(pixmap);
}
