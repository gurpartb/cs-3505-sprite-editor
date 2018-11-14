#include "model.h"
#include<math.h>
#include "gif.h"
#include <QBuffer>
///
/// \brief Model::Model
/// This model class communicates with the drawing window and the main window
/// and acts as a container for the each frame of the current sprite being edited
/// Stores each frames as an object in a frame vector class
///
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
/// \brief Model::setCurrentFrameCounter
/// Sets the current frame, called from changing the animation slider
///
void Model::setCurrentFrameFromSlider(int frameNumber)
{
    currentFrame = static_cast<unsigned int> (frameNumber);
    emit displaySelectedFrameFromPreview(framesVector[currentFrame]->getPixmap(), frameNumber);
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
///
/// \brief Model::undo
/// Slot called when undo is pressed
/// returns the last saved pixmap from the current slected frame
///
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
    std::cout << "Model(retrieveFrameNumFromClickedPreview) - emit change display with frame: " << currentFrame << std::endl;
    emit displaySelectedFrameFromPreview(framesVector[currentFrame]->getPixmap(), frameNumber);
}
///
/// \brief Model::saveCurrentFrame
/// \param map the current map after drawing as comleted
/// Slot called as soon as the user clicks on the drawing window
/// Saves the pixmap before the user draws on the window
///
void Model::saveCurrentFrame(QPixmap* map)
{
    framesVector[currentFrame]->currentMap = new QPixmap(*map);
}

///
/// \brief Model::saveAs
/// Stores the information of the current sprite into an integer vector
///
void Model::saveAs()
{
    std::vector<int> saveVector;
    double pixmapSize = pow(numOfPixels, .5);
    //push back height and width of the frame
    saveVector.push_back(signed(pixmapSize));
    saveVector.push_back(signed(pixmapSize));
    saveVector.push_back(-1);//negative one will represent new lines for the ascii file
    saveVector.push_back(signed(currentFrame)+1);
    saveVector.push_back(-1);
    for(std::vector<Frame*>::iterator it = framesVector.begin(); it != framesVector.end(); ++it )
    {
            QPixmap *current = (*it)->currentMap;
            QImage currentImage = current->toImage();//convert Qpixmap to QImage to retreiving rgba values

            for(int i = 0; i < pixmapSize; i++)
            {
                for(int j = 0; j < pixmapSize; j++)
                {
                    QRgb pixelColor = currentImage.pixel(static_cast<int>(i*800/pixmapSize+10), static_cast<int>(j*800/pixmapSize+10));
                    saveVector.push_back(qRed(pixelColor));
                    saveVector.push_back(qGreen(pixelColor));
                    saveVector.push_back(qBlue(pixelColor));
                    saveVector.push_back(qAlpha(pixelColor));
                }
                saveVector.push_back(-1);
            }
    }
    emit sendSaveVector(saveVector);//send info to be written in mainwindow
}

///
/// \brief Model::storeNumberOfPixels
/// \param numberOfPixels
/// Computes and stores the number of pixels for the current sprite
///
void Model::storeNumberOfPixels(int numberOfPixels)
{
    numOfPixels = static_cast<int>(pow(numberOfPixels, 2));
}

///
/// \brief Model::openSprite
/// /// creates frames for new loaded sprite
/// Erases all info stored in frames vector and creates frames for the new
/// sprite being loaded in
/// \param frameQueue
/// the frames loaded in from the .ssp file
///
void Model::openSprite(QQueue<int>* frameQueue, int numOfPixels, int numOfFrames)
{
    resetAll();//resets any information stored in the frames vector
    emit resetFrameCountFromOpen();
    emit enableButtonsFromLoad(numOfPixels);
    for(int i = 0; i < numOfFrames; i++)
    {
        Frame* newFrame = new Frame();
        framesVector.push_back(newFrame);
        emit openFrame(frameQueue, numOfPixels);
    }
    emit setDefaultColorOnOpen();
}

///
/// \brief Model::addPixmapFromDuplication
/// Duplicates passed int pixmap and pushes it onto the end of the vector
/// \param newPixmap
/// the new pixmap to be added
///
void Model::addPixmapFromDuplication(QPixmap* newPixmap)
{
    framesVector.back()->addNewPixmap(newPixmap);
}

///
/// \brief Model::duplicateFrame
/// Duplicates the curent selected frame
///
void Model::duplicateFrame()
{
    Frame* newFrame = new Frame();
    // add old pixmap
    QPixmap* newPixmap = framesVector[currentFrame]->duplicate();
    newFrame->addNewPixmap(newPixmap);
    framesVector.push_back(newFrame);
    updateCurrentFrameCounter();
    emit duplicatedFrameAdded(newPixmap);
}

///
/// \brief Model::addPixmapFromLoad
/// \param newPixmap
///
///
void Model::addPixmapFromLoad(QPixmap* newPixmap)
{
    framesVector.back()->addPixmapFromLoad(newPixmap);
}
///
/// \brief Model::retrieveFrameForPlayingAnimation
/// \param frameNumber
/// gets the frame from the vector at the passed in index and sends it to drawing window
///
void Model::retrieveFrameForPlayingAnimation(int frameNumber){
    emit sendFrameToAnimationPlayer(framesVector[static_cast<unsigned int> (frameNumber)]->getPixmap());
}
///
/// \brief Model::deleteRecentFrame
/// Deletest he last frame stored in the frames vector
///
///
void Model::deleteRecentFrame()
{
    if(framesVector.size() > 1)
    {
        std::cout << "Deleted Frame in Frames Vector" << std::endl;
        int lastFrame = static_cast<int> (framesVector.size()) - 1;
        framesVector.erase(framesVector.end() - 1);
        emit displaySelectedFrameFromPreview(framesVector[static_cast<unsigned int> (lastFrame - 1)]->getPixmap(), static_cast<int> (lastFrame - 1));
        emit deletePreviewFrame();
    }
}
///
/// \brief Model::exportGifFromFrames
/// Writes the information from the pixmaps stored in each frame into gif format
/// \param fileName
/// The name of the file being written to
///
void Model::exportGifFromFrames(const char* fileName) {
    GifWriter gifWriter;
    GifBegin(&gifWriter, fileName, 800, 800,
            10, framesVector[0]->getPixmap()->depth(), false);
    //Convert Qpixmap frames to uint8_t* then add into Gif
    for(unsigned long i = 0; i < framesVector.size(); i++) {
        QPixmap* pixMap = framesVector[i]->getPixmap();
        pixMap->scaled(800, 800);
        QImage image = pixMap->toImage().convertToFormat(QImage::Format_RGBA8888);
        GifWriteFrame(&gifWriter, image.bits(), 800, 800, 10, 8, false);
    }
    GifEnd(&gifWriter);
}
