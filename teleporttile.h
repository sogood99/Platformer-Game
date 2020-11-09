#ifndef TELEPORTTILE_H
#define TELEPORTTILE_H

#include "utils.h"
#include "tiles.h"
#include "player.h"
#include "monster.h"
#include "goblin.h"

class TeleportTile: public Tiles{
    /*
     * Child of tiles, can teleport player between two tiles
     * Design is like a linked list
     */
public:
    TeleportTile(QPoint pointA, QPoint pointB, TeleportTile* otherTile = nullptr);
    void collidedWith(Player *) override;
    void update() override;
public:
    Tiles* otherTile = nullptr;
private:
    QMediaPlayer* teleportSound = nullptr;
    void playTeleportSound();
};

#endif // TELEPORTTILE_H
