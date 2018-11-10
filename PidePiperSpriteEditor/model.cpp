#include "model.h"

Model::Model()
{

    currentFrame = 0;

}

///
/// \brief Model::currentFrameUpdatePixmap:
/// Sends a new Pixmap to the current frame's Pixmap Vector. Allows for Undo.
/// \param pixmap:
/// New updated Pixmap.
///
void Model::currentFrameUpdatePixmap(QPixmap *pixmap)
{

    std::cout << "Model(currentFrameUpda) - UPDATING framesVector: " << currentFrame << std::endl;
    framesVector[currentFrame]->addNewPixmap(pixmap);

}

///
/// \brief Model::createNewFrame:
/// Creates a new frame for the UI.
///
void Model::createNewFrame()
{

    Frame *newFrame = new Frame();
    framesVector.push_back(newFrame);
    currentFrame = static_cast<unsigned int> (framesVector.size()-1);
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

void Model::undo()
{
    QPixmap* pix = framesVector[currentFrame]->undo();
    emit undoSignal(pix);
}

///
/// \brief Model::retrieveFrameNumberFromClickedPreview:
/// Gets what preview frame number was clicked on, to display it on the main drawing label.
/// \param frameNumber:
/// What preview frame was clicked to pull from the framesVector to display.
///
void Model::retrieveFrameNumberFromClickedPreview(int frameNumber)
{
    currentFrame = static_cast<unsigned int> (frameNumber);
    std::cout << "Model(retrieveFrameNumFrom) - emit change display with frame: " << currentFrame << std::endl;
    emit displaySelectedFrameFromPreview(framesVector[currentFrame]->getPixmap(), frameNumber);

}

