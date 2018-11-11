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


public:
    int frameNumber;

    explicit PreviewFrame(int frameNumber, QPixmap *, QWidget* parent = Q_NULLPTR);
    ~PreviewFrame();
    void updatePixmap(QPixmap *);
    QPixmap* getPixmap();
    QPixmap *pixmap;

signals:
    void sendFrameNumber(int);

private:
    void mousePressEvent(QMouseEvent *event);

};

#endif // PREVIEWFRAME_H
