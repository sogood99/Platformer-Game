#include "collapsetile.h"
#include <QPropertyAnimation>

CollapseTile::CollapseTile(QPoint p):Tiles(p){
    QPixmap pixmap(":/Pictures/TilePictures/TilePictures/CollapseTile.png");
    pixmap = pixmap.scaled(80,80);
    setPixmap(pixmap);
}

void CollapseTile::remove(){
    // Animation for collapsing into center
    setTransformOriginPoint(QPointF(40,40));
    QPropertyAnimation *removeAnimation = new QPropertyAnimation(this, "scale");
    removeAnimation->setStartValue(1);
    removeAnimation->setEndValue(0);
    removeAnimation->setDuration(300);
    removeAnimation->start();
    connect(removeAnimation, &QPropertyAnimation::finished, this, &QObject::deleteLater);
}

void CollapseTile::collidedWith(Bullet *){
    setupRemove();
}


