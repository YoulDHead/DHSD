#ifndef DHFS_H
#define DHFS_H

#include <vector>
#include <string>
#include "sddirectaccess.h"
#include <fcntl.h>


class DHFSFile{

public:
    DHFSFile();

    bool SetFileNumber(unsigned int InFileNumber);
    bool SetFirstSectorNumber(unsigned int InFirstSectorNumber);
    bool SetNumberOfSectors(unsigned int InNumberOfSectors);
    bool SetTailBytesNumber(unsigned int InTailBytesNumber);
    bool SetFileName(std::string InFileName);

    unsigned int GetFileNumber();         // 2 bytes
    unsigned int GetFirstSectorNumber();  // 3 bytes
    unsigned int GetNumberOfSectors();    // 3 bytes
    unsigned int GetTailBytesNumber();    // 2 bytes
    std::string GetFileName();

private:

    unsigned int FileNumber;         // 2 bytes
    unsigned int FirstSectorNumber;  // 3 bytes
    unsigned int NumberOfSectors;    // 3 bytes
    unsigned int TailBytesNumber;    // 2 bytes

    std::string FileName;

};


#define DHFS_SECTOR_SIZE    512

class DHFS
{
public:
    DHFS();

    bool WriteToSD();
    void AddFile(DHFSFile);
    unsigned int GetNextFileNumber();
    bool Init(std::string InDevPath);
    void Clear();

private:

    std::vector<DHFSFile> Files;
    unsigned int NextFileNumber;
    SDDirectAccess SDCard;
    bool SdInit;

};

#endif // DHFS_H
