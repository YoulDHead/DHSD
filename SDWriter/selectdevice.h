#ifndef SELECTDEVICE_H
#define SELECTDEVICE_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class SelectDevice;
}

class SelectDevice : public QDialog {

    Q_OBJECT

public:
    explicit SelectDevice(QWidget *parent = 0);
    ~SelectDevice();
    std::string getDeviceSelected();
    void PrepareDeviceList();

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();



private:
    Ui::SelectDevice *ui;

    std::string DeviceSelected;
    std::vector <QListWidgetItem*> MyItems;
    void setDeviceSelected();

};

#endif // SELECTDEVICE_H
