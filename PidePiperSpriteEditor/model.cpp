#include "model.h"

Model::Model()
{

    currentFrame = 0; //need to update still.

}

void Model::currentFrameUpdatePixmap(QPixmap *pixmap)
{

    std::cout << "UPDATING framesVector" << std::endl;
    framesVector[currentFrame]->addNewPixmap(pixmap);
    std::cout << "updated framesVector" << std::endl;

}

void Model::createNewFrame()
{

    Frame *newFrame = new Frame();
    framesVector.push_back(newFrame);

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
    framesVector.clear();
}

