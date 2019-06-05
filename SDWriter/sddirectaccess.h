#ifndef SDDIRECTACCESS_H
#define SDDIRECTACCESS_H
#include "windows.h"
#include <iostream>
#include <winioctl.h>
#include <sstream>

class SDDirectAccess
{
public:
    SDDirectAccess();

    void Test();

    bool OpenDevice(LPCWSTR InDeviceName);
    bool CloseDevice();

    DWORD ReadSector(int InSectorSize, int InSectorNumber, char * OutBuffer);
    DWORD WriteSector(int InSectorSize, int InSectorNumber, char * InBuffer);


    bool QueryDevices();

    std::wstring s2ws(const std::string& s);
private:

    HANDLE FH;
    bool Initiated;
};

#endif // SDDIRECTACCESS_H
