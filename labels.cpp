#include "labels.h"

Labels::Labels(int height, Player *player, QGraphicsView* view){
    QFont font;
    healthlabel = new QLabel("Health: "+ QString::number(player->health), view);
    healthlabel->setStyleSheet("background-color: rgba(255, 255, 255, 50); border: none; color: red");
    scorelabel = new QLabel("Score: "+ QString::number(player->score), view);
    scorelabel->setStyleSheet("background-color: rgba(255, 255, 255, 50); border: none;color: yellow");

    healthlabel->show();
    healthlabel->move(0,height);

    scorelabel->show();

    scorelabel->move(0,height+healthlabel->geometry().height());

    font = healthlabel->font();
    font.setPixelSize(18);
    healthlabel->setFont(font);
    scorelabel->setFont(font);

    Labels::player = player;
    Labels::view = view;

}

void Labels::update(){
    // Used to update labels

    if (player == nullptr){
        return;
    }
    healthlabel->setText("Health: "+ QString::number(player->health));
    scorelabel->setText("Score: "+ QString::number(player->score));
    auto geo = healthlabel->geometry();
    geo.setWidth(healthlabel->fontMetrics().horizontalAdvance(healthlabel->text()));
    healthlabel->setGeometry(geo);

    geo = scorelabel->geometry();
    geo.setWidth(scorelabel->fontMetrics().horizontalAdvance(scorelabel->text()));
    scorelabel->setGeometry(geo);
}

void Labels::removePlayer(){
    // Used when player is removed and no need for updating
    player = nullptr;
}
