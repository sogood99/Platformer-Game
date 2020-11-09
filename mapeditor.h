#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include "utils.h"

#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QButtonGroup>
#include <QMenuBar>
#include <QMainWindow>
#include <QHash>

class MainWindow;

enum Categories{
    NoCategory,
    DeleteCategory,
    Collectables,
    Tiles,
    Flags,
    Monsters
};

// INUSE is for pair of tiles/stuff

class MapEditor: public QGraphicsView{
    Q_OBJECT
    /*
     * Used to Create, Edit Maps
    */
public:
    MapEditor(MainWindow *parent = nullptr);
    void open(QString name);
    bool save(QString name);
    QHash<QPair<int,int>, GameObject> hash;
    QHash<QPair<int,int>, QGraphicsPixmapItem*> displayObj;
    QHash<QPair<int,int>, QPair<int,int>> linkedObjects;
    QButtonGroup* categoryButtonGroup = nullptr;
    QWidget* selectWidget = nullptr;
    QWidget* tileSelector = nullptr;
    QWidget* collectableSelector = nullptr;
    QWidget* monsterSelector = nullptr;
    QWidget* flagSelector = nullptr;
    bool hasStartFlag = false;
    bool hasEndFlag = false;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void setupIcon();
    void setupTileSelector();
    void setupCollectableSelector();
    void setupMonsterSelector();
    void setupFlagSelector();
    void addObj(QPoint sceneP, GameObject object, bool isLink = false);
    QPoint positionToGrid(QPoint p);
    GameObject selectedObject = go_Empty;
    Categories selectedCategory = NoCategory;
public slots:
    void newCategorySelected(int cat);
    void newMapObjectSelected(int mapObj);
    QString setupSave();
    void setupNew();
    void setupOpen();
    void setupExit();
    void setupPlay();
private:
    QPoint lastMousePos;
    bool isMove = false;
    bool isDrag = false;
    bool secondTile = false;
    QPoint lastTile;
    GameObject lastObject;
    QHash<GameObject, QString> iconName;
    QGraphicsLineItem secondTileLine;
    MainWindow* mainWindow = nullptr;
};

#endif // MAPEDITOR_H
