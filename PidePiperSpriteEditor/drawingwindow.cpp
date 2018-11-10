#include "drawingwindow.h"

///
/// \brief DrawingWindow::DrawingWindow:
/// Drawing window constructor to initalize fields, set the size of the window, and temporarly fill it.
///
DrawingWindow::DrawingWindow(QWidget* parent) : QLabel(parent)
{
    pixelSize = 50;
    frameCount = -1; //Starting at -1 since the first initial frame is the 0th frame.
    sizeHasBeenChosen = false;
    pixMap = new QPixmap(800, 800);
    pixMap->fill(Qt::white);
    setPixmap(*pixMap);
}

DrawingWindow::~DrawingWindow()
{
    
}

///
/// \brief DrawingWindow::userChoseSize:
/// Method to create the first frame after a new file has been chosen and calculate the size of each pixel.
/// \param size:
/// Size chosen from user.
///
void DrawingWindow::userChoseSize(int size)
{
    pixMap->fill(Qt::white);
    setPixmap(*pixMap);
    emit createdInitialFrame();

    std::cout << "Chose size: " << size << std::endl;
    pixelSize = windowSize / size;
    sizeHasBeenChosen = true;

}

///
/// \brief DrawingWindow::frameAdded:
/// Helper that updates counters, and updates the current label pixmap to the new frame.
///
void DrawingWindow::frameAdded()
{
    pixMap->fill(Qt::white);
    frameCount++;
    setPixmap(*pixMap);
    currentFrameSelected = frameCount;
    emit addFrameToUi(pixMap, frameCount);
    emit updatePixmap(pixMap);
}

///
/// \brief DrawingWindow::mousePressEvent:
/// Mouse press event for the first press down.
/// \param event:
/// Press down event.
///
void DrawingWindow::mousePressEvent(QMouseEvent* event)
{

    if (event->buttons() &Qt::LeftButton && sizeHasBeenChosen)
    {
        currentlyDrawing = true;
    }

}

///
/// \brief DrawingWindow::mouseMoveEvent:
/// Mouse move event when mouse is pressed down and moving to draw.
/// \param event:
/// Left button down and moving.
///
void DrawingWindow::mouseMoveEvent(QMouseEvent *event)
{

     if ((event->buttons() &Qt::LeftButton) && currentlyDrawing && sizeHasBeenChosen)
     {
        QPoint pos = event->pos();
        findPixelRatio(pos.x(), pos.y());
        drawPixel();
     }

}

///
/// \brief DrawingWindow::mouseReleaseEvent:
/// Mouse Release for left mouse button release to set Ui and end drawing.
/// \param event:
/// Left mouse release event.
///
void DrawingWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() &Qt::LeftButton) && currentlyDrawing && sizeHasBeenChosen)
    {
        QPoint pos = event->pos();
        findPixelRatio(pos.x(), pos.y());

        emit updatePixmap(pixMap); //Save the previous Pixmap so we can undo.
        drawPixel();
        emit updateFramePreview(pixMap);
        currentlyDrawing = false;
    }
}

///
/// \brief DrawingWindow::findPixelRatio:
/// Helper method to find the size of the pixel.
/// \param currentX
/// \param currentY
///
void DrawingWindow::findPixelRatio(double currentX, double currentY)
{
    topLeftX = floor(currentX / pixelSize) * pixelSize;
    topLeftY = floor(currentY / pixelSize) * pixelSize;

    bottomRightX = topLeftX + pixelSize;
    bottomRightY = topLeftY + pixelSize;
}

///
/// \brief DrawingWindow::drawPixel:
/// Method to draw pixels onto the screen with a selected color.
///
void DrawingWindow::drawPixel()
{

    QPointF topLeft;
    QPointF bottomRight;

    topLeft.setX(topLeftX);
    topLeft.setY(topLeftY);

    bottomRight.setX(bottomRightX);
    bottomRight.setY(bottomRightY);

    QRectF pixel(topLeft, bottomRight);

    QPainter painter(pixMap);
    QPainterPath path;
    QPen pen(Qt::green, 1);
    painter.setPen(pen);
    path.addRect(pixel);
    painter.fillPath(path, Qt::green);
    painter.drawPath(path);
    this->setPixmap(*pixMap);
}

///
/// \brief DrawingWindow::drawPixelFromLoad
/// \param color
///
void DrawingWindow::drawPixelFromLoad(QColor color)
{
    //std::cout << "PAINTIN" << std::endl;

    QPointF topLeft;
    QPointF bottomRight;

    topLeft.setX(topLeftX);
    topLeft.setY(topLeftY);

    bottomRight.setX(bottomRightX);
    bottomRight.setY(bottomRightY);

    //QRectF(const QPointF &topLeft, const QPointF &bottomRight)
    QRectF pixel(topLeft, bottomRight);

    QPainter painter(pixMap);
    QPainterPath path;
    QPen pen(color, 1);
    painter.setPen(pen);
    path.addRect(pixel);
    painter.fillPath(path, color);
    painter.drawPath(path);
    this->setPixmap(*pixMap);
}

void DrawingWindow::undo(QPixmap* map)
{
    if(map == nullptr){
        std::cout <<"Nothing left to undo" << '\n';
        return;
    }
    pixMap = new QPixmap(*map);
    this->setPixmap(*pixMap);
    emit updateFramePreview(pixMap);
}

///
/// \brief DrawingWindow::displaySelectedFrameFromPreview:
/// Takes the selected frame from the scrollbar and displays it on the current drawing window.
/// \param framePreviewPixmap - Pixmap from frame preview
///
void DrawingWindow::displaySelectedFrameFromPreview(QPixmap *framePreviewPixmap, int frameSelected)
{
    currentFrameSelected = frameSelected;
    std::cout << "DrawingWindow(displaySelected) - Updating main Pixmap from frame preview: " << currentFrameSelected << std::endl;
    this->pixMap = framePreviewPixmap;
    this->setPixmap(*framePreviewPixmap);
}

///
/// \brief DrawingWindow::resetFrameCount:
/// Resets frame count on new file creation.
///
void DrawingWindow::resetFrameCount()
{
    frameCount = -1;
}

///
/// \brief DrawingWindow::resetFrameCountFromOpen
///
void DrawingWindow::resetFrameCountFromOpen()
{
    frameCount = 0;
}

///
/// \brief DrawingWindow::openingFrame
/// \param frameQueue
/// \param pixmapSize
///
void DrawingWindow::openingFrame(QQueue<int>* frameQueue, int pixmapSize)
{
    sizeHasBeenChosen = true;
    pixMap->fill(Qt::white);
    pixelSize = windowSize/pixmapSize;
    // Fill the pixmap
    // Find the pixel ratio
    for(int i = 0; i < pixmapSize; i++)
    {
        for(int j = 0; j < pixmapSize; j++)
        {
            // change the pixel color
            int red = frameQueue->dequeue();
            int green = frameQueue->dequeue();
            int blue = frameQueue->dequeue();
            int alpha = frameQueue->dequeue();
            QColor color(red, green, blue, alpha);
            findPixelRatio(j*windowSize/pixmapSize, i*windowSize/pixmapSize);
            drawPixelFromLoad(color);
        }
    }
    frameCount++;
    setPixmap(*pixMap);
    QPixmap* newPixmap = new QPixmap(*pixMap);
    emit addDuplicatedPixmap(newPixmap);
    //emit addFrameToUi(*pixMap, frameCount);
}

///
/// \brief DrawingWindow::duplicatedFrame
/// \param newPixmap
///
void DrawingWindow::duplicatedFrame(QPixmap* newPixmap)
{
    //*pixMap = newPixmap->copy();
    frameCount++;
    setPixmap(*newPixmap);
    //std::cout << "Current Frame: " << frameCount << std::endl;
}
