#include "previewframe.h"
///
/// \brief PreviewFrame::PreviewFrame:
/// Constructor for the previewframe, to store the pixmap that is being displayed, and the frame number.
/// \param frameNumber: How to ID what frame is being clicked.
/// \param pixmap: Stores the current display of the pixmap
///
PreviewFrame::PreviewFrame(int frameNumber, QPixmap *pixmap, QWidget* parent) : QLabel(parent)
{
    this->frameNumber = frameNumber;
    this->setStyleSheet("background-color:rgb(255,255,255,1); border: 1px solid black");
    this->setPixmap(pixmap->scaled(150, 150));
}

PreviewFrame::~PreviewFrame()
{

}

///
/// \brief PreviewFrame::mousePressEvent:
/// event handler when a user clicks on the frame to bring it to the UI.
/// \param event:
/// What mouse event it was.
///
void PreviewFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() &Qt::LeftButton)
    {
        std::cout << "PreviewFrame(mousePressEvent) - Selected new Frame from Preview: " << frameNumber << std::endl;
        emit sendFrameNumber(frameNumber);
    }
}

///
/// \brief PreviewFrame::updatePixmap:
/// Updates the current preview with a new pixmap when a user is using that preview on the UI.
/// \param pixmap:
/// New pixmap to be previewed.
///
void PreviewFrame::updatePixmap(QPixmap *pixmap)
{
    this->setPixmap(pixmap->scaled(150, 150));
}


///
/// \brief PreviewFrame::updatePixmap:
/// Updates the current preview with a new pixmap when a user is using that preview on the UI.
/// \param pixmap:
/// New pixmap to be previewed.
///
QPixmap* PreviewFrame::getPixmap()
{
    return pixmap;
}
