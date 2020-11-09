#ifndef SANDTILES_H
#define SANDTILES_H

#include "tiles.h"

class SandTile: public Tiles{
    /*
     * Child of tiles, has friction.
     */
public:
    SandTile(QPoint p);
};

#endif // SANDTILES_H
