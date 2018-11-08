#ifndef FRAME_H
#define FRAME_H
#include <vector>
#include <stack>
#include <QPixmap>
#include <QObject>
#include <iostream>

class Frame
{
    //Fields
    std::vector<QPixmap*> pixmapVector;

public:
    Frame();
    void addNewPixmap(QPixmap *pixmap);
    QPixmap* undo();

};

#endif // FRAME_H
