#ifndef DHSD_DHFS_GUARD
#define DHSD_DHFS_GUARD
#include "DHSD_SDCARD.h"
#include "DHSD_SPI.h"

class DHSD_DHFS{
  
  public:
    DHSD_DHFS();
    bool Init(DHSD_SPI * InCSPI);

    bool ReadSectorFromFile(unsigned long InFileNum, unsigned long InSectorOffset, unsigned char * OutBuffer);
    unsigned long GetTailBytes(unsigned long InFileNum);
    unsigned long GetNumberOfSectors(unsigned long InFileNum);
    

  private:
    DHSD_SDCARD SDCARD;

    unsigned char * InData;

    bool FSRead;

    unsigned long FirstSector;
    unsigned long NumberOfSectors;
    unsigned long TailBytes;

    
  
};

#endif
