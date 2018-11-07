#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawingwindow.h"
#include <QPixmap>
#include <QMessageBox>
#include <QColorDialog>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    //void userChoseSize(int size);

private slots:
    void on_fileNew_triggered();

    void on_colorSelectButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
