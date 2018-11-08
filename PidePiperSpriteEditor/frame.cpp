#include "frame.h"


Frame::Frame()
{

}

///
/// \brief Frame::addNewPixmap
/// \param pixmap
///
void Frame::addNewPixmap(QPixmap pixmap)
{

    std::cout << "ASS" << std::endl;
    QPixmap savePixmap = pixmap.copy();
    pixmapVector.push_back(savePixmap);
    std::cout << "FUCK" << std::endl;

}
