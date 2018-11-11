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
    void setCurrentFrameFromSlider(int);
    void resetAll();
    void undo();
    void retrieveFrameNumberFromClickedPreview(int);
    void retrieveFrameForPlayingAnimation(int);


signals:
    void frameAdded();
    void undoSignal(QPixmap*);
    void displaySelectedFrameFromPreview(QPixmap*, int);
    void sendFrameToAnimationPlayer(QPixmap*);

private:
    unsigned int currentFrame;

};

#endif // MODEL_H
