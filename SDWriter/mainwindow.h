#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "selectdevice.h"
#include <QLabel>
#include <QFileDialog>
#include "filemidplane.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_action_SDCard_reader_triggered();

    void on_action_Exit_triggered();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QLabel DeviceLabel;
    std::string SDDevicePath;
    QFileDialog FSDialog;

    FileMidplane FMP;


};

#endif // MAINWINDOW_H
