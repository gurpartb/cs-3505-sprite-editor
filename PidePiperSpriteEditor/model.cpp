#include "model.h"

Model::Model()
{

    currentFrame = 0; //need to update still.

}

void Model::currentFrameUpdatePixmap(QPixmap *pixmap)
{

    std::cout << "UPDATING framesVector" << std::endl;
    framesVector[currentFrame]->addNewPixmap(pixmap);
    std::cout << "Current Frame:  " << currentFrame << std::endl;
    //std::cout << "updated framesVector" << std::endl;

}

void Model::createNewFrame()
{

    Frame *newFrame = new Frame();
    framesVector.push_back(newFrame);
    emit frameAdded();
}

///
/// \brief Model::updateCurrentFrameCounter
/// Updates the counter when creating a new frame, called from the create frame button.
///
void Model::updateCurrentFrameCounter()
{
    currentFrame++;
}

///
/// \brief Model::resetAll
/// Resets the frames and counter when user creates a new file.
///
void Model::resetAll()
{
    currentFrame = 0;

    //Ranged based for to delete the entire framesVector
    for (auto i: framesVector)
    {
        delete i;
    }

    framesVector.clear();
}

void Model::undo(){
    QPixmap* pix =framesVector[currentFrame]->undo();
    emit undoSignal(pix);
}

