#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Model *model, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->drawingWindowLabel, &DrawingWindow::updatePixmap, model, &Model::currentFrameUpdatePixmap);
    connect(ui->drawingWindowLabel, &DrawingWindow::createdInitialFrame, model, &Model::createNewFrame);
    connect(ui->addFrameButton, &QPushButton::pressed, model, &Model::updateCurrentFrameCounter);
    connect(ui->addFrameButton, &QPushButton::pressed, model, &Model::createNewFrame);
    connect(ui->undoButton, &QPushButton::pressed, model, &Model::undo);
    connect(model, &Model::undoSignal, ui->drawingWindowLabel, &DrawingWindow::undo);
    connect(this, &MainWindow::resetAll, model, &Model::resetAll);
    connect(model, &Model::frameAdded, ui->drawingWindowLabel, &DrawingWindow::frameAdded);
    connect(ui->drawingWindowLabel, &DrawingWindow::addFrameToUi, this, &MainWindow::addFrameToUi);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fileNew_triggered()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Select size"));
    msgBox.setInformativeText(tr("Make sure to save, you will lose your data!"));
    QAbstractButton* smallSize = msgBox.addButton(tr("8x8"), QMessageBox::NoRole);
    QAbstractButton* mediumSize = msgBox.addButton(tr("16x16"), QMessageBox::NoRole);
    QAbstractButton* largeSize = msgBox.addButton(tr("32x32"), QMessageBox::NoRole);
    msgBox.setStandardButtons(QMessageBox::Cancel);

    msgBox.exec();
    if (msgBox.clickedButton()==smallSize)
    {
        emit resetAll();
        ui->drawingWindowLabel->userChoseSize(8);
    }
    else if (msgBox.clickedButton() == mediumSize)
    {
        emit resetAll();
        ui->drawingWindowLabel->userChoseSize(16);
    }
    else if (msgBox.clickedButton() == largeSize)
    {
        emit resetAll();
        ui->drawingWindowLabel->userChoseSize(32);
    }

}

void MainWindow::addFrameToUi(QPixmap pixmap, int frameCount)
{
    //Code below is expanding the scrollArea so you can scroll through all of the current frame previews.
    if (frameCount > 4)
    {
        int sizeOfFramePreview = 150;
        int currentScrollHeight = ui->scrollAreaWidgetContents->height();
        ui->scrollAreaWidgetContents->setMinimumHeight(currentScrollHeight + sizeOfFramePreview);
    }


}

void MainWindow::on_colorSelectButton_clicked()
{

}

