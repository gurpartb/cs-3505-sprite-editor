#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connect(this, &MainWindow::userChoseSize, window, &DrawingWindow::userChoseSize);
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
    }
    else if (msgBox.clickedButton() == mediumSize)
    {
        ui->drawingWindowLabel->userChoseSize(16);
    }
    else if (msgBox.clickedButton() == largeSize)
    {
        ui->drawingWindowLabel->userChoseSize(32);
    }
}

void MainWindow::on_colorSelectButton_clicked()
{

}
