#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DeviceLabel.setText("No Device selected");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QString FName = FSDialog.getOpenFileName();

    QListWidgetItem * NewItem = new QListWidgetItem;
    NewItem->setText(FName);

    ui->listWidget->addItem(NewItem);
    ui->listWidget->update();

}

void MainWindow::on_action_SDCard_reader_triggered()
{
    SelectDevice SDevice;

    SDevice.exec();

    SDDevicePath=SDevice.getDeviceSelected();


    if(SDDevicePath.size()>0){

        DeviceLabel.setText(QString::fromStdString(SDDevicePath));
    }else{
        DeviceLabel.setText("No Device selected");
    }
    statusBar()->addWidget(&DeviceLabel);

}

void MainWindow::on_action_Exit_triggered()
{
    this->close();
}

void MainWindow::on_pushButton_4_clicked()
{

    if(ui->listWidget->selectedItems().size()>0){

        QList<QListWidgetItem *> SelectedList = ui->listWidget->selectedItems();

        for(int a=0;a<ui->listWidget->selectedItems().size();a++){


            int row = ui->listWidget->row(SelectedList.at(a));
            ui->listWidget->takeItem(row);

        }


    }


}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->listWidget->selectedItems().size()>0){


        if(ui->listWidget->row(ui->listWidget->selectedItems().at(0))>0){
            int row = ui->listWidget->row(ui->listWidget->selectedItems().at(0));
            if(row>0){
                QListWidgetItem * TempItem = ui->listWidget->takeItem(row);
                ui->listWidget->insertItem(row-1,TempItem);
            }
        }



    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->listWidget->selectedItems().size()>0){


        if(ui->listWidget->row(ui->listWidget->selectedItems().at(0))>0){
            int row = ui->listWidget->row(ui->listWidget->selectedItems().at(0));
            QListWidgetItem * TempItem = ui->listWidget->takeItem(row);
            ui->listWidget->insertItem(row+1,TempItem);
        }



    }
}

void MainWindow::on_pushButton_5_clicked()
{
    if(SDDevicePath.size()>0){

        // init direct access with selected device
        FMP.Init(SDDevicePath);

        // clear file list
        FMP.ClearFilesList();

        // add files to list

        for(int i = 0; i < ui->listWidget->count(); ++i)
        {
            QString str = ui->listWidget->item(i)->text();
            FMP.AddFilesToList(str.toStdString());
        }

        FMP.WriteFiles();





    }
}
