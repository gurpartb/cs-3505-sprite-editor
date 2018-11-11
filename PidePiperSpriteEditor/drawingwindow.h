//Idea taken: https://wiki.qt.io/Clickable_QLabel

#ifndef DRAWINGWINDOW_H
#define DRAWINGWINDOW_H

#include <QLabel>
#include <QWidget>
#include <Qt>
#include <iostream>
#include <QMouseEvent>
#include <string>
#include <math.h>
#include <QRectF>
#include <QPainter>

class DrawingWindow : public QLabel {
    Q_OBJECT

public:
    explicit DrawingWindow(QWidget* parent = Q_NULLPTR);
    ~DrawingWindow();
    void userChoseSize(int size);

signals:
    void clicked();
    void updatePixmap(QPixmap*);
    void createdInitialFrame();
    void addFrameToUi(QPixmap*, int);
    void updateFramePreview(QPixmap*);

public slots:
    void frameAdded();
    void undo(QPixmap*);
    void displaySelectedFrameFromPreview(QPixmap*, int);
    void resetFrameCount();

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    //void findPixelRatio(double currentX, double currentY);
    void drawPixel(QPoint pos);
    QRectF getCurrentPixel(QPoint pos);
    QRectF getMirrorPixel(QPoint pos);
    QPointF getTopLeftPoint(QPoint pos);
    QPixmap *pixMap;

    bool currentlyDrawing;
    bool mirrorDrawing;
    int pixelSize; //This is a field for the size of the window.
    int frameCount;
    int currentFrameSelected;
    const int windowSize = 800;
    bool sizeHasBeenChosen; //This is a test to check if the user has inputed size of the window before drawing.
    QColor color;
};

#endif // DRAWINGWINDOW_H
