#include "DHSD_SPI.h"

void DHSD_SPI::CSHigh(){
  digitalWrite(CS,HIGH);  
}

void DHSD_SPI::CSLow(){
  digitalWrite(CS,LOW);  
}

void DHSD_SPI::DCSHigh(){
  digitalWrite(DCS,HIGH);  
}

void DHSD_SPI::DCSLow(){
  digitalWrite(DCS,LOW);  
}

bool DHSD_SPI::Init(int InCS, int InDCS){//, SPISettings InSPISettings){
  CS=InCS;
  DCS=InDCS;
  //Settings=InSPISettings;
  Serial.print("CS=");
  Serial.println(CS);
  pinMode(CS,OUTPUT);
  pinMode(DCS,OUTPUT);
  SPI.begin();
  //SPI.setDataMode(SPI_MODE0);
  //SPI.setClockDivider(SPI_CLOCK_DIV128);
  return true;
};

char DHSD_SPI::Write(unsigned char * InBuffer, unsigned int InLength){

  unsigned char LastByte;
    
  while(InLength--){

  #ifdef debug
 //   Serial.print("Write 0x");  
 //   Serial.println(InBuffer[0],HEX);
  #endif
    LastByte = SPI.transfer(InBuffer[0]);
  
    #ifdef debug
 //   Serial.print("SPI Write LastByte = 0x");
  //  Serial.println(LastByte,HEX);
    #endif
    
    InBuffer++;
  };
    
  return LastByte;
};

unsigned char DHSD_SPI::Read(){

  return SPI.transfer(0x00);
 
};

