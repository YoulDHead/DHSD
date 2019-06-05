#ifndef FILEMIDPLANE_H
#define FILEMIDPLANE_H
#include <string.h>
#include "dhfs.h"

class FileMidplane
{
public:
    FileMidplane();

    bool Init(std::string InDevicePath);

    bool WriteFiles();

    void AddFilesToList(std::string InFilePathName);

    void ClearFilesList();

private:

    std::vector <std::string> FileList;

    DHFS FS;

};

#endif // FILEMIDPLANE_H
