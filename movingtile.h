#ifndef MOVINGTILE_H
#define MOVINGTILE_H

#include "tiles.h"

class MovingTile: public Tiles{
    /*
     * Moving tile that is able to transport player
     */
public:
    MovingTile(QPoint start, QPoint end, qreal velocity = 1);
    void update();
public:
    QPoint end;
    QPoint start;
    int direction = 1;
    int currentCount = 0;
    int totalCount = 0;
};

#endif // MOVINGTILE_H
