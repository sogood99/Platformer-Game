#include "flags.h"
#include "game.h"

Flags::Flags(QPoint p):Tiles(p){

}

StartFlag::StartFlag(Player* player,QPoint p):Flags(p){
    player->moveBy(p.x(),p.y()-80);
    player->bounce(-1);
    QPixmap pixmap(":/Pictures/TilePictures/TilePictures/StartTile.png");
    pixmap = pixmap.scaled(80,80);
    setPixmap(pixmap);
}

void StartFlag::collide(Player *player){
    Q_UNUSED(player);
}

EndFlag::EndFlag(Game* game, QPoint p):Flags(p){
    m_game = game;
    QPixmap pixmap(":/Pictures/TilePictures/TilePictures/EndTile.png");
    pixmap = pixmap.scaled(80,80);
    setPixmap(pixmap);
}

void EndFlag::collide(Player *player){
    Q_UNUSED(player);
    m_game->gameWin();
}
