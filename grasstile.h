#ifndef GRASSTILE_H
#define GRASSTILE_H

#include "tiles.h"

class GrassTile: public Tiles{
    // Most basic tile, no friction and stuff
public:
    GrassTile(QPoint p);
};

#endif // GRASSTILE_H
