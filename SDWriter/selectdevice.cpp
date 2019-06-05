#include "ui_selectdevice.h"
#include "selectdevice.h"
#include "windows.h"
#include <iostream>

SelectDevice::SelectDevice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDevice)
{

    ui->setupUi(this);
    PrepareDeviceList();

}

SelectDevice::~SelectDevice()
{

    for(int i=0;i<MyItems.size();i++){
        delete MyItems.at(i);
    }

    delete ui;
}

void SelectDevice::PrepareDeviceList(){

    ui->listWidget->setSelectionBehavior( QAbstractItemView::SelectItems );
    ui->listWidget->setSelectionMode( QAbstractItemView::SingleSelection );

    // get bitmask for existing drives
    DWORD DrivesMask = GetLogicalDrives();
    if(DrivesMask!=0){
        for(unsigned int a=0;a<sizeof(DrivesMask)*8;a++){
            if(((DrivesMask>>a)&1)==1){
                char DriveLetter=(0x41+a);

                char * DriveName = new char [3];
                DriveName[0]=DriveLetter;
                DriveName[1]=':';
                DriveName[2]=0;

                char * TargetString= new char [65536];

                std::string TempString(DriveName);

                DWORD res = QueryDosDeviceA(DriveName,TargetString,65536);
                if(res>0 && res!=ERROR_INSUFFICIENT_BUFFER){
                    std::string TempString2 (TargetString);
                    QVariant v = QVariant(QString::fromStdString(TempString2));
                    QListWidgetItem * MyItem = new QListWidgetItem;
                    MyItem->setText(QString::fromStdString(TempString));
                    MyItem->setData(Qt::UserRole,v);

                    ui->listWidget->addItem(MyItem);
                    ui->listWidget->update();
                    MyItems.push_back(MyItem);
                }

                delete [] DriveName;
                delete TargetString;
            }

        }

    }

};

std::string SelectDevice::getDeviceSelected(){

    return DeviceSelected;

};

void SelectDevice::setDeviceSelected(){

    QList<QListWidgetItem*> SelectedItems = ui->listWidget->selectedItems();

    if(SelectedItems.size()>0){
        QVariant v = SelectedItems.at(0)->text();//data(Qt::UserRole);
        QString s = v.toString();
        DeviceSelected = s.toStdString();
        DeviceSelected = DeviceSelected.insert(0,"\\\\.\\");
    }else{
        DeviceSelected="";
    }

};

void SelectDevice::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{

   setDeviceSelected();

}

void SelectDevice::on_buttonBox_accepted()
{

    setDeviceSelected();

}

void SelectDevice::on_buttonBox_rejected()
{
    setDeviceSelected();
}

