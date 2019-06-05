#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    SetList();

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::SetList(){

 /*   QListWidgetItem * MyItem = new QListWidgetItem;
   MyItem->setText("vvv");
   //MyItem->setData(Qt::UserRole,v);

   ui->listWidget->addItem(MyItem);
   ui->listWidget->update();

*/

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
                }

                delete [] DriveName;
                delete TargetString;
            }

        }

    }
}
