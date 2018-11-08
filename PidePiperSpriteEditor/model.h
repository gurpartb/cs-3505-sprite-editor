#ifndef MODEL_H
#define MODEL_H
#include <QPixmap>
#include <QObject>
#include "frame.h"

class Model : public QObject
{
    Q_OBJECT

public:
    Model();
    std::vector<Frame*> framesVector;

public slots:
    void currentFrameUpdatePixmap(QPixmap*);
    void createNewFrame();
    void updateCurrentFrameCounter();
    void resetAll();
    void undo();
signals:
    void frameAdded();
    void undoSignal(QPixmap*);

private:
    unsigned int currentFrame;

};

#endif // MODEL_H
