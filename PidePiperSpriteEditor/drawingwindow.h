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

public slots:
    void frameAdded();
    void undo(QPixmap*);

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void findPixelRatio(double currentX, double currentY);
    void drawPixel();
    QPixmap *pixMap;

    bool currentlyDrawing;
    int pixelSize; //This is a field for the size of the window.
    const int windowSize = 800;
    bool sizeHasBeenChosen; //This is a test to check if the user has inputed size of the window before drawing.
    double currentPixelX;
    double currentPixelY;
    double topLeftX, topLeftY, bottomRightX, bottomRightY;
    QColor color;
};

#endif // DRAWINGWINDOW_H
