#include "DHSD_FileReader.h"

void DHSD_FileReader::Init(DHSD_DHFS * FS, byte InFileNum) {

  LFS = * FS;
  FileNum = InFileNum;
  Sectors = LFS.GetNumberOfSectors(FileNum);
  TailBytes = LFS.GetTailBytes(FileNum);\
  CurrentSector=0;
  CurrentPos=0;
  memset(ReadBuffer,0,512);

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


bool DHSD_FileReader::ReadNext(byte * BufferOut,int InSize, int * OutSize){

  // check we already have some bytes to return
  if(CurrentPos>0){
    Serial.println("CurrentPos>0");
    if((512-CurrentPos)>InSize){
      memcpy(BufferOut,ReadBuffer+CurrentPos,InSize);
      CurrentPos+=InSize;
      #ifdef debug
      Serial.print("Cpos>0,Data>Insize, CurrentPos=");
      Serial.println(CurrentPos,DEC);
      #endif
    }else{
      memcpy(BufferOut,ReadBuffer+CurrentPos,(512-CurrentPos));
      CurrentSector+=1;
      LFS.ReadSectorFromFile(FileNum, CurrentSector, SecondBuffer);
      memcpy(BufferOut+(512-CurrentPos),ReadBuffer,(InSize-(512-CurrentPos));
      CurrentPos=(InSize-(512-CurrentPos);
      #ifdef debug
      Serial.print("Cpos>0,Data<Insize, CurrentPos=");
      Serial.println(CurrentPos,DEC);
      #endif
    }       
  }else{
    Serial.println("CurrentPos
      
    
  }

  
  
  // check current sector == 0 we need to cut wav header
  
};
