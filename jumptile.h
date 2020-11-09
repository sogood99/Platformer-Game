#ifndef JUMPTILES_H
#define JUMPTILES_H

#include "tiles.h"
#include "utils.h"

class JumpTile: public Tiles{
    /*
     * Child of Tiles class, gives player a jump when on.
     */
    Q_OBJECT
public:
    JumpTile(QPoint p);
    QTimer frameTimer;
    void nextFrame();
    int currentFrame = 0;
};

#endif // JUMPTILES_H
