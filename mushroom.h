#ifndef MUSHROOM_H
#define MUSHROOM_H

#include "monster.h"

class Mushroom: public Monster{
    /*
     * Child of Monster class, mushroom is most basic form of monster.
     */
public:
    Mushroom(QPoint p);
    void update() override;
    void nextFrame() override;
    void remove() override;
protected:
    void animateRemove() override;
};

#endif // MUSHROOM_H
