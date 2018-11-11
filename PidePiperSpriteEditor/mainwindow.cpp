#include "mainwindow.h"
#include "ui_mainwindow.h"

///
/// \brief MainWindow::MainWindow:
/// Main Window constructor to disable the UI, set connections, and setup the Ui.
/// \param model
/// \param parent
///
MainWindow::MainWindow(Model *model, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scrollArea->setWidgetResizable(true);

    //Disable Ui until user sets size.
    enableUi(false);

    currentSelectedFrame = -1;

    //MainWindow and DrawingWindow connections
    connect(ui->drawingWindowLabel, &DrawingWindow::addFrameToUi, this, &MainWindow::addFrameToUi);
    connect(ui->drawingWindowLabel, &DrawingWindow::updateFramePreview, this, &MainWindow::updateFramePreview);
    connect(this, &MainWindow::resetFrameCount, ui->drawingWindowLabel, &DrawingWindow::resetFrameCount);

    //MainWindow and Model connections
    connect(this, &MainWindow::resetAll, model, &Model::resetAll);

    //DrawingWindow and Model connections
    connect(ui->drawingWindowLabel, &DrawingWindow::updatePixmap, model, &Model::currentFrameUpdatePixmap);
    connect(ui->drawingWindowLabel, &DrawingWindow::createdInitialFrame, model, &Model::createNewFrame);
    connect(ui->drawingWindowLabel, &DrawingWindow::saveCurrentFrame, model, &Model::saveCurrentFrame);

    //UI to Model connections
    connect(ui->addFrameButton, &QPushButton::pressed, model, &Model::updateCurrentFrameCounter);
    connect(ui->addFrameButton, &QPushButton::pressed, model, &Model::createNewFrame);
    connect(ui->undoButton, &QPushButton::pressed, model, &Model::undo);
    connect(model, &Model::undoSignal, ui->drawingWindowLabel, &DrawingWindow::undo);
    connect(model, &Model::frameAdded, ui->drawingWindowLabel, &DrawingWindow::frameAdded);
    connect(ui->drawingWindowLabel, &DrawingWindow::enableUiSignal, this, &MainWindow::enableUi);


    //Frame Preview Class Connections:
    connect(this, &MainWindow::sendFrameNumberToModel, model, &Model::retrieveFrameNumberFromClickedPreview);
    connect(model, &Model::displaySelectedFrameFromPreview, ui->drawingWindowLabel, &DrawingWindow::displaySelectedFrameFromPreview);


    connect(this, &MainWindow::changeColor, ui->drawingWindowLabel, &DrawingWindow::setColor);
    ui->colorSelectButton->setStyleSheet("background-color: rgb(0,0,0,1)");

}

MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::enableUi:
/// Enables Ui from passive state to let user manage buttons.
///
void MainWindow::enableUi(bool enabled)
{
    ui->drawingWindowLabel->setEnabled(enabled);
    ui->drawButton->setEnabled(enabled);
    ui->eraseButton->setEnabled(enabled);
    ui->colorDropButton->setEnabled(enabled);
    ui->mirrorDrawButton->setEnabled(enabled);
    ui->undoButton->setEnabled(enabled);
    ui->duplicateButton->setEnabled(enabled);
    ui->addFrameButton->setEnabled(enabled);
    ui->colorSelectButton->setEnabled(enabled);
}

///
/// \brief MainWindow::on_fileNew_triggered:
/// File manager when user selects new File, and then selects a new sprite image file.
///
void MainWindow::on_fileNew_triggered()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Select size"));
    msgBox.setInformativeText(tr("Make sure to save, you will lose your data!"));
    QAbstractButton* smallSize = msgBox.addButton(tr("8x8"), QMessageBox::NoRole);
    QAbstractButton* mediumSize = msgBox.addButton(tr("16x16"), QMessageBox::NoRole);
    QAbstractButton* largeSize = msgBox.addButton(tr("32x32"), QMessageBox::NoRole);
    msgBox.setStandardButtons(QMessageBox::Cancel);

    int sign = msgBox.exec();

    if (msgBox.clickedButton()==smallSize)
    {
        emit resetAll();
        resetFramePreview();
        ui->drawingWindowLabel->userChoseSize(8);
    }
    else if (msgBox.clickedButton() == mediumSize)
    {
        emit resetAll();
        resetFramePreview();
        ui->drawingWindowLabel->userChoseSize(16);
    }
    else if (msgBox.clickedButton() == largeSize)
    {
        emit resetAll();
        resetFramePreview();
        ui->drawingWindowLabel->userChoseSize(32);
    }

    if(sign != QMessageBox::Cancel) {
        enableUi(true);
    }

}

///
/// \brief MainWindow::addFrameToUi:
/// Adds a new frame to preview on the user interface.
/// \param pixmap: Pixmap to be added to the ui preview.
/// \param frameCount: what frame number to assign to the preview frame object.
///
void MainWindow::addFrameToUi(QPixmap *pixmap, int frameCount)
{

    //Code below is expanding the scrollArea so you can scroll through all of the current frame previews.
    if (frameCount >= 4)
    {
        int sizeOfFramePreview = 135;
        ui->scrollAreaWidgetContents->setMinimumHeight(ui->scrollAreaWidgetContents->height() + sizeOfFramePreview);
    }

    PreviewFrame *framePreview = new PreviewFrame(frameCount, pixmap);
    connect(framePreview, &PreviewFrame::sendFrameNumber, this, &MainWindow::getFrameNumberToSendToModel);
    previewFrameVector.push_back(framePreview);

    //Displaying frame preview to UI at Position 0.
    ui->frameLayout->insertWidget(0, framePreview);
    currentSelectedFrame = frameCount;
    std::cout << "MainWindow(addFrameToUi) - Added a frame preview: " << currentSelectedFrame << std::endl;
}

///
/// \brief MainWindow::updateFramePreview:
/// Function called when the selected frame is edited on the drawing label, to update the preview.
/// \param pixmap: Updated pixmap to send to frame preview.
///
void MainWindow::updateFramePreview(QPixmap *pixmap)
{
    std::cout << "MainWindow(updateFramePreview) - updating frame preview on: " << currentSelectedFrame << std::endl;
    previewFrameVector[static_cast<unsigned int> (currentSelectedFrame)]->updatePixmap(pixmap);
}

///
/// \brief MainWindow::resetFramePreview:
/// Resets the Frame Preview on new file creation.
///
void MainWindow::resetFramePreview()
{

    //Ranged based for to delete the entire previewFrameVector
    for (auto i: previewFrameVector)
    {
        delete i;
    }
    previewFrameVector.clear();
    currentSelectedFrame = -1;
    emit resetFrameCount();

    int originalScrollAreaHeight = 545;
    ui->scrollAreaWidgetContents->setMinimumHeight(originalScrollAreaHeight);

}

///
/// \brief MainWindow::getFrameNumberToSendToModel
/// Helper method to connect the mouse click event with the frame number from PreviewFrame.
/// Takes the frame number from PreviewFrame that was dynamically made, then sent to the
/// \param chosenFrameNumber
///
void MainWindow::getFrameNumberToSendToModel(int chosenFrameNumber)
{
    std::cout << "MainWindow(getFrameNumberToSendToModel) - Retrieved Frame Number to send to Model: " << chosenFrameNumber << std::endl;
    currentSelectedFrame = chosenFrameNumber;
    emit sendFrameNumberToModel(chosenFrameNumber);
}

void MainWindow::on_colorSelectButton_clicked()
{
    QColorDialog colorWindow;
    colorWindow.exec();
    ui->colorSelectButton->setStyleSheet("background-color: " + colorWindow.selectedColor().name());
    emit changeColor(colorWindow.selectedColor());
}

