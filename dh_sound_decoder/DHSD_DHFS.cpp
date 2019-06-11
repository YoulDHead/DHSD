#include "DHSD_DHFS.h"

DHSD_DHFS::DHSD_DHFS() {

}

bool DHSD_DHFS::Init(DHSD_SPI * InCSPI) {

  InData = new unsigned char [512];

  if (SDCARD.Init(InCSPI)) {
    SDCARD.Read(0, InData);
    return true;
  };

  return false;
};

unsigned long DHSD_DHFS::GetTailBytes(unsigned long InFileNum) {

  memcpy(&TailBytes, reinterpret_cast<unsigned long *>(InData + 4 + 16 * InFileNum + 12), sizeof(unsigned long));
  return TailBytes;
};

unsigned long DHSD_DHFS::GetNumberOfSectors(unsigned long InFileNum) {

  memcpy(&NumberOfSectors, reinterpret_cast<unsigned long *>(InData + 4 + 16 * InFileNum + 8), sizeof(unsigned long));
  return NumberOfSectors;
};

bool DHSD_DHFS::ReadSectorFromFile(unsigned long InFileNum, unsigned long InSectorOffset, unsigned char * OutBuffer) {

  // pos = 4+16*InFileNum 16 bytes

  memcpy(&FirstSector, reinterpret_cast<unsigned long *>(InData + 4 + 16 * InFileNum + 4), sizeof(unsigned long));
  memcpy(&NumberOfSectors, reinterpret_cast<unsigned long *>(InData + 4 + 16 * InFileNum + 8), sizeof(unsigned long));
  memcpy(&TailBytes, reinterpret_cast<unsigned long *>(InData + 4 + 16 * InFileNum + 12), sizeof(unsigned long));
  if (InSectorOffset <= NumberOfSectors) {

    SDCARD.Read((FirstSector + InSectorOffset) * 0x200, OutBuffer);

    return true;
  }
  return false;


};
/*memcpy(&FileNum,reinterpret_cast<unsigned long *>(InData+pos),sizeof(unsigned long));
  memcpy(&TFile.FirstSector,reinterpret_cast<unsigned long *>(InData+pos),sizeof(unsigned long));
  memcpy(&TFile.NumberOfSectors,reinterpret_cast<unsigned long *>(InData+pos),sizeof(unsigned long));
  memcpy(&TFile.TailBytes,reinterpret_cast<unsigned long *>(InData+pos),sizeof(unsigned long));
*/


