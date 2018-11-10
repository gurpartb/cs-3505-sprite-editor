#include "frame.h"


Frame::Frame() : pixmapVector()
{

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

///
/// \brief Frame::getPixmap:
/// Gets the most recent pixmap from the current frame.
/// \return QPixmap:
/// Most recent pixmap.
///
QPixmap* Frame::getPixmap()
{
    std::cout << "Frame(getPixmap) - returning pixmap" << std::endl;
    return pixmapVector.back();
}

QPixmap* Frame::undo()
{
    if(pixmapVector.size() == 0)
    {
        return nullptr;
    }
    QPixmap *lastMap = pixmapVector.back();
    pixmapVector.pop_back();
    return lastMap;
}
