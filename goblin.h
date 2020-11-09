#ifndef GOBLIN_H
#define GOBLIN_H

#include "utils.h"
#include "player.h"
#include "monster.h"

class Goblin: public Monster{
    /*
     * Subclass of monster class, if player is near it attacks player, if collides with tile, delete
     */
    Q_OBJECT
public:
    Goblin(Player* player, QPoint p = QPoint(0,0));
    virtual ~Goblin() {}

public:
    enum { Type = UserType + 1 };
    int type() const override{
        return Type;
    }
public:
    void update() override;
    void nextFrame() override;
    void remove() override;
    void setupRemove() override;
protected:
    Player* m_player= nullptr;
    bool start = false;
    void animateRemove() override;
};

#endif // GOBLIN_H
