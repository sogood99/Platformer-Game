#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include <QObject>
#include <QGraphicsPixmapItem>

class GameObj: public QObject, public QGraphicsPixmapItem{
    /*
     * Base Class for Most thing That Exist in class Game: public QGraphicsView which player sees
     * Update given used by Game class for same delta T, use for movement and collision
     * If Game sees m_bRemove, it will call remove() and pop its reference (pointer) to it.
     */
    Q_OBJECT
public:
    GameObj(){}
    virtual void update() = 0; // for collision and movement
    virtual void setupRemove(); // for setting remove in next time step
    virtual void remove() = 0; // for deleting self, might contain some ending animation
    bool m_bRemove = false; // for letting Game know that should be removed, usually looked at by parent
};

#endif // GAMEOBJ_H
