#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QTextStream>

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
    ui->drawingWindowLabel->setDisabled(true);
    ui->drawButton->setDisabled(true);
    ui->eraseButton->setDisabled(true);
    ui->colorDropButton->setDisabled(true);
    ui->mirrorDrawButton->setDisabled(true);
    ui->undoButton->setDisabled(true);
    ui->duplicateButton->setDisabled(true);
    ui->addFrameButton->setDisabled(true);
    ui->colorSelectButton->setDisabled(true);

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

    //UI to Model connections
    connect(ui->addFrameButton, &QPushButton::pressed, model, &Model::updateCurrentFrameCounter);
    connect(ui->addFrameButton, &QPushButton::pressed, model, &Model::createNewFrame);
    connect(ui->undoButton, &QPushButton::pressed, model, &Model::undo);
    connect(model, &Model::undoSignal, ui->drawingWindowLabel, &DrawingWindow::undo);
    connect(model, &Model::frameAdded, ui->drawingWindowLabel, &DrawingWindow::frameAdded);

    //Frame Preview Class Connections:
    connect(this, &MainWindow::sendFrameNumberToModel, model, &Model::retrieveFrameNumberFromClickedPreview);
    connect(model, &Model::displaySelectedFrameFromPreview, ui->drawingWindowLabel, &DrawingWindow::displaySelectedFrameFromPreview);

    //Save Connection
     connect(this, &MainWindow::save, model, &Model::saveAs);
     connect(model, &Model::sendSaveVector, this, &MainWindow::saveAs);
     connect(this, &MainWindow::sizeChosen, model, &Model::storeNumberOfPixels);
     ui->fileSaveAs->setDisabled(true);

     //Open Connections
     connect(this, &MainWindow::openSprite, model, &Model::openSprite);
     connect(model, &Model::resetFrameCountFromOpen, ui->drawingWindowLabel, &DrawingWindow::resetFrameCountFromOpen);
     connect(model, &Model::openFrame, ui->drawingWindowLabel, &DrawingWindow::openingFrame);
     connect(ui->drawingWindowLabel, &DrawingWindow::addDuplicatedPixmap, model, &Model::addPixmapFromDuplication);

     //Duplicate Connections
     connect(ui->duplicateButton, &QPushButton::pressed, model, &Model::duplicateFrame);
     connect(model, &Model::duplicatedFrameAdded, ui->drawingWindowLabel, &DrawingWindow::duplicatedFrame);

}

MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::enableUi:
/// Enables Ui from passive state to let user manage buttons.
///
void MainWindow::enableUi()
{
    ui->drawingWindowLabel->setEnabled(true);
    ui->drawButton->setEnabled(true);
    ui->eraseButton->setEnabled(true);
    ui->colorDropButton->setEnabled(true);
    ui->mirrorDrawButton->setEnabled(true);
    ui->undoButton->setEnabled(true);
    ui->duplicateButton->setEnabled(true);
    ui->addFrameButton->setEnabled(true);
    ui->colorSelectButton->setEnabled(true);
    ui->fileSaveAs->setEnabled(true);
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

    msgBox.exec();
    if (msgBox.clickedButton()==smallSize)
    {
        emit resetAll();
        emit sizeChosen(8);
        resetFramePreview();
        ui->drawingWindowLabel->userChoseSize(8);
        enableUi();
    }
    else if (msgBox.clickedButton() == mediumSize)
    {
        emit resetAll();
        emit sizeChosen(16);
        resetFramePreview();
        ui->drawingWindowLabel->userChoseSize(16);
        enableUi();
    }
    else if (msgBox.clickedButton() == largeSize)
    {
        emit resetAll();
        emit sizeChosen(32);
        resetFramePreview();
        ui->drawingWindowLabel->userChoseSize(32);
        enableUi();
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
    currentSelectedFrame = chosenFrameNumber;
    emit sendFrameNumberToModel(chosenFrameNumber);
}

void MainWindow::on_colorSelectButton_clicked()
{

}


///
/// \brief MainWindow::on_fileSaveAs_triggered
///
void MainWindow::on_fileSaveAs_triggered()
{
    emit save();
}

///
/// \brief MainWindow::saveAs
/// \param saveVector
///
void MainWindow::saveAs(std::vector<int> saveVector)
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Sprite"), "",
        tr("Sprite (*.ssp);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        QTextStream out(&file);
        //out.setVersion(QDataStream::Qt_4_5);
        for(std::vector<int>::iterator it = saveVector.begin(); it != saveVector.end(); ++it )
        {
            if(*it == 10)
            {
                out<<endl;
                continue;
            }
            out << *it;
            out << " ";
        }
    }
}

///
/// \brief MainWindow::on_fileLoadSprite_triggered
///
void MainWindow::on_fileLoadSprite_triggered()
{
    QQueue<int>* openQueue = new QQueue<int>;
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Sprite"), "",
        tr("Sprite (*.ssp);;All Files (*)"));

    if (fileName.isEmpty())
           return;
       else {

           QFile file(fileName);

           if (!file.open(QIODevice::ReadOnly)) {
               QMessageBox::information(this, tr("Unable to open file"),
                   file.errorString());
               return;
           }

           QTextStream in(&file);
           QString content = in.readAll();
           QStringList data = content.split(" ");
           for(const auto& i : data)
           {
               openQueue->enqueue(i.toInt());
           }
       }
    emit openSprite(openQueue);
}
