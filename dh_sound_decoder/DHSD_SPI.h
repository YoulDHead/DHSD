#ifndef DHSD_SPI_GUARD
#define DHSD_SPI_GUARD

#include <SPI.h>

//#define debug

class DHSD_SPI{
  
  public:

    bool Init(int InCS, int InDCS);
    char Write(unsigned char * InBuffer, unsigned int InLength);
    void CSHigh();
    void CSLow();
    void DCSHigh();
    void DCSLow();
    unsigned char Read();

  private:

    int CS;
    int DCS;
    SPISettings Settings;
    
};


#endif
