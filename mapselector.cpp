#include "mapselector.h"
#include <QPushButton>
#include <QStringList>
#include <QDir>
#include <QSpacerItem>
#include <QLabel>
#include <QtMath>

MapSelector::MapSelector(MainWindow *parent):QWidget(parent), mainWindow(parent){
    QVBoxLayout* mapLayout = new QVBoxLayout(this);
    setLayout(mapLayout);

    QScrollArea* mapSelect = new QScrollArea(this);

    mapSelect->setGeometry(0,0,600,400);

    mapLayout->addWidget(mapSelect);

    QWidget* buttonWidgets = new QWidget(this);

    QPushButton* addButton = new QPushButton("Add Map", buttonWidgets);
    addButton->setStyleSheet("background-color:rgb(100,200,100)");
    QPushButton* editButton = new QPushButton("Edit Map", buttonWidgets);
    editButton->setStyleSheet("background-color:rgb(100,100,100)");
    QPushButton* deleteButton = new QPushButton("Delete Map", buttonWidgets);
    deleteButton->setStyleSheet("background-color:rgb(200,100,100)");
    QPushButton* playButton = new QPushButton("Play", buttonWidgets);
    playButton->setStyleSheet("background-color:rgb(200,200,200)");

    QHBoxLayout* buttonLayouts = new QHBoxLayout(buttonWidgets);
    buttonLayouts->addWidget(addButton);
    connect(addButton, &QPushButton::clicked, this, &MapSelector::addMap);
    connect(addButton, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    buttonLayouts->addWidget(editButton);
    connect(editButton, &QPushButton::clicked, this, &MapSelector::editMap);
    connect(editButton, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    buttonLayouts->addWidget(deleteButton);
    connect(deleteButton, &QPushButton::clicked, this, &MapSelector::removeMap);
    connect(deleteButton, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    buttonLayouts->addWidget(playButton);
    connect(playButton, &QPushButton::clicked, this, &MapSelector::playMap);
    connect(playButton, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);

    buttonWidgets->setLayout(buttonLayouts);

    mapLayout->addWidget(buttonWidgets);

    mapButtons = new QButtonGroup(mapSelect);
    mapButtons->setExclusive(true);

    QDir dir("SavedMaps/");
    QStringList filters;
    filters << "*.map";
    dir.setNameFilters(filters);
    auto filesList = dir.entryList();

    QWidget* scrollFile = new QWidget(this);
    scrollFile->setMinimumHeight(2000);
    scrollFile->setFixedWidth(765);

    QVBoxLayout* fileLayout = new QVBoxLayout(scrollFile);
    fileLayout->setAlignment(Qt::AlignTop);

    foreach (auto str, filesList) {
        QPushButton* but = new QPushButton(scrollFile);
        but->setCheckable(true);
        mapButtons->addButton(but);
        but->setText(str.left(str.length()-4));
        but->setStyleSheet("color: rgba(255,255,255,0)");

        QLabel* titleLabel = new QLabel(str.left(str.length()-4),but);
        titleLabel->move(10,20);
        titleLabel->setStyleSheet("color: rgba(0,0,0,255)");

        QLabel* scoreLabel = new QLabel(findScore(str.left(str.length()-4)),but);
        scoreLabel->move(500,20);
        scoreLabel->setStyleSheet("color: rgba(0,0,0,255)");

        but->setMinimumHeight(50);
        but->setFixedWidth(750);
        fileLayout->addWidget(but);
        mapSelect->setLayout(fileLayout);
        connect(but, &QPushButton::clicked, mainWindow, &MainWindow::buttonSound);
    }
    scrollFile->setLayout(fileLayout);
    mapSelect->setWidget(scrollFile);
}

QString MapSelector::findScore(QString fileName){
    /* used to collect scores of fileName and output highest
     * (score, time) if score is > other score
     * If the same, select lowest time
     */
    fileName.append(QString(".scores"));
    fileName = QString("SavedMaps/")+fileName;
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly)){
        int currentHighScore = 0;
        qreal currentTimeUsed = 20000;
        bool isSet = false;
        QTextStream in(&inputFile);
        while (!in.atEnd()){
            QString line = in.readLine();
            if (!line.isNull()){
                isSet = true;
                int nextScore = 0;
                qreal nextTime = 20000;

                QTextStream ss;
                ss.setString(&line);
                ss>>nextScore>>nextTime;

                if (nextScore > currentHighScore){
                    currentHighScore = nextScore;
                    currentTimeUsed = nextTime;
                }else if (nextScore == currentHighScore){
                    if (currentTimeUsed > nextTime){
                        currentTimeUsed = nextTime;
                    }
                }
            }
        }
        if (isSet){
            return QString("High Score: ") + QString::number(currentHighScore)
                    +", Best Time: " + QString::number(currentTimeUsed)+"s";
        }
    }
    return "High Score: 0, Best Time: None";
}

void MapSelector::addMap(){
    // Used by qAction to create new map
    mainWindow->addMap();
}

void MapSelector::editMap(){
    // Used by qAction to edit map
    auto button = mapButtons->checkedButton();
    if (button != nullptr){
        mainWindow->editMap(button->text());
    }
}

void MapSelector::removeMap(){
    // Used by qAction to remove map
    auto button = mapButtons->checkedButton();
    if (!(button == nullptr)){
        QFile file_map("SavedMaps/"+button->text()+".map");
        file_map.remove();
        QFile file_scores("SavedMaps/"+button->text()+".scores");
        file_scores.remove();
        delete button;
    }
}

void MapSelector::playMap(){
    auto button = mapButtons->checkedButton();
    if (button != nullptr){
        mainWindow->playMap(button->text());
    }
}
