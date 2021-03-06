#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QInputDialog>
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

    setWindowIcon(QIcon(":/pplogo.jpg"));

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
    connect(model, &Model::setDefaultColorOnOpen, ui->drawingWindowLabel, &DrawingWindow::setDefaultColorOnOpen);

    //UI to Model connections
    connect(ui->addFrameButton, &QPushButton::pressed, model, &Model::updateCurrentFrameCounter);
    connect(ui->addFrameButton, &QPushButton::pressed, model, &Model::createNewFrame);
    connect(ui->undoButton, &QPushButton::pressed, model, &Model::undo);
    connect(this, SIGNAL(sliderChanged(int)), model, SLOT(setCurrentFrameFromSlider(int)));
    connect(model, &Model::undoSignal, ui->drawingWindowLabel, &DrawingWindow::undo);
    connect(model, &Model::frameAdded, ui->drawingWindowLabel, &DrawingWindow::frameAdded);
    connect(ui->drawingWindowLabel, &DrawingWindow::enableUiSignal, this, &MainWindow::enableUi);

    //Frame Preview Class Connections:
    connect(this, &MainWindow::sendFrameNumberToModel, model, &Model::retrieveFrameNumberFromClickedPreview);
    connect(model, &Model::displaySelectedFrameFromPreview, ui->drawingWindowLabel, &DrawingWindow::displaySelectedFrameFromPreview);

    connect(this, &MainWindow::changeColor, ui->drawingWindowLabel, &DrawingWindow::setColor);
    ui->colorSelectButton->setStyleSheet("background-color: rgb(0,0,0,1)");

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
    connect(model, &Model::enableButtonsFromLoad, this, &MainWindow::enableUi);
    connect(model, &Model::enableButtonsFromLoad, ui->drawingWindowLabel, &DrawingWindow::initializeLabelFromLoad);
    connect(ui->drawingWindowLabel, &DrawingWindow::addPixmapToFrameFromLoad, model, &Model::addPixmapFromLoad);

    //Duplicate Connections
    connect(ui->duplicateButton, &QPushButton::pressed, model, &Model::duplicateFrame);
    connect(model, &Model::duplicatedFrameAdded, ui->drawingWindowLabel, &DrawingWindow::duplicatedFrame);
    connect(ui->drawingWindowLabel, &DrawingWindow::addFrameToPreviewOfFrames, this, &MainWindow::addFrameToUi);

    //Draw connection
    connect(ui->drawButton, &QPushButton::pressed, ui->drawingWindowLabel, &DrawingWindow::setIsDraw);

    //Mirror Pixel Connections
    connect(ui->mirrorDrawButton, &QPushButton::pressed, ui->drawingWindowLabel, &DrawingWindow::setIsMirrorDrawing);

    // Rectangle connection
    connect(ui->rectangleDrawButton, &QPushButton::pressed, ui->drawingWindowLabel, &DrawingWindow::setIsRectangleDrawing);

     // Dropper connection
     connect(ui->colorDropButton, &QPushButton::pressed, ui->drawingWindowLabel, &DrawingWindow::setIsColorDropper);
     connect(ui->drawingWindowLabel, &DrawingWindow::setColorButtonUI, this, &MainWindow::setColorButton);

     // Eraser connection
     connect(ui->eraseButton, &QPushButton::pressed, ui->drawingWindowLabel, &DrawingWindow::setIsEraser);

    fpsTimer = new QTimer(this);
    connect(fpsTimer, SIGNAL(timeout()), this, SLOT(getAnimationFrame()));
    connect(model, SIGNAL(sendFrameToAnimationPlayer(QPixmap*)), this, SLOT(playAnimation(QPixmap*)));
    connect(this, SIGNAL(retrieveAnimationFrameSignal(int)), model, SLOT(retrieveFrameForPlayingAnimation(int)));

    //Delete Frame Button
    connect(ui->deleteFrameButton, &QPushButton::pressed, model, &Model::deleteRecentFrame);
    connect(model, SIGNAL(deletePreviewFrame()), this, SLOT(deleteLastPreviewFrame()));
    connect(this, SIGNAL(deleteDrawingWindowFrames()), ui->drawingWindowLabel, SLOT(removeFrame()));

    //Export Gif connection
    connect(this, SIGNAL(exportGifSignal(const char*)), model, SLOT(exportGifFromFrames(const char*)));

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
    ui->fileSaveAs->setEnabled(enabled);
    ui->rectangleDrawButton->setEnabled(enabled);
    ui->deleteFrameButton->setEnabled(enabled);
    ui->rectangleDrawButton->setEnabled(enabled);
    ui->deleteFrameButton->setEnabled(enabled);
    if (enabled == true)
    {
        ui->drawingWindowLabel->setStyleSheet("background-color:white; border: 1px solid black;");
    }

    //activates the animation on start
    if(enabled)
    {
        ui->fpsSlider->setValue(1);
    }
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

    ui->animationSlider->setValue(0);
    ui->animationSlider->setMaximum(0);
    if (msgBox.clickedButton()==smallSize)
    {
        emit resetAll();
        emit sizeChosen(8);
        resetFramePreview();
        ui->drawingWindowLabel->userChoseSize(8);
    }
    else if (msgBox.clickedButton() == mediumSize)
    {
        emit resetAll();
        emit sizeChosen(16);
        resetFramePreview();
        ui->drawingWindowLabel->userChoseSize(16);
    }
    else if (msgBox.clickedButton() == largeSize)
    {
        emit resetAll();
        emit sizeChosen(32);
        resetFramePreview();
        ui->drawingWindowLabel->userChoseSize(32);
    }

    if(sign != QMessageBox::Cancel) {
        enableUi(true);
    }
}

///
/// \brief MainWindow::addFrameToUi:
/// Adds a new frame to preview on the user interface and increases the animation slider.
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
    framePreview->setMaximumHeight(152);
    framePreview->setMaximumWidth(152);
    connect(framePreview, &PreviewFrame::sendFrameNumber, this, &MainWindow::getFrameNumberToSendToModel);
    previewFrameVector.push_back(framePreview);

    //Displaying frame preview to UI at Position 0.
    ui->frameLayout->insertWidget(0, framePreview);
    currentSelectedFrame = frameCount;
    std::cout << "MainWindow(addFrameToUi) - Added a frame preview: " << currentSelectedFrame << std::endl;

    ui->animationSlider->setMaximum(static_cast<int> (previewFrameVector.size() - 1));
    ui->animationSlider->blockSignals(true);
    ui->animationSlider->setValue(static_cast<int> (previewFrameVector.size() - 1));
    ui->animationSlider->blockSignals(false);
    ui->scrollArea->repaint(); //Update scroll area (this fixed a bug on a load file)
}
///
/// \brief MainWindow::deleteRecentPreviewFrame:
/// Delets the most recently created frame
///
///
void MainWindow::deleteLastPreviewFrame()
{

    previewFrameVector.erase(previewFrameVector.end() - 1);
    QLayoutItem *child;
    child = ui->frameLayout->takeAt(0);
    delete child -> widget();
    std::cout << child << std::endl;
    delete child;
    int sliderValue = ui->animationSlider->value();
    if(sliderValue >= static_cast<int> (previewFrameVector.size()))
    {
        ui->animationSlider->setValue(static_cast<int> (previewFrameVector.size() - 1));
    }
    ui->animationSlider->setMaximum(static_cast<int> (previewFrameVector.size() - 1));
    emit deleteDrawingWindowFrames();

}
///
/// \brief MainWindow::
///
/// :
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


void MainWindow::getAnimationFrame()
{
    std::cout << "Getting Animation Frame: " << currentPlayedFrame << std::endl;
    if(static_cast<unsigned int> (currentPlayedFrame) > previewFrameVector.size() - 1)
    {
        currentPlayedFrame = 0;
    }
    emit retrieveAnimationFrameSignal(currentPlayedFrame);
    currentPlayedFrame++;
}


void MainWindow::playAnimation(QPixmap * pixmap)
{

    const QPixmap pixmapInstance = *pixmap;

    ui->fpsPreviewLabel->setPixmap(pixmapInstance.scaled(150, 150));

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
    colorWindow.setOption(QColorDialog::ShowAlphaChannel);
    colorWindow.exec();
    ui->colorSelectButton->setStyleSheet("background-color: " + colorWindow.selectedColor().name());
    emit changeColor(colorWindow.selectedColor());
}

// We need signal to boolean in drawing window perhaps
// Dropper is going to read color from the currentImage at clicked point
// Udpate the, color in the drawing
// update the, color in the ui->color button
void MainWindow::setColorButton(QRgb buttonColor){
    // emit changeColor(buttonColor);
    QColor colorButton(buttonColor);
    ui->colorSelectButton->setStyleSheet("background-color: " + colorButton.name());
}

//void MainWindow::on_mirrorDrawButton_clicked()
//{
//    emit mirrorDrawButtonClicked();

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
   // QString fileName = QFileDialog::getSaveFileName(this, tr("Save Sprite"), "", tr("Sprite (*.ssp);;All Files (*)"));
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Sprite"), "", tr("Sprite (*.ssp)"));
    if (!fileName.endsWith(".ssp"))
    {
        fileName += ".ssp";
    }

    if (fileName.isEmpty())
        return;
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }
        QTextStream out(&file);
        for(std::vector<int>::iterator it = saveVector.begin(); it != saveVector.end(); ++it )
        {
            if(*it == -1)
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
    ui->fpsSlider->setValue(0);
    resetFramePreview();
    QQueue<int>* openQueue = new QQueue<int>;
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Sprite"), "",
        tr("Sprite (*.ssp);;All Files (*)"));
    int numOfPixels, numOfFrames;

    if (fileName.isEmpty())
    {
           return;
    }
    else
    {

           QFile file(fileName);

           if (!file.open(QIODevice::ReadOnly))
           {
               QMessageBox::information(this, tr("Unable to open file"),
                   file.errorString());
               return;
           }

           QTextStream in(&file);
           QString line1 = in.readLine();
           QStringList sizes = line1.split(" ");
           numOfPixels = sizes[0].toInt();

           QString line2 = in.readLine();
           QStringList frames = line2.split(" ");
           numOfFrames = frames[0].toInt();
           QString content = in.readAll();
           QStringList data = content.split(" ");
           for(const auto& i : data)
           {
               openQueue->enqueue(i.toInt());
           }
       }
    emit openSprite(openQueue,numOfPixels,numOfFrames);
}

void MainWindow::on_mirrorDrawButton_clicked()
{

}

//void MainWindow::on_rectangleButton_clicked()
//{

//}
void MainWindow::on_animationSlider_valueChanged(int value)
{
    emit sliderChanged(value);
}

void MainWindow::on_fpsSlider_valueChanged(int value)
{
    ui->fpsCountLabel->setText(QString::fromStdString("Frames Per Second: " + std::to_string(value)));
    if(value > 0)
    {
        currentPlayedFrame = 0;
        if(fpsTimer->isActive())
        {
            fpsTimer -> setInterval(int(1000 / value));
        }
        else
        {
            fpsTimer -> start(int(1000 / value));
        }
    }
    else
    {
        fpsTimer -> stop();
    }
}

void MainWindow::on_fileExportGif_triggered()
{
     QInputDialog input;
     input.setLabelText("Enter what you would like to name the gif file:");
     int sign = input.exec();
     QByteArray byteArray = input.textValue().toLocal8Bit();

     if(sign != QDialog::Rejected) {
         emit exportGifSignal(byteArray.data());
     }


}
