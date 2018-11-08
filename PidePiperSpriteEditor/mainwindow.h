#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawingwindow.h"
#include <QPixmap>
#include <QObject>
#include <QMessageBox>
#include <QColorDialog>
#include "model.h"

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

private slots:
    void on_fileNew_triggered();

    void on_colorSelectButton_clicked();

    void on_addFrameButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
