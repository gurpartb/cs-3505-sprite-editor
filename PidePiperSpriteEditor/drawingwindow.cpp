#include "drawingwindow.h"

DrawingWindow::DrawingWindow(QWidget* parent) : QLabel(parent)
{
    pixelSize = 50;
    sizeHasBeenChosen = false;
    pixMap = new QPixmap(800, 800);
    pixMap->fill(Qt::white);
    setPixmap(*pixMap);
}

DrawingWindow::~DrawingWindow()
{
    
}

void DrawingWindow::userChoseSize(int size)
{
    pixMap->fill(Qt::white);
    setPixmap(*pixMap);
    emit createdInitialFrame();

    if (!sizeHasBeenChosen)
    {
        std::cout << "Selected: " << size << std::endl;
        pixelSize = windowSize / size;
        sizeHasBeenChosen = true;
    }
    else
    {
        std::cout << "Selected: " << size << std::endl;
        pixelSize = windowSize / size;
        sizeHasBeenChosen = true;
    }
}

void DrawingWindow::frameAdded()
{
    pixMap->fill(Qt::white);

    setPixmap(*pixMap);
}

void DrawingWindow::mousePressEvent(QMouseEvent* event)
{

    if (event->buttons() &Qt::LeftButton && sizeHasBeenChosen)
    {
        std::cout << "clickin" << std::endl;
        currentlyDrawing = true;
    }

}

void DrawingWindow::mouseMoveEvent(QMouseEvent *event)
{

     if ((event->buttons() & Qt::LeftButton) && currentlyDrawing && sizeHasBeenChosen)
     {
        std::cout << "draggin" << std::endl;
        QPoint pos = event->pos();
        findPixelRatio(pos.x(), pos.y());
        drawPixel();
     }

}

void DrawingWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && currentlyDrawing && sizeHasBeenChosen)
    {
        std::cout << "releasin" << std::endl;
        QPoint pos = event->pos();
        findPixelRatio(pos.x(), pos.y());

        emit updatePixmap(pixMap); //Save the previous Pixmap so we can undo.

        drawPixel();
        currentlyDrawing = false;
    }
}

/**
 * @brief DrawingWindow::findPixelRatio
 * @param currentX
 * @param currentY
 */
void DrawingWindow::findPixelRatio(double currentX, double currentY)
{
    topLeftX = floor(currentX / pixelSize) * pixelSize;
    topLeftY = floor(currentY / pixelSize) * pixelSize;

    bottomRightX = topLeftX + pixelSize;
    bottomRightY = topLeftY + pixelSize;
}

void DrawingWindow::drawPixel()
{
    std::cout << "PAINTIN" << std::endl;

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
    QPen pen(Qt::green, 1);
    painter.setPen(pen);
    path.addRect(pixel);
    painter.fillPath(path, Qt::green);
    painter.drawPath(path);
    this->setPixmap(*pixMap);
}



