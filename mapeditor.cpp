#include "mapeditor.h"
#include "mainwindow.h"
#include <QFile>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>

MapEditor::MapEditor(MainWindow* parent):QGraphicsView(parent), mainWindow(parent){
    setupIcon();
    parent->setFixedSize(800,600);
    setFixedSize(800,600);
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(QRectF(QPointF(X_MIN,Y_MIN), QPointF(X_MAX,Y_MAX)));
    QBrush brush(QColor(255,255,255));
    scene->setBackgroundBrush(brush);

    auto origin_x = new QGraphicsLineItem(0, Y_MIN, 0, Y_MAX);
    origin_x->setPen(QPen(QColor(0,0,0,255)));
    scene->addItem(origin_x);

    for (int i = 80; i < X_MAX; i += 80){
        auto line = new QGraphicsLineItem(i, Y_MIN, i, Y_MAX);
        line->setPen(QPen(QColor(0,0,0,100)));
        scene->addItem(line);
    }
    for (int i = -80; i > X_MIN; i -= 80){
        auto line = new QGraphicsLineItem(i, Y_MIN, i, Y_MAX);
        line->setPen(QPen(QColor(0,0,0,100)));
        scene->addItem(line);
    }

    auto origin_y = new QGraphicsLineItem(X_MIN, 0, X_MAX, 0);
    origin_y->setPen(QPen(QColor(0,0,0,255)));
    scene->addItem(origin_y);

    for (int j = 80; j < Y_MAX; j += 80){
        auto line = new QGraphicsLineItem(X_MIN, j, X_MAX, j);
        line->setPen(QPen(QColor(0,0,0,100)));
        scene->addItem(line);
    }
    for (int j = -80; j > Y_MIN; j -= 80){
        auto line = new QGraphicsLineItem(X_MIN, j, X_MAX, j);
        line->setPen(QPen(QColor(0,0,0,100)));
        scene->addItem(line);
    }

    setScene(scene);

    horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    verticalScrollBar()->setStyleSheet("QScrollBar {width:0px;}");

    horizontalScrollBar()->hide();
    verticalScrollBar()->hide();

    QLabel* label = new QLabel(this);

    selectWidget = new QWidget(label);
    QSignalMapper* signalMapper = new QSignalMapper(this);
    categoryButtonGroup = new QButtonGroup(this);

    QVBoxLayout* selectedCategory = new QVBoxLayout(this);
    selectedCategory->setSpacing(0);
    QRadioButton* tile = new QRadioButton("Tiles",this);
    categoryButtonGroup->addButton(tile);
    QFont font = tile->font();
    font.setPixelSize(16);
    font.setBold(true);
    tile->setFont(font);
    selectedCategory->addWidget(tile);
    QRadioButton* collectable = new QRadioButton("Collectables",this);
    categoryButtonGroup->addButton(collectable);
    selectedCategory->addWidget(collectable);
    collectable->setFont(font);
    QRadioButton* monster = new QRadioButton("Monster",this);
    categoryButtonGroup->addButton(monster);
    selectedCategory->addWidget(monster);
    monster->setFont(font);;
    QRadioButton* delButton = new QRadioButton("Delete",this);
    categoryButtonGroup->addButton(delButton);
    selectedCategory->addWidget(delButton);
    delButton->setStyleSheet("color: red");
    delButton->setFont(font);

    signalMapper->setMapping(tile, Tiles);
    signalMapper->setMapping(collectable, Collectables);
    signalMapper->setMapping(monster, Monsters);
    signalMapper->setMapping(delButton, DeleteCategory);

    connect(tile, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(tile, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(collectable, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(collectable, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(monster, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(monster, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(delButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(delButton, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(newCategorySelected(int)));

    QPixmap pixmap(800,250);
    pixmap.fill(QColor(20,20,20,200));
    label->setGeometry(0,450,800,150);
    label->setPixmap(pixmap);
    label->show();

    selectWidget->setLayout(selectedCategory);
    selectWidget->move(0,10);
    selectWidget->raise();
    selectWidget->show();

    tileSelector = new QWidget(label);
    tileSelector->move(125,10);
    setupTileSelector();

    collectableSelector = new QWidget(label);
    collectableSelector->move(125,10);
    setupCollectableSelector();

    monsterSelector = new QWidget(label);
    monsterSelector->move(125,10);
    setupMonsterSelector();

    flagSelector = new QWidget(this);
    flagSelector->move(650,0);
    setupFlagSelector();

    QMenuBar* menuBar = new QMenuBar(this);
    parent->setMenuBar(menuBar);
    QMenu *fileMenu = new QMenu("File", this);
    menuBar->addMenu(fileMenu);
    QAction* playAction = new QAction("Play");
    fileMenu->addAction(playAction);
    QAction* newAction = new QAction("New");
    fileMenu->addAction(newAction);
    QAction* openAction = new QAction("Open");
    fileMenu->addAction(openAction);
    QAction* saveAction = new QAction("Save");
    fileMenu->addAction(saveAction);
    QAction* quitAction = new QAction("Back");
    fileMenu->addAction(quitAction);

    connect(playAction, &QAction::triggered, this, &MapEditor::setupPlay);
    connect(playAction, &QAction::triggered, mainWindow, &MainWindow::buttonSound);
    connect(saveAction, &QAction::triggered, this, &MapEditor::setupSave);
    connect(saveAction, &QAction::triggered, mainWindow, &MainWindow::buttonSound);
    connect(newAction, &QAction::triggered, this, &MapEditor::setupNew);
    connect(newAction, &QAction::triggered, mainWindow, &MainWindow::buttonSound);
    connect(openAction, &QAction::triggered, this, &MapEditor::setupOpen);
    connect(openAction, &QAction::triggered, mainWindow, &MainWindow::buttonSound);
    connect(quitAction, &QAction::triggered, this, &MapEditor::setupExit);
    connect(quitAction, &QAction::triggered, mainWindow, &MainWindow::buttonSound);
}

void MapEditor::mousePressEvent(QMouseEvent *event){
    // If press without shift means placing things
    // With shift means moving map

    if (event->button() == Qt::LeftButton){
        if (event->modifiers() & Qt::ShiftModifier){
            isMove = true;
        }else{
            isDrag = true;
            auto position_on_grid = positionToGrid(mapToScene(event->pos()).toPoint());
            auto pair_on_grid = QPair<int,int>(position_on_grid.x(),position_on_grid.y());

            if (hash[pair_on_grid] == go_LINKED){
                QGraphicsView::mousePressEvent(event);
                isDrag = false;
                return;
            }

            if (selectedCategory == DeleteCategory){
                addObj(positionToGrid(mapToScene(event->pos()).toPoint()), go_Empty);
            }else if (selectedObject != go_Empty){
                if (selectedObject == go_MovingTile || selectedObject == go_TeleportTile ||
                        selectedObject == go_Bat){
                    secondTile = true;
                    QPoint p = positionToGrid(mapToScene(event->pos()).toPoint());
                    auto x_p = p.x()*80+40; auto y_p = p.y()*80+40;
                    secondTileLine.setLine(x_p,y_p,x_p,y_p);
                    lastTile = p;
                    scene()->addItem(&secondTileLine);
                }
                addObj(positionToGrid(mapToScene(event->pos()).toPoint()), selectedObject);
            }
        }
        lastMousePos = event->pos();
    }
    QGraphicsView::mousePressEvent(event);
}

void MapEditor::mouseMoveEvent(QMouseEvent *event){
    // see mouse pressed event
    QPoint diff = lastMousePos-event->pos();
    if (isMove){
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + diff.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() + diff.y());
        lastMousePos = event->pos();
    }else if (secondTile){
        QPoint p = positionToGrid(mapToScene(event->pos()).toPoint());
        auto x_p = p.x()*80+40; auto y_p = p.y()*80+40;
        auto pen = QPen(QColor(0,0,200));
        pen.setWidth(5);
        secondTileLine.setPen(pen);
        secondTileLine.setLine(lastTile.x()*80+40,lastTile.y()*80+40,x_p,y_p);
    }else if (isDrag){
        auto position_on_grid = positionToGrid(mapToScene(event->pos()).toPoint());
        auto pair_on_grid = QPair<int,int>(position_on_grid.x(),position_on_grid.y());
        if (hash[pair_on_grid] == go_LINKED){
            return;
        }
        if (selectedCategory == DeleteCategory){
            addObj(positionToGrid(mapToScene(event->pos()).toPoint()), go_Empty);
        }else if (selectedObject != go_Empty){
            addObj(positionToGrid(mapToScene(event->pos()).toPoint()), selectedObject);
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}


void MapEditor::mouseReleaseEvent(QMouseEvent *event){
    // See mouse pressed event
    if (secondTile){
        scene()->removeItem(&secondTileLine);
        if (positionToGrid(mapToScene(event->pos()).toPoint()) == lastTile){
            addObj(positionToGrid(mapToScene(event->pos()).toPoint()), go_Empty);
        }else{
            auto p = positionToGrid(mapToScene(event->pos()).toPoint());
            linkedObjects.insert(QPair<int,int>(lastTile.x(),lastTile.y()),QPair<int,int>(p.x(),p.y()));
            addObj(positionToGrid(mapToScene(event->pos()).toPoint()), selectedObject, true);
        }
    }
    isMove = false;
    isDrag = false;
    secondTile = false;
    QGraphicsView::mouseReleaseEvent(event);
}

void MapEditor::wheelEvent(QWheelEvent *event){
    /*
     * Used to scale screen when moves wheel
     */
    const qreal factor = 1.05;
    if (qFabs(event->angleDelta().y())>0.2){
        if (event->angleDelta().y() > 0){
            scale(factor,factor);
        }else{
            auto f = 1/factor;
            scale(f,f);
        }
    }
}

void MapEditor::newCategorySelected(int cat){
    /*
     * Called when new category in radio button group is pressed
     */
    if (Categories(cat) == Tiles){
        selectedCategory = Tiles;
        tileSelector->show();
        collectableSelector->hide();
        monsterSelector->hide();
    }else if (Categories(cat) == Collectables){
        selectedCategory = Collectables;
        tileSelector->hide();
        collectableSelector->show();
        monsterSelector->hide();
    }else if (Categories(cat) == Monsters){
        selectedCategory = Monsters;
        tileSelector->hide();
        collectableSelector->hide();
        monsterSelector->show();
    }else if (Categories(cat) == DeleteCategory){
        selectedCategory = DeleteCategory;
        tileSelector->hide();
        collectableSelector->hide();
        monsterSelector->hide();
    }else if (Categories(cat) == Flags){
        selectedCategory = Flags;
        tileSelector->hide();
        collectableSelector->hide();
        monsterSelector->hide();
        QAbstractButton* checked = categoryButtonGroup->checkedButton();
        if (checked){
            categoryButtonGroup->setExclusive(false);
            checked->setChecked(false);
            categoryButtonGroup->setExclusive(true);
        }
    }
}


void MapEditor::newMapObjectSelected(int mapObj){
    // Used when a button in a category is selected
    selectedObject = GameObject(mapObj);
}

QString MapEditor::setupSave(){
    // Used by qAction when want to save
    QString saveFileName = QInputDialog::getText(this, tr("Save"),
                                             tr("Save File Name:"), QLineEdit::Normal);
    if (!saveFileName.isEmpty()){
        if (save(saveFileName)){
            return saveFileName;
        }
    }
    return "";
}

void MapEditor::setupNew(){
    // Used by qAction when want to make new map
    for (auto key : hash.keys()) {
        hash.remove(key);
    }
    for (auto key : displayObj.keys()){
        scene()->removeItem(displayObj.value(key));
        delete displayObj.value(key);
        displayObj.remove(key);
    }
    for (auto key : linkedObjects.keys()){
        linkedObjects.remove(key);
    }
    hasStartFlag = false;
    hasEndFlag = false;
}

void MapEditor::setupOpen(){
    // Used by qAction when want to make open old map
    QString saveFileName = QInputDialog::getText(this, tr("Save"),
                                             tr("Save File Name:"), QLineEdit::Normal);
    if (!saveFileName.isEmpty()){
        open(saveFileName);
    }
}

void MapEditor::setupPlay(){
    // Used by qAction to play current map
    QString mapName = setupSave();
    if (!mapName.isEmpty()){
        mainWindow->playMap(mapName);
    }
}

void MapEditor::setupExit(){
    // Used by qAction to exit back home
    mainWindow->home();
}

bool MapEditor::save(QString name){
    /*
     * How the tiles on screen are saved, format is SavedMap/(file name).map
     * A SavedMap/(file name).score is created to collect the score of players
     * _ _ _ (optional) _ _
     * First is enum of the GameObject
     * Second and third is QPoint
     * If there is a link (bat, movingTile, teleport tile), there is fouth and fifth int
     */
    if (!(hasStartFlag && hasEndFlag)){
        qDebug()<<hasStartFlag<<hasEndFlag;
        QMessageBox msgBox(this);
        msgBox.setText("Unable To Save");
        msgBox.setInformativeText("Missing Start or End Flag");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return false;
    }

    QDir directory;
    directory.mkdir("SavedMaps");
    QFile file("SavedMaps/"+name+".map");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox msgBox(this);
        msgBox.setText("Unable To Save");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return false;
    }
    QTextStream out(&file);

    for (auto key : hash.keys()) {
        if (hash.value(key) != go_Empty && hash.value(key) != go_LINKED){
            out<< hash.value(key)<< " " <<key.first<<" "<< key.second;
            if (hash.value(key) == go_MovingTile || hash.value(key) == go_TeleportTile
                    || hash.value(key) == go_Bat){
                out << " " << linkedObjects.value(key).first<<" "<<
                       linkedObjects.value(key).second;
            }
            out<< "\n";
        }
    }
    QFile file_scores("SavedMaps/"+name+".scores");
    file_scores.open(QIODevice::WriteOnly);
    file_scores.write("");

    QMessageBox msgBox(this);
    msgBox.setText("Save Successful");
    msgBox.setInformativeText("Succesfully Saved As:\n" + name);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    return true;
}

void MapEditor::open(QString name){
    // Opens the .map file, see save() function for comment
    QFile inputFile("SavedMaps/"+ name+ ".map");
    if (inputFile.open(QIODevice::ReadOnly)){
        setupNew();
        QTextStream in(&inputFile);
        while (!in.atEnd()){
            QString line = in.readLine();
            if (line.isNull()){
                QMessageBox msgBox(this);
                msgBox.setText("Open Successful");
                msgBox.setInformativeText("Open:\n" + name);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
                return;
            }
            GameObject currentObj;
            int tempObj = -1;
            QTextStream stream;
            stream.setString(&line);

            stream >> tempObj;
            currentObj = GameObject(tempObj);

            QPoint pointA, pointB;
            tempObj = -1;

            stream >> tempObj;
            pointA.setX(tempObj);
            tempObj = -1;

            stream >> tempObj;
            pointA.setY(tempObj);
            tempObj = -1;

            addObj(pointA, currentObj);

            if (currentObj == go_MovingTile || currentObj == go_TeleportTile||
                    currentObj == go_Bat){
                stream >> tempObj;
                pointB.setX(tempObj);
                tempObj = -1;

                stream >> tempObj;
                pointB.setY(tempObj);
                tempObj = -1;
                addObj(pointB, currentObj, true);
                linkedObjects.insert(QPair<int,int>(pointA.x(),pointA.y()),
                                     QPair<int,int>(pointB.x(),pointB.y()));
            }
        }
        inputFile.close();
        QMessageBox msgBox(this);
        msgBox.setText("Open Successful");
        msgBox.setInformativeText("Open:\n" + name);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}

inline void MapEditor::setupIcon(){
    // Icons!
    iconName.insert(go_EndFlag, ":/Pictures/TilePictures/TilePictures/EndTile.png");
    iconName.insert(go_StartFlag, ":/Pictures/TilePictures/TilePictures/StartTile.png");
    iconName.insert(go_BulletCollect, ":/Pictures/ConsumablePictures/ConsumablePictures/Bullet/Bullet.png");
    iconName.insert(go_Potion, ":/Pictures/ConsumablePictures/ConsumablePictures/Potion/Potion.png");
    iconName.insert(go_TrapTile, ":/Pictures/TilePictures/TilePictures/Spikes.png");
    iconName.insert(go_Gold, ":/Pictures/ConsumablePictures/ConsumablePictures/Coin/Coin_0.png");
    iconName.insert(go_GrassTile, ":/Pictures/TilePictures/TilePictures/GrassTile.png");
    iconName.insert(go_MovingTile, ":/Pictures/TilePictures/TilePictures/IceTile.png");
    iconName.insert(go_JumpTile, ":/Pictures/TilePictures/TilePictures/JumpTile_0.png");
    iconName.insert(go_SandTile, ":/Pictures/TilePictures/TilePictures/SandTile.png");
    iconName.insert(go_TeleportTile, ":/Pictures/TilePictures/TilePictures/TeleportTile.png");
    iconName.insert(go_Mushroom, ":/Pictures/Monster/Running/MonsterPictures/Mushroom_0.png");
    iconName.insert(go_Bat, ":/Pictures/Monster/Running/MonsterPictures/Bat_0.png");
    iconName.insert(go_Goblin, ":/Pictures/Monster/Running/MonsterPictures/Goblin_000.png");
    iconName.insert(go_CollapseTile, ":/Pictures/TilePictures/TilePictures/CollapseTile.png");
}

void MapEditor::addObj(QPoint gridP, GameObject object, bool isLink){
    /*
     * Assume in grid, shows when a new item is added
     */

    QPair<int,int> key = QPair<int,int>(gridP.x(),gridP.y());

    auto pair_on_grid = QPair<int,int>(gridP.x(),gridP.y());

    if (hash[pair_on_grid] == go_StartFlag){
        hasStartFlag = false;
    }else if (hash[pair_on_grid] == go_EndFlag){
        hasEndFlag = false;
    }

    if (object == go_StartFlag){
        if (hasStartFlag){
            QMessageBox msgBox(this);
            msgBox.setText("Start Flag Already Exists");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
            return;
        }
        hasStartFlag = true;
    }else if (object == go_EndFlag){
        if (hasEndFlag){
            QMessageBox msgBox(this);
            msgBox.setText("End Flag Already Exists");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
            return;
        }
        hasEndFlag = true;
    }

    if (hash[pair_on_grid] == go_MovingTile || hash[pair_on_grid] == go_TeleportTile||
            hash[pair_on_grid] == go_Bat){
        auto linked_on_grid = linkedObjects[pair_on_grid];
        auto linked_position_on_grid = QPoint(linked_on_grid.first, linked_on_grid.second);
        addObj(linked_position_on_grid, go_Empty);
    }

    if (isLink){
        hash[key] = go_LINKED;
    }else{
        hash[key] = object;
    }

    QGraphicsPixmapItem* pixmapItem;

    if (displayObj.value(key) == nullptr){
        if (object == go_Empty){
            return;
        }
        pixmapItem = new QGraphicsPixmapItem;
        auto pixmap = QPixmap(iconName.value(object));
        pixmap = pixmap.scaled(80,80);
        pixmapItem->setPixmap(pixmap);
        pixmapItem->moveBy(gridP.x()*80,gridP.y()*80);
        displayObj[key] = pixmapItem;
        scene()->addItem(pixmapItem);
    }else{
        pixmapItem = displayObj[key];
        QPixmap pixmap;
        if (object == go_Empty){
            pixmap.fill(QColor(255,255,255));
        }else{
            pixmap = QPixmap(iconName.value(object));
            pixmap = pixmap.scaled(80,80);
        }
        pixmapItem->setPixmap(pixmap);
    }
    if (isLink){
        pixmapItem->setOpacity(0.5);
    }else{
        pixmapItem->setOpacity(1);
    }
}

QPoint MapEditor::positionToGrid(QPoint p){
    // Maps 80x80 to 1x1 (Tile size)
    if (p.x() < 0){
        p -= QPoint(80,0);
    }
    if (p.y() < 0){
        p -= QPoint(0,80);
    }
    p.setX(p.x()/80);
    p.setY(p.y()/80);
    return p;
}

void MapEditor::setupTileSelector(){
    // Setups tile buttons
    QSignalMapper* sigMap = new QSignalMapper(tileSelector);
    QHBoxLayout* tilesLayout = new QHBoxLayout(tileSelector);

    QPushButton* grassTile = new QPushButton(tileSelector);
    QPixmap pixmap = QPixmap(iconName.value(go_GrassTile));
    pixmap = pixmap.scaled(80,80);
    grassTile->setIcon(pixmap);
    grassTile->setIconSize(QSize(40,40));

    tilesLayout->addWidget(grassTile);
    tilesLayout->addSpacing(1);

    QPushButton* sandTile = new QPushButton(tileSelector);
    pixmap = QPixmap(iconName.value(go_SandTile));
    pixmap = pixmap.scaled(80,80);
    sandTile->setIcon(pixmap);
    sandTile->setIconSize(QSize(40,40));

    tilesLayout->addWidget(sandTile);
    tilesLayout->addSpacing(1);

    QPushButton* teleportTile = new QPushButton(tileSelector);
    pixmap = QPixmap(iconName.value(go_TeleportTile));
    pixmap = pixmap.scaled(80,80);
    teleportTile->setIcon(pixmap);
    teleportTile->setIconSize(QSize(40,40));

    tilesLayout->addWidget(teleportTile);
    tilesLayout->addSpacing(1);

    QPushButton* movingTile = new QPushButton(tileSelector);
    pixmap = QPixmap(iconName.value(go_MovingTile));
    pixmap = pixmap.scaled(80,80);
    movingTile->setIcon(pixmap);
    movingTile->setIconSize(QSize(40,40));

    tilesLayout->addWidget(movingTile);
    tilesLayout->addSpacing(1);

    QPushButton* jumpTile = new QPushButton(tileSelector);
    pixmap = QPixmap(iconName.value(go_JumpTile));
    pixmap = pixmap.scaled(80,80);
    jumpTile->setIcon(pixmap);
    jumpTile->setIconSize(QSize(40,40));

    tilesLayout->addWidget(jumpTile);
    tilesLayout->addSpacing(1);

    QPushButton* collapseTile = new QPushButton(tileSelector);
    pixmap = QPixmap(iconName.value(go_CollapseTile));
    pixmap = pixmap.scaled(80,80);
    collapseTile->setIcon(pixmap);
    collapseTile->setIconSize(QSize(40,40));

    tilesLayout->addWidget(collapseTile);
    tilesLayout->addSpacing(1);

    QPushButton* trapTile = new QPushButton(tileSelector);
    pixmap = QPixmap(iconName.value(go_TrapTile));
    pixmap = pixmap.scaled(80,80);
    trapTile->setIcon(pixmap);
    trapTile->setIconSize(QSize(40,40));

    tilesLayout->addWidget(trapTile);

    tileSelector->setLayout(tilesLayout);
    tileSelector->hide();

    sigMap->setMapping(grassTile, go_GrassTile);
    sigMap->setMapping(sandTile, go_SandTile);
    sigMap->setMapping(teleportTile, go_TeleportTile);
    sigMap->setMapping(movingTile, go_MovingTile);
    sigMap->setMapping(jumpTile, go_JumpTile);
    sigMap->setMapping(collapseTile, go_CollapseTile);
    sigMap->setMapping(trapTile, go_TrapTile);

    connect(grassTile, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(grassTile, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(sandTile, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(sandTile, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(teleportTile, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(teleportTile, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(movingTile, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(movingTile, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(jumpTile, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(jumpTile, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(collapseTile, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(collapseTile, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(trapTile, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(trapTile, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(sigMap, SIGNAL(mapped(int)), this, SLOT(newMapObjectSelected(int)));
}

void MapEditor::setupCollectableSelector(){
    // Setups collectable buttons
    QSignalMapper* sigMap = new QSignalMapper(collectableSelector);
    QHBoxLayout* collectableLayout = new QHBoxLayout(collectableSelector);

    QPushButton* goldCollect = new QPushButton(collectableSelector);
    QPixmap pixmap = QPixmap(iconName.value(go_Gold));
    pixmap = pixmap.scaled(80,80);
    goldCollect->setIcon(pixmap);
    goldCollect->setIconSize(QSize(60,60));

    collectableLayout->addWidget(goldCollect);
    collectableLayout->addSpacing(5);

    QPushButton* potionCollect = new QPushButton(collectableSelector);
    pixmap = QPixmap(iconName.value(go_Potion));
    pixmap = pixmap.scaled(80,80);
    potionCollect->setIcon(pixmap);
    potionCollect->setIconSize(QSize(60,60));

    collectableLayout->addWidget(potionCollect);
    collectableLayout->addSpacing(5);

    QPushButton* bulletCollect = new QPushButton(collectableSelector);
    pixmap = QPixmap(iconName.value(go_BulletCollect));
    pixmap = pixmap.scaled(80,80);
    bulletCollect->setIcon(pixmap);
    bulletCollect->setIconSize(QSize(60,60));

    collectableLayout->addWidget(bulletCollect);
    collectableSelector->setLayout(collectableLayout);
    collectableSelector->hide();

    sigMap->setMapping(goldCollect, go_Gold);
    sigMap->setMapping(potionCollect, go_Potion);
    sigMap->setMapping(bulletCollect, go_BulletCollect);

    connect(goldCollect, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(goldCollect, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(potionCollect, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(potionCollect, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(bulletCollect, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(bulletCollect, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(sigMap, SIGNAL(mapped(int)), this, SLOT(newMapObjectSelected(int)));
}

void MapEditor::setupMonsterSelector(){
    // Setups monster buttons
    QSignalMapper* sigMap = new QSignalMapper(monsterSelector);
    QHBoxLayout* monsterLayout = new QHBoxLayout(monsterSelector);

    QPushButton* mushroomMons = new QPushButton(monsterSelector);
    QPixmap pixmap = QPixmap(iconName.value(go_Mushroom));
    pixmap = pixmap.scaled(80,80);
    mushroomMons->setIcon(pixmap);
    mushroomMons->setIconSize(QSize(60,60));

    monsterLayout->addWidget(mushroomMons);
    monsterLayout->addSpacing(5);

    QPushButton* goblinMons = new QPushButton(monsterSelector);
    pixmap = QPixmap(iconName.value(go_Goblin));
    pixmap = pixmap.scaled(80,80);
    goblinMons->setIcon(pixmap);
    goblinMons->setIconSize(QSize(60,60));

    monsterLayout->addWidget(goblinMons);
    monsterLayout->addSpacing(5);

    QPushButton* batMons = new QPushButton(monsterSelector);
    pixmap = QPixmap(iconName.value(go_Bat));
    pixmap = pixmap.scaled(80,80);
    batMons->setIcon(pixmap);
    batMons->setIconSize(QSize(60,60));

    monsterLayout->addWidget(batMons);

    monsterSelector->setLayout(monsterLayout);
    monsterSelector->hide();

    sigMap->setMapping(mushroomMons, go_Mushroom);
    sigMap->setMapping(goblinMons, go_Goblin);
    sigMap->setMapping(batMons, go_Bat);

    connect(mushroomMons, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(mushroomMons, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(goblinMons, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(goblinMons, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(batMons, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(batMons, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(sigMap, SIGNAL(mapped(int)), this, SLOT(newMapObjectSelected(int)));
}


void MapEditor::setupFlagSelector(){
    // Setups flag buttons
    QSignalMapper* sigMap = new QSignalMapper(flagSelector);
    QHBoxLayout* flagLayout = new QHBoxLayout(flagSelector);
    QPushButton* startFlag = new QPushButton(flagSelector);
    QPushButton* endFlag = new QPushButton(flagSelector);
    auto pixmap = QPixmap(iconName.value(go_StartFlag));
    pixmap = pixmap.scaled(30,30);

    startFlag->setIcon(pixmap);
    startFlag->setIconSize(QSize(30,30));
    startFlag->setStyleSheet("background-color:rgba(200,200,200,200)");
    flagLayout->addWidget(startFlag);

    pixmap = QPixmap(iconName.value(go_EndFlag));
    pixmap = pixmap.scaled(30,30);

    endFlag->setIcon(pixmap);
    endFlag->setIconSize(QSize(30,30));
    endFlag->setStyleSheet("background-color:rgba(200,200,200,200)");
    flagLayout->addWidget(endFlag);

    sigMap->setMapping(startFlag, go_StartFlag);
    sigMap->setMapping(endFlag, go_EndFlag);

    QSignalMapper* categoryMap = new QSignalMapper(flagSelector);
    categoryMap->setMapping(startFlag, Flags);
    categoryMap->setMapping(endFlag, Flags);
    connect(startFlag, SIGNAL(clicked()), categoryMap, SLOT(map()));
    connect(startFlag, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(endFlag, SIGNAL(clicked()), categoryMap, SLOT(map()));
    connect(endFlag, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    connect(categoryMap, SIGNAL(mapped(int)), this, SLOT(newCategorySelected(int)));

    connect(startFlag, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(endFlag, SIGNAL(clicked()), sigMap, SLOT(map()));
    connect(sigMap, SIGNAL(mapped(int)), this, SLOT(newMapObjectSelected(int)));
}
