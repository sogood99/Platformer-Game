#ifndef FLAGS_H
#define FLAGS_H

#include "tiles.h"
#include "player.h"

class Game;

class Flags: public Tiles{
    /*
     * Contains start and end flag, used for placing player and ending game
     */
public:
    enum { Type = UserType + 10 };
    Flags(QPoint p);
    int type() const override{
        return Type;
    }
    virtual void collide(Player* player) = 0;
};

class StartFlag: public Flags{
public:
    StartFlag(Player* player, QPoint p);
    virtual void collide(Player *player) override;
};

class EndFlag: public Flags{
public:
    EndFlag(Game* game, QPoint p);
    virtual void collide(Player *player) override;
    Game* m_game = nullptr;
};

#endif // FLAGS_H
