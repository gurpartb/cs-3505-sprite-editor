#ifndef FRAME_H
#define FRAME_H
#include <vector>
#include <stack>
#include <QPixmap>
#include <QObject>
#include <iostream>

class Frame
{
public:
    //Fields
    std::vector<QPixmap*> pixmapVector;

public:
    Frame();
    void addPixmapFromLoad(QPixmap* pixmap);
    void addNewPixmap(QPixmap *pixmap);
    QPixmap* undo();
    QPixmap* getPixmap();
    QPixmap* duplicate();
    QPixmap *currentMap;
};

#endif // FRAME_H
