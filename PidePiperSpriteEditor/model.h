#ifndef MODEL_H
#define MODEL_H
#include <QPixmap>
#include <QObject>
#include "frame.h"
#include <QQueue>


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
    void saveAs();
    void storeNumberOfPixels(int);
    void openSprite(QQueue<int>*);
    void addPixmapFromDuplication(QPixmap*);
    void duplicateFrame();
    void addPixmapFromLoad(QPixmap*);
    void retrieveFrameForPlayingAnimation(int);
    void exportGifFromFrames(const char*);

signals:
    void frameAdded();
    void undoSignal(QPixmap*);
    void displaySelectedFrameFromPreview(QPixmap*, int);
    void sendSaveVector(std::vector<int>);
    void resetFrameCountFromOpen();
    void openFrame(QQueue<int>*, int);
    void duplicatedFrameAdded(QPixmap*);
    void enableButtonsFromLoad(int);
    void sendFrameToAnimationPlayer(QPixmap*);

private:
    unsigned int currentFrame;
    int numOfPixels;

};

#endif // MODEL_H
