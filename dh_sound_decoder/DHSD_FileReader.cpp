#include "DHSD_FileReader.h"

void DHSD_FileReader::Init(DHSD_DHFS * FS, byte InFileNum, bool InSkipHeader) {

  LFS = * FS;
  FileNum = InFileNum;
  Sectors = LFS.GetNumberOfSectors(FileNum);
  TailBytes = LFS.GetTailBytes(FileNum); \
  CurrentSector = 0;
  CurrentPos = 0;
  memset(ReadBuffer, 0, 512);
  SkipHeader = InSkipHeader;

#ifdef debug
  Serial.println("FileReader::Init");
  Serial.print("File number = ");
  Serial.println(FileNum, DEC);
  Serial.print("NOS=");
  Serial.println(Sectors, DEC);
  Serial.print("TailBytes=");
  Serial.println(TailBytes, DEC);
#endif

};


bool DHSD_FileReader::ReadNext(byte * BufferOut, int InSize, int * OutSize) {

  // first run
  if (CurrentSector == 0 && CurrentPos == 0) {
    if (SkipHeader) {
      CurrentPos = 44;
    }
#ifdef debug
    Serial.print("0 CurrentSector ");
    Serial.println(CurrentSector, DEC);
    Serial.print("0 CurrentPos ");
    Serial.println(CurrentPos, DEC);

#endif

    LFS.ReadSectorFromFile(FileNum, 0, ReadBuffer);

    byte b = 0;
    for (int a = 0; a < 512; a++) {
      Serial.print(" 0x");
      Serial.print(ReadBuffer[a], HEX);
      b++;
      if (b == 16) {
        Serial.println();
        b = 0;
      }

    }
      Serial.println();
  }

  // if we have enough data in the current sector
  if (InSize < (512 - CurrentPos)) {

#ifdef debug
    Serial.print("1 CurrentPos ");
    Serial.println(CurrentPos, DEC);
    Serial.print("1 InSize ");
    Serial.println(InSize, DEC);
#endif

    memcpy(BufferOut, ReadBuffer + CurrentPos, InSize);
    CurrentPos += InSize;
    *OutSize = InSize;

#ifdef debug
    Serial.print("1 OutSize ");
    Serial.println(*OutSize, DEC);
#endif


  } else {
    // if we need to read some data from the next sector
    // we need to copy some data
    memcpy(BufferOut, ReadBuffer + CurrentPos, (512 - CurrentPos));
    *OutSize = (512 - CurrentPos);
    // now we need to read next sector if we can
    if (CurrentSector < Sectors) {
      CurrentSector++;
      LFS.ReadSectorFromFile(FileNum, CurrentSector, ReadBuffer);

      byte b = 0;
      for (int a = 0; a < 512; a++) {
        Serial.print(" 0x");
        Serial.print(ReadBuffer[a], HEX);
        b++;
        if (b == 16) {
          Serial.println();
          b = 0;
        }

      }
      Serial.println();

      memcpy(BufferOut + *OutSize, ReadBuffer, (InSize - *OutSize));
      CurrentPos = (InSize - *OutSize);
      *OutSize += CurrentPos;

    }

  }

  return true;


};
