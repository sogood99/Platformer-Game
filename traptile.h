#ifndef TRAPTILE_H
#define TRAPTILE_H

#include "tiles.h"

class TrapTile: public Tiles{
    /*
     * Child of tiles class, looks like grass tile until collide
     */
public:
    TrapTile(QPoint p);
    void setActive();
    void collidedWith(Player *) override;
    void collidedWith(Monster *) override;
public:
    bool isActive = false; // bool for if trap is shown
};

#endif // TRAPTILE_H
