#include "sddirectaccess.h"


SDDirectAccess::SDDirectAccess()
{
    Initiated=false;
}


std::wstring SDDirectAccess::s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

bool SDDirectAccess::OpenDevice(LPCWSTR InDeviceName){


    HANDLE hDevice = INVALID_HANDLE_VALUE;  // handle to the drive to be examined
      BOOL bResult   = FALSE;                 // results flag
      DWORD junk     = 0;                     // discard results
      DWORD dwSize;

      hDevice = CreateFileW(InDeviceName,          // drive to open
                            0,                // no access to the drive
                            FILE_SHARE_READ | // share mode
                            FILE_SHARE_WRITE,
                            NULL,             // default security attributes
                            OPEN_EXISTING,    // disposition
                            0,                // file attributes
                            NULL);            // do not copy file attributes

      if (hDevice == INVALID_HANDLE_VALUE)    // cannot open the drive
      {
        return (FALSE);
      }

    STORAGE_DEVICE_NUMBER diskNumber;
    DWORD bytesReturned;
    DeviceIoControl( hDevice,
                                    IOCTL_STORAGE_GET_DEVICE_NUMBER,
                                    NULL,
                                    0,
                                    &diskNumber,
                                    sizeof( STORAGE_DEVICE_NUMBER ),
                                    &bytesReturned,
                                    NULL );


    std::string PhysicalDeviceName;
    std::stringstream MYSS;

    MYSS<<diskNumber.DeviceNumber;
    MYSS>>PhysicalDeviceName;

    PhysicalDeviceName.insert(0,"\\\\?\\PhysicalDrive");

    std::wstring stemp = s2ws(PhysicalDeviceName);
    LPCWSTR DevPath = stemp.c_str();

    FH = CreateFile(DevPath,
                           GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_READ | FILE_SHARE_WRITE,
                           0,
                           OPEN_EXISTING,
                           0,
                           0);
;

    if(FH==INVALID_HANDLE_VALUE){

        std::cout<<"SDDirectAccess::OpenDevice(LPCWSTR InDeviceName)::Error::"<<GetLastError()<<std::endl;
        return false;


    }
    Initiated=true;
    return true;

};

bool SDDirectAccess::CloseDevice(){

    if(CloseHandle(FH)==0){
        std::cout<<"SDDirectAccess::CloseDevice()::Error::"<<GetLastError()<<std::endl;
        return false;
    }
    return true;
};

DWORD SDDirectAccess::ReadSector(int InSectorSize, int InSectorNumber, char * OutBuffer){

    if(INVALID_SET_FILE_POINTER==SetFilePointer(FH,InSectorSize*InSectorNumber,NULL,FILE_BEGIN)){

        std::cout<<"SDDirectAccess::ReadSector(int InSectorSize, int InSectorNumber, char * OutBuffer)::Error::"<<GetLastError()<<std::endl;
        return 0;

    };
    DWORD bytesRead=0;
    if(ReadFile(FH,OutBuffer,InSectorSize,&bytesRead,NULL)){
        return bytesRead;
    }

    return 0;

};
DWORD SDDirectAccess::WriteSector(int InSectorSize, int InSectorNumber, char * InBuffer){

    if(INVALID_SET_FILE_POINTER==SetFilePointer(FH,InSectorSize*InSectorNumber,NULL,FILE_BEGIN)){

        std::cout<<"SDDirectAccess::WriteSector(int SectorSize, int SectorNumber, char * InBuffer)::Error::"<<GetLastError()<<std::endl;
        return 0;

    };

    DWORD BytesWritten=0;
    if(WriteFile(FH,InBuffer,InSectorSize,&BytesWritten,NULL)){
        return BytesWritten;
    }

    return 0;

};


void SDDirectAccess::Test()
{


    /*if(OpenDevice(L"\\\\.\\physicaldrive3")){
*/
    char rBuffer [512];
    DWORD bytesRead=0;

        if(ReadFile(FH,&rBuffer,512,&bytesRead,NULL)){
            std::cout<<"bytesRead="<<bytesRead<<std::endl;
        }




}



bool SDDirectAccess::QueryDevices()
{

    DWORD BitMask = GetLogicalDrives();

    for(int a=0;a<sizeof(DWORD)*8;a++){

        int dtemp = (BitMask>>a) & 0x01;
        std::cout<<"a="<<a<<" dtemp="<<dtemp<<std::endl;
    }

    WCHAR * Buffer = new WCHAR [10024];
    DWORD result = QueryDosDevice(NULL,Buffer,10024);

    for(int b=0;b<10024;b++){
        char m=Buffer[b];
        std::cout<<" "<<m;
    }
    std::cout<<std::endl;


    return false;
}
