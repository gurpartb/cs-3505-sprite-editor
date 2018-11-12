#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "drawingwindow.h"
#include "previewframe.h"
#include "model.h"
#include <QMainWindow>
#include <QPixmap>
#include <QObject>
#include <QTimer>
#include <QMessageBox>
#include <QColorDialog>
#include <QQueue>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Model *model, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void resetAll();
    void sendFrameNumberToModel(int frameNumber);
    void resetFrameCount();
    void changeColor(QColor color);
    void mirrorDrawButtonClicked();
    void save();
    void sizeChosen(int);
    void openSprite(QQueue<int>*);
    void sliderChanged(int);
    void updateFPSDisplay(int);
    void retrieveAnimationFrameSignal(int);

private slots:
    void addFrameToUi(QPixmap*, int);
    void updateFramePreview(QPixmap*);
    void getFrameNumberToSendToModel(int);
    void on_fileNew_triggered();
    void on_colorSelectButton_clicked();
    void on_mirrorDrawButton_clicked();
    void saveAs(std::vector<int>);
    void on_fileSaveAs_triggered();
    void on_fileLoadSprite_triggered();
    //void on_rectangleButton_clicked();

    void on_animationSlider_valueChanged(int value);
    void on_fpsSlider_valueChanged(int value);
    void playAnimation(QPixmap *);
    void getAnimationFrame();
    void setColorButton(QRgb buttonColor);

private:
    Ui::MainWindow *ui;
    std::vector<PreviewFrame*> previewFrameVector;
    QTimer *fpsTimer;

    int currentSelectedFrame;
    int currentPlayedFrame;

    void resetFramePreview();
    void enableUi(bool);
};

#endif // MAINWINDOW_H
