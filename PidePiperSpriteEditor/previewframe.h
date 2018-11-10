#ifndef PREVIEWFRAME_H
#define PREVIEWFRAME_H
#include <QObject>
#include <QLabel>
#include <QMouseEvent>
#include <Qt>
#include <iostream>

class PreviewFrame : public QLabel
{
    Q_OBJECT

    int frameNumber;

public:
    explicit PreviewFrame(int frameNumber, QPixmap *, QWidget* parent = Q_NULLPTR);
    ~PreviewFrame();
    void updatePixmap(QPixmap *);

signals:
    void sendFrameNumber(int);

private:
    void mousePressEvent(QMouseEvent *event);
    QPixmap *pixmap;
};

#endif // PREVIEWFRAME_H
