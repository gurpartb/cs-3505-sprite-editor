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

   // std::cout << "Checking pixmap in framesvector" << std::endl;
    if (!pixmap)
    {
        std::cout << "No Pixmap" << std::endl;
    }
    else
    {
   //     std::cout << "Adding to pixmap:" << std::endl;
        QPixmap *savePixmap = new QPixmap(*pixmap);
     //   std::cout << "COPIED PIXMAP" << std::endl;
        pixmapVector.push_back(savePixmap);
        //std::cout << "added to pixmap vector" << std::endl;
    }

}

QPixmap* Frame::undo(){
    if(pixmapVector.size() == 0){
        return nullptr;
    }

   QPixmap *lastMap = pixmapVector.back();
   pixmapVector.pop_back();
   return lastMap;
}
