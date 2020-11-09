#ifndef COLLAPSETILE_H
#define COLLAPSETILE_H

#include "tiles.h"

class CollapseTile: public Tiles{
    /*
     * Child of tiles class, collapses when there is collision with bullet.
     */
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
public:
    CollapseTile(QPoint p);
    void remove() override;
    void collidedWith(Bullet *) override;
};

#endif // COLLAPSETILE_H
