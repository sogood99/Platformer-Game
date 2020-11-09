#include "sandtile.h"

SandTile::SandTile(QPoint p): Tiles(p, 0.5){
    QPixmap pixmap(":/Pictures/TilePictures/TilePictures/SandTile.png");
    pixmap = pixmap.scaled(80,80);
    setPixmap(pixmap);
}
