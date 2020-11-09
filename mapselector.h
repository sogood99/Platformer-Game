#ifndef MAPSELECTOR_H
#define MAPSELECTOR_H

#include "utils.h"
#include "mainwindow.h"
#include <QMainWindow>
#include <QVBoxLayout>
#include <QButtonGroup>

class MapSelector: public QWidget{
    /*
     * Used to select maps from saved maps.
     */
    Q_OBJECT
public:
    MapSelector(MainWindow* parent);
    QButtonGroup* mapButtons;
    QString findScore(QString fileName);
public slots:
    void addMap();
    void editMap();
    void removeMap();
    void playMap();
private:
    MainWindow* mainWindow = nullptr;
};

#endif // MAPSELECTOR_H
