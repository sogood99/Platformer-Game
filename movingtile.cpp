#include "movingtile.h"

MovingTile::MovingTile(QPoint start, QPoint end, qreal velocity):Tiles(start){

    MovingTile::end = end;
    MovingTile::start = start;

    velocity = qBound(-MAX_VELOCITY, velocity, MAX_VELOCITY);

    auto diff = end-start;
    qreal d_x = diff.x();
    qreal d_y = diff.y();
    qreal length = sqrt(diff.dotProduct(diff,diff));

    Tiles::velocity.first = d_x*velocity/length;
    Tiles::velocity.second = d_y*velocity/length;

    totalCount = length/velocity;

    QPixmap pixmap(":/Pictures/TilePictures/TilePictures/IceTile.png");
    pixmap = pixmap.scaled(80,80);
    setPixmap(pixmap);
}

void MovingTile::update(){
    if (currentCount > totalCount){
        direction *= -1;
        velocity.first *= -1;
        velocity.second *= -1;
        currentCount = 0;
    }else{
        currentCount++;
        moveBy(velocity.first,velocity.second);
    }
}
