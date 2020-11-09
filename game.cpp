#include "game.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QMenuBar>
#include <QMainWindow>
#include <QMediaPlaylist>

Game::Game(QString mapName,MainWindow *parent):QGraphicsView(parent), m_timer(this),mainWindow(parent){

    Game::mapName = mapName;
    QMainWindow* mainWindow = qobject_cast<QMainWindow*>(parent);
    QMenuBar* gameMenuBar = new QMenuBar(this);
    QMenu* gameActions = new QMenu("Actions",this);
    QAction* restartAction = new QAction("Restart",this);
    QAction* exitAction = new QAction("Exit", this);
    gameActions->addAction(restartAction);
    gameActions->addAction(exitAction);
    gameMenuBar->addMenu(gameActions);
    mainWindow->setMenuBar(gameMenuBar);

    connect(exitAction, &QAction::triggered, this, &Game::quit);
    connect(restartAction, &QAction::triggered, this, &Game::restart);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(QRectF(QPointF(X_MIN,Y_MIN), QPointF(X_MAX,Y_MAX)));
    setFixedSize(QSize(widthSize,heightSize));

    // load map for game scene

    m_player = new Player();

    loadGame(mapName);

    labels = new Labels(gameMenuBar->height(), m_player, this);

    QBrush brush(QColor(100,150,200));

    scene->setBackgroundBrush(brush);
    setScene(scene);

    m_timer.setInterval(TIME_STEP);
    m_timer.start();

    connect(&m_timer, &QTimer::timeout, this, &Game::movePlayer);

    horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    verticalScrollBar()->setStyleSheet("QScrollBar {width:0px;}");

    horizontalScrollBar()->hide();
    verticalScrollBar()->hide();

    initalizeLinkedLists();
    timeUsed.start();

    QMediaPlaylist *playlist = new QMediaPlaylist(this);
    playlist->addMedia(QUrl("qrc:/Music/Music/IntroMusic.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    gameBgmPlayer = new QMediaPlayer(this);
    gameBgmPlayer->setPlaylist(playlist);
    gameBgmPlayer->play();

    gameLostSound = new QMediaPlayer(this);
    gameLostSound->setMedia(QUrl("qrc:/Music/Music/LoseSound.mp3"));
    gameWonSound = new QMediaPlayer(this);
    gameWonSound->setMedia(QUrl("qrc:/Music/Music/WinSound.mp3"));
}

Game::~Game(){

}

void Game::keyPressEvent(QKeyEvent *event){
    // First give the important controls to player
    if (m_player == nullptr){
        return QGraphicsView::keyPressEvent(event);
    }
    switch (event->key()) {
    case Qt::Key_Left:{
        m_player->addDirection(-1);
        break;
    }
    case Qt::Key_Right:{
        m_player->addDirection(1);
        break;
    }
    case Qt::Key_Up:{
        m_player->jump();
        break;
    }
    case Qt::Key_F:{
        m_player->fire();
        break;
    }
    default:
        break;
    }
}

void Game::keyReleaseEvent(QKeyEvent *event){
    // First give the important controls to player
    if (m_player == nullptr){
        QGraphicsView::keyReleaseEvent(event);
        return;
    }
    if (event->key() == Qt::Key_Right){
        m_player->addDirection(-1);
    }else if (event->key() == Qt::Key_Left){
        m_player->addDirection(1);
    }
}

void Game::wheelEvent(QWheelEvent *event){
    // Disable scrolling by wheel
    Q_UNUSED(event);
    return;
}

void Game::movePlayer(){
    // Used to move camera with player and update labels
    if (m_player == nullptr){
        return;
    }

    QPointF topLeft = mapToScene(100,100);
    QPointF bottomRight = mapToScene(600,400);

    if (topLeft.x() > m_player->sceneBoundingRect().x()){
        auto dx = topLeft.x() - m_player->sceneBoundingRect().x();
        QScrollBar *hbar = horizontalScrollBar();
        hbar->setValue(hbar->value()-dx);
    }else if (bottomRight.x() < m_player->sceneBoundingRect().x()){
        auto dx = m_player->sceneBoundingRect().x() - bottomRight.x();
        QScrollBar *hbar = horizontalScrollBar();
        hbar->setValue(hbar->value()+dx);
    }
    if (topLeft.y() > m_player->sceneBoundingRect().y()){
        auto dy = topLeft.y() - m_player->sceneBoundingRect().y();
        QScrollBar *vbar = verticalScrollBar();
        vbar->setValue(vbar->value()-dy);
    }else if (bottomRight.y() < m_player->sceneBoundingRect().y()){
        auto dy = m_player->sceneBoundingRect().y() - bottomRight.y();
        QScrollBar *vbar = verticalScrollBar();
        vbar->setValue(vbar->value()+dy);
    }
    updateItems();
}

void Game::loadGame(QString file_name){
    // Used to load game from custom .map file
    QFile inputFile("SavedMaps/"+ file_name+ ".map");
    if (inputFile.open(QIODevice::ReadOnly)){
        QTextStream in(&inputFile);
        while (!in.atEnd()){
            QString line = in.readLine();
            QTextStream stream;
            stream.setString(&line);
            GameObject currentObj;
            int tempObj = -1;
            stream >> tempObj;

            currentObj = GameObject(tempObj);

            QPoint pointA, pointB;
            tempObj = -1;

            stream >> tempObj;
            pointA.setX(tempObj*80);
            tempObj = -1;

            stream >> tempObj;
            pointA.setY(tempObj*80);
            tempObj = -1;
            if (currentObj == go_MovingTile || currentObj == go_TeleportTile||
                    currentObj == go_Bat){
                stream >> tempObj;
                pointB.setX(tempObj*80);
                tempObj = -1;

                stream >> tempObj;
                pointB.setY(tempObj*80);
                tempObj = -1;
            }

            switch (currentObj) {
            case go_StartFlag:
                startFlag = new StartFlag(m_player, pointA);
                break;
            case go_EndFlag:
                endFlag = new EndFlag(this, pointA);
                break;
            case go_Gold:
                gameObjList.append(new Gold(pointA));
                break;
            case go_Potion:
                gameObjList.append(new Potion(pointA));
                break;
            case go_BulletCollect:
                gameObjList.append(new BulletCollect(pointA));
                break;
            case go_GrassTile:
                gameObjList.append(new GrassTile(pointA));
                break;
            case go_SandTile:
                gameObjList.append(new SandTile(pointA));
                break;
            case go_TeleportTile:
                gameObjList.append(new TeleportTile(pointA,pointB));
                break;
            case go_MovingTile:
                gameObjList.append(new MovingTile(pointA,pointB));
                break;
            case go_TrapTile:
                gameObjList.append(new TrapTile(pointA));
                break;
            case go_JumpTile:
                gameObjList.append(new JumpTile(pointA));
                break;
            case go_CollapseTile:
                gameObjList.append(new CollapseTile(pointA));
                break;
            case go_Mushroom:
                gameObjList.append(new Mushroom(pointA));
                break;
            case go_Goblin:
                gameObjList.append(new Goblin(m_player, pointA));
                break;
            case go_Bat:
                gameObjList.append(new Bat(pointA, pointB));
                break;
            default:
                break;
            }
        }
        inputFile.close();
    }
}

void Game::gameWin(){
    // Used when collide with endflag
    if (!gameWon){
        gameBgmPlayer->stop();
        gameWonSound->play();
        auto e_T = timeUsed.elapsed()*0.001;
        QMessageBox msgBox(this);
        msgBox.setText("Game Won");
        msgBox.setDetailedText("Total Score: " + QString::number(m_player->score+m_player->health)+"\n"
                               "Time Used: " + QString::number(e_T)+"s");
        msgBox.exec();
        gameWon = true;
        QFile file_scores("SavedMaps/"+mapName+".scores");
        file_scores.open(QIODevice::Append);

        QTextStream out(&file_scores);
        out << QString::number(m_player->score+m_player->health) << " "<<
                       QString::number(e_T)<<"\n";
        home();
    }
}

void Game::gameLost(){
    // Used when health < 0
    auto e_T = timeUsed.elapsed()*0.001;
    gameBgmPlayer->stop();
    gameLostSound->play();
    QMessageBox msgBox(this);
    msgBox.setText("Game Lost");
    msgBox.setDetailedText("Time Used: " + QString::number(e_T));
    msgBox.exec();
    home();
}

void Game::initalizeLinkedLists(){
    // Initializes linked list of gameobj
    if (!scene()){
        return;
    }
    scene()->addItem(startFlag);
    scene()->addItem(endFlag);
    foreach (GameObj* g, gameObjList){
        scene()->addItem(g);
    }
    scene()->addItem(m_player);

}

void Game::updateItems(){
    // Calling update on all objects in linked list
    for (auto g_it = gameObjList.begin(); g_it != gameObjList.end(); g_it++){
        auto g = (*g_it);
        if (g->m_bRemove){
            g->remove();
            g_it = gameObjList.erase(g_it);
        }else{
            g->update();
        }
    }
    if (m_player != nullptr){
        if (m_player->m_bRemove){
            m_player->remove();
            m_player = nullptr;
            labels->removePlayer();
            gameLost();
        }else{
            m_player->update();
        }
    }
    labels->update();
}

void Game::quit(){
    QMessageBox msgbox(this);
    msgbox.setText("Quit");
    msgbox.setInformativeText("Are You Sure You Want To Quit?");
    msgbox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgbox.setDefaultButton(QMessageBox::No);
    if (msgbox.exec() == QMessageBox::Yes){
        home();
    }
}

void Game::home(){
    mainWindow->home();
}

void Game::restart(){
    mainWindow->playMap(mapName);
}
