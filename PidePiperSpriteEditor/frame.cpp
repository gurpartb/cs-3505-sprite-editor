#include "frame.h"


Frame::Frame() : pixmapVector()
{
 currentMap  = new QPixmap();
}

///
/// \brief Frame::addNewPixmap
/// \param pixmap
///
void Frame::addNewPixmap(QPixmap *pixmap)
{

    if (!pixmap)
    {
        std::cout << "No Pixmap" << std::endl;
    }
    else
    {
        QPixmap *savePixmap = new QPixmap(*pixmap);
        pixmapVector.push_back(savePixmap);
    }

}

void Frame::addPixmapFromLoad(QPixmap *pixmap)
{
    //pixmapVector.push_back(pixmap);
    *currentMap = pixmap->copy();
}

///
/// \brief Frame::getPixmap:
/// Gets the most recent pixmap from the current frame.
/// \return QPixmap:
/// Most recent pixmap.
///
QPixmap* Frame::getPixmap()
{
    std::cout << "Frame(getPixmap) - returning pixmap: " << currentMap << std::endl;
    return currentMap;
}
///
/// \brief Frame::undo
/// \return QPixmap
/// returns pops off last pixmap in frame
/// sets current map to the last map
///
QPixmap* Frame::undo()
{
    if(pixmapVector.size() == 0)
    {
        return nullptr;
    }
    QPixmap *lastMap = pixmapVector.back();
    pixmapVector.pop_back();
    *currentMap  = lastMap->copy();
    return lastMap;
}

///
/// \brief Frame::duplicate
/// \return
///
QPixmap* Frame::duplicate()
{
    QPixmap* newPixmap = new QPixmap(*currentMap);
    return newPixmap;
}
