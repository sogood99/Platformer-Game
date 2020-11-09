#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"
#include "gameobj.h"
#include "bullet.h"

class Player: public GameObj{
    /*
     * Used as player that controlled through Game: public QGraphicsView
     */
    Q_OBJECT
public:
    enum { Type = UserType + 2 };
    Player();
    virtual ~Player(){}
public:
    QPair<qreal, qreal> m_velocity; // velocity infuenced by other gameobjects
    QPair<qreal, qreal> m_acceleration; // acceleration infulenced by other gameobjects
    const QPair<qreal, qreal> baseVelocity = QPair<qreal, qreal>(1 ,0); // move velocity
    const QPair<qreal, qreal> jumpVelocity = QPair<qreal, qreal>(0, -3); //jump velocity
    const QPair<qreal, qreal> gravityAcceleration = QPair<qreal, qreal>(0,GRAVITY);
    bool takingDamage = false;
    void addDirection(int direction); // adds direction for player
    void jump(); // used by player to check if can jump, and jump
    void bounce(qreal velocity); // used by other gameobjects to add y velocity
    void push(qreal velocity); // used by other gameobjects to add x velocity
    void minusHealth(int subtract = 1);
    void addScore(int add = 1);
    virtual void update() override; //update position and state and stuff
    virtual void remove() override;
    virtual void setRemove();
    int type() const override{
        return Type;
    }
    int health = 3;
    int score = 0;
    int bulletCount = 0;
public slots:
    void setNotDamage();
    void nextFrame();//change frame
    void animateDamage();
    void fire();
private:
    // some private properties of player
    int direction = 0;
    bool spriteReversed = false;
    bool collideBottom = false;
    bool collideTop = false;
    bool collideLeft = false;
    bool collideRight = false;
    state currentState = idle;

    // used when state is changed
    void stateChanged();

    // some members used for animation
    QTimer frameTimer;
    int currentIndex = 0;
    QTimer damageTimer;
    int damageCounter = 0;

    // player's bullet
    PlayerBullet *bullet = nullptr;
    void animateRemove();
    int removeIndex = 0;

    // some sound effects
    void initializeSoundEffects();
    void playSoundEffect(QMediaPlayer*);

    QMediaPlayer* collectableSound = nullptr;
    QMediaPlayer* damageSound = nullptr;
    QMediaPlayer* jumpSound = nullptr;
    QMediaPlayer* shootSound = nullptr;
};

#endif // PLAYER_H
