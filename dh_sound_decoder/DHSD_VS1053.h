#ifndef DHSD_VS1053_GUARD
#define DHSD_VS1053_GUARD

#include "DHSD_SPI.h"

#define DHSD_VS1053_REG_MODE  0x00
#define DHSD_VS1053_REG_STATUS 0x01
#define DHSD_VS1053_REG_BASS 0x02
#define DHSD_VS1053_REG_CLOCKF 0x03
#define DHSD_VS1053_REG_DECODETIME 0x04
#define DHSD_VS1053_REG_AUDATA 0x05
#define DHSD_VS1053_REG_WRAM 0x06
#define DHSD_VS1053_REG_WRAMADDR 0x07
#define DHSD_VS1053_REG_HDAT0 0x08
#define DHSD_VS1053_REG_HDAT1 0x09
#define DHSD_VS1053_REG_VOLUME 0x0B

#define DHSD_VS1053_SCI_READ 0x03
#define DHSD_VS1053_SCI_WRITE 0x02

#define DHSD_VS1053_GPIO_DDR 0xC017
#define DHSD_VS1053_GPIO_IDATA 0xC018
#define DHSD_VS1053_GPIO_ODATA 0xC019

#define DHSD_VS1053_INT_ENABLE  0xC01A

#define DHSD_VS1053_MODE_SM_DIFF 0x0001
#define DHSD_VS1053_MODE_SM_LAYER12 0x0002
#define DHSD_VS1053_MODE_SM_RESET 0x0004
#define DHSD_VS1053_MODE_SM_CANCEL 0x0008
#define DHSD_VS1053_MODE_SM_EARSPKLO 0x0010
#define DHSD_VS1053_MODE_SM_TESTS 0x0020
#define DHSD_VS1053_MODE_SM_STREAM 0x0040
#define DHSD_VS1053_MODE_SM_SDINEW 0x0800
#define DHSD_VS1053_MODE_SM_ADPCM 0x1000
#define DHSD_VS1053_MODE_SM_LINE1 0x4000
#define DHSD_VS1053_MODE_SM_CLKRANGE 0x8000


#define DHSD_VS1053_SCI_AIADDR 0x0A
#define DHSD_VS1053_SCI_AICTRL0 0x0C
#define DHSD_VS1053_SCI_AICTRL1 0x0D
#define DHSD_VS1053_SCI_AICTRL2 0x0E


class DHSD_VS1053{

  public:
    bool Init(DHSD_SPI * InCSPI, int InXRST);

  private:
    unsigned char Reset();
    void WarmReset();

    unsigned int ControlRead(unsigned char InAddress);
    void ControlWrite(unsigned char InAddress, unsigned int InData);

    unsigned char DataRead();
    void DataWrite(unsigned char * InData, unsigned int InLength);

    void SetVolume(char InVolume);
    
    DHSD_SPI XSPI;
   
    void SineTest();

    int XRST;

};


#endif
