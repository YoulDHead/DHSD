#include "DHSD_VS1053.h"

unsigned int DHSD_VS1053::ControlRead(unsigned char InAddress){

  unsigned int Data=0;
  XSPI.CSLow();
  unsigned char buf [2] = {DHSD_VS1053_SCI_READ,InAddress};

  DataWrite((unsigned char*)&buf,2);

  delayMicroseconds(10);

  Data = DataRead();
  Data<<=8;
  Data |= DataRead();

  Serial.print("Control read - 0x");
  Serial.println(Data,HEX);

  XSPI.CSHigh();

  return Data;
  
}


void DHSD_VS1053::ControlWrite(unsigned char InAddress, unsigned int InData){

  XSPI.CSLow();

  unsigned char buf [4] = {DHSD_VS1053_SCI_WRITE,InAddress,(InData>>8),(InData&0xff)};

  DataWrite((unsigned char*)&buf,4);
  XSPI.CSHigh();
  

}

unsigned char DHSD_VS1053::DataRead(){

  unsigned char Data=0;
 
  Data = XSPI.Read();
  Serial.print("Dataread - 0x");
  Serial.println(Data,HEX);
  return Data;
  
}


void DHSD_VS1053::DataWrite(unsigned char * InData, unsigned int InLength){

  
  XSPI.Write(InData,InLength);  
  
  
};


void DHSD_VS1053::SineTest(){
  uint16_t mode = ControlRead(DHSD_VS1053_REG_MODE);
  mode |= 0x0020;
  ControlWrite(DHSD_VS1053_REG_MODE, mode);

//  while (!digitalRead(_dreq));
     delay(10);

  XSPI.DCSLow();

  unsigned char buf [8] = {0x53,0xef,0x6e,0x44,0x00,0x00,0x00,0x00};
  
  XSPI.Write((unsigned char*)&buf,8);
  
  XSPI.DCSHigh();

  
  delay(500);
  XSPI.DCSLow();

  unsigned char buf1 [8] = {0x45,0x78,0x69,0x74,0x00,0x00,0x00,0x00};
  
  XSPI.Write((unsigned char*)&buf1,8);
  
  XSPI.DCSHigh();

};

void DHSD_VS1053::WarmReset(){

  ControlWrite(DHSD_VS1053_REG_MODE, DHSD_VS1053_MODE_SM_SDINEW | DHSD_VS1053_MODE_SM_RESET);
  delay(100);
  
}


unsigned char DHSD_VS1053::Reset(){
   
  digitalWrite(XRST,LOW);
  delay(100);
  digitalWrite(XRST,HIGH);
  
  XSPI.CSHigh();
  XSPI.DCSHigh();

  delay(100);

  WarmReset();

  delay(100);

  ControlWrite(DHSD_VS1053_REG_CLOCKF, 0x6000);

  SetVolume(40);

  char reg=ControlRead(DHSD_VS1053_REG_MODE);

  Serial.print("Mode = 0x"); Serial.println(reg, HEX);

  reg=ControlRead(DHSD_VS1053_REG_STATUS);

  Serial.print("Stat = 0x"); Serial.println(reg, HEX);

  reg=ControlRead(DHSD_VS1053_REG_CLOCKF);
  
  Serial.print("ClkF = 0x"); Serial.println(reg, HEX);

  reg=ControlRead(DHSD_VS1053_REG_VOLUME);
  
  Serial.print("Vol. = 0x"); Serial.println(reg, HEX);

  return (ControlRead(DHSD_VS1053_REG_STATUS)>>4)&0x0f;
  
}

void DHSD_VS1053::SetVolume(char InVolume){
  
  unsigned int Volume = InVolume;
  Volume <<= 8;
  Volume |= InVolume;

  noInterrupts();
  ControlWrite(DHSD_VS1053_REG_VOLUME,Volume);
  interrupts();

};

bool DHSD_VS1053::Init(DHSD_SPI * InSPI, int InXRST){
    
  XSPI = *InSPI;

  // set reset pin
  XRST=InXRST;
  pinMode(XRST,OUTPUT);
  digitalWrite(XRST,LOW);

  XSPI.CSHigh();
  XSPI.DCSHigh();
  
  unsigned char ver = Reset();
  Serial.print("Version - ");
  Serial.println(ver);
  SineTest();
  
   
  
};

