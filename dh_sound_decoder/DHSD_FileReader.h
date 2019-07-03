#ifndef DHSD_FILEREADER_GUARD
#define DHSD_FILEREADER_GUARD
#include "DHSD_DHFS.h"

#define debug
class DHSD_FileReader {

  public:
    void Init(DHSD_DHFS * FS, byte InFileNum, bool InSkipHeader);
    bool ReadNext(byte * BufferOut,int InSize, int * OutSize);
    

  private:
    unsigned long Sectors;
    unsigned long CurrentSector;
    unsigned long TailBytes;
    DHSD_DHFS LFS;
    byte FileNum;

    byte ReadBuffer[512];
    int CurrentPos;
    bool SkipHeader;

};


#endif // DHSD_FILEREADER_GUARD
