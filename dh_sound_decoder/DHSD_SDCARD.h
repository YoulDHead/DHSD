#ifndef DHSD_SDCARD_GUARD
#define DHSD_SDCARD_GUARD

#include "DHSD_SPI.h"
//#define debug

class DHSD_SDCARD{
  public:

    bool Init(DHSD_SPI * InCSPI);
    bool Read(unsigned long InBlockNumber,unsigned char * OutBlock);
    bool Write(unsigned long InBlockNumber,unsigned char * InBlock);
  private:
    bool CMD0();
    bool CMD1();
    char CMD8();
    bool CMD17(unsigned long InBlockNumber);
    char CMD41();
    bool CMD55();
    bool CMD59();


    bool InitV1();
    bool InitV2();
    DHSD_SPI CSPI;      
  
};


#endif
