#ifndef LABELS_H
#define LABELS_H

#include "player.h"
#include <QLabel>

class Labels{
    /*
     * Labels for showing player's score and health. (in qgraphicsview)
     */
public:
    Labels(int height, Player* player, QGraphicsView* view);
public:
    Player* player = nullptr;
    QLabel* healthlabel = nullptr;
    QLabel* scorelabel = nullptr;
    QGraphicsView* view = nullptr;
public:
    void update();
    void removePlayer();
};

#endif // LABELS_H
