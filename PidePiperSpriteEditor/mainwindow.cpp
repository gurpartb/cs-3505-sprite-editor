#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Model *model, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->drawingWindowLabel, &DrawingWindow::updatePixmap, model, &Model::currentFrameUpdatePixmap);
    connect(ui->drawingWindowLabel, &DrawingWindow::createdInitialFrame, model, &Model::createNewFrame);
    connect(ui->addFrameButton, &QPushButton::pressed, model, &Model::updateCurrentFrameCounter);
    connect(ui->addFrameButton, &QPushButton::pressed, model, &Model::createNewFrame);
    connect(this, &MainWindow::resetAll, model, &Model::resetAll);
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
        ui->drawingWindowLabel->userChoseSize(8);
        emit resetAll();
    }
    else if (msgBox.clickedButton() == mediumSize)
    {
        ui->drawingWindowLabel->userChoseSize(16);
        emit resetAll();
    }
    else if (msgBox.clickedButton() == largeSize)
    {
        ui->drawingWindowLabel->userChoseSize(32);
        emit resetAll();
    }
}

void MainWindow::on_colorSelectButton_clicked()
{

}


///
/// \brief MainWindow::on_addFrameButton_clicked
///
void MainWindow::on_addFrameButton_clicked()
{

}

