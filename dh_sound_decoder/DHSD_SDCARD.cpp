#include "DHSD_SDCARD.h"


bool DHSD_SDCARD::InitV1(){
  return CMD1();
};
bool DHSD_SDCARD::InitV2(){
  
  char state=-1;
  
  unsigned char inbuf=0xff;

  // shift 4 bytes of CMD8 answer
  for(char a=0;a<4;a++){
    CSPI.Write(&inbuf,1);    
  }

  char c=0;
  do {

      if(CMD55()){
        state=CMD41();
        
  #ifdef debug
  Serial.print("Initv2 state=0x");
  Serial.println(state,HEX);
  #endif

      }
      c++;
      if(c>100){
          return false;
      }
  }while (state!=0x00); 

  CSPI.CSHigh();
    
  return true;  
  
};

bool DHSD_SDCARD::Init(DHSD_SPI * InCSPI){
  #ifdef debug
  Serial.println("SDCARD init started");
  #endif
  CSPI = *InCSPI;

  CSPI.CSHigh();
  unsigned char buf=0xff; 
  for (char a=0;a<10;a++){
    CSPI.Write(&buf,1);
  }
  CSPI.CSLow();
  
  CMD0();
  char SDType=CMD8();

  bool InitResult=false;
  
  switch (SDType){
      // v1
      case 1:
        #ifdef debug
        Serial.println("Card type 1");
        #endif

        InitResult = InitV1();
      break;
      // v2
      case 2:
        #ifdef debug
        Serial.println("Card type 2");
        #endif
        InitResult = InitV2();
      break;  
      default:
        #ifdef debug
        Serial.println("Unknown SD card");
        #endif
        InitResult = false;
      break;
  }
  CMD59();
  
  #ifdef debug
  if(InitResult){ 
    Serial.println("Init Complete");
  }else{
    Serial.println("Init error");
  };   
  #endif
  CSPI.CSHigh();

  return InitResult;
};

bool DHSD_SDCARD::CMD0(){

  #ifdef debug
  Serial.println("SDCARD-CMD0");
  #endif
  unsigned char buf [6] = {0x40,0x00,0x00,0x00,0x00,0x95};

  CSPI.Write((unsigned char*)&buf,6);
  unsigned char inbuf=0xff;
  int a=0;
  
  do{
    if(CSPI.Write(&inbuf,1)==0x01){
      #ifdef debug
      Serial.println("CMD0 - 0x01 received");    
      #endif
      return true;
    };
    a++;
  }while(a<10);
  
  return false;
};

bool DHSD_SDCARD::CMD1(){
 
  #ifdef debug
  Serial.println("SDCARD-CMD1");
  #endif
 
  unsigned char inbuf=0xff;
 
 // shift 1 byte of CMD0 answer
  CSPI.Write(&inbuf,1);  

  unsigned char *  buf = new unsigned char [6] {0x41,0x00,0x00,0x00,0x00,0x00};

  CSPI.Write(buf,6);
 
  unsigned char state=0;
  int loops = 0;
  do{

    state=CSPI.Write(&inbuf,1);
    if(state==0x00){
      #ifdef debug
        Serial.println("SDCARD-CMD1 Card type 1");
      #endif
      delete [] buf;
      return true;      
    }
    loops++;
  }while(loops<100);
  
  delete [] buf;
  return false;

}

char DHSD_SDCARD::CMD8(){
  
  #ifdef debug
  Serial.println("SDCARD-CMD8");
  #endif
  
  unsigned char inbuf=0xff;

  // stub
  CSPI.Write(&inbuf,1);  

  unsigned char *  buf = new unsigned char [6] {0x48,0x00,0x00,0x01,0xAA,0x87};

  CSPI.Write(buf,6);
 
  unsigned char state=0;
  int loops = 0;
  do{

    state=CSPI.Write(&inbuf,1);
    if(state==0x04){
      #ifdef debug
        Serial.println("SDCARD-CMD8 Card type 1");
      #endif
      delete [] buf;
      return 1;      
    }

    if(state==0x01){
      #ifdef debug
        Serial.println("SDCARD-CMD8 Card type 2");
      #endif
      delete [] buf;
      return 2;      
      
    }

    loops++;

  }while(loops<10);
    
  delete [] buf;
};

bool DHSD_SDCARD::CMD17(unsigned long InBlockNumber){

  #ifdef debug
  Serial.println("SDCARD-CMD17");
  #endif
  
  unsigned char inbuf=0xff;
  
  unsigned char *  buf = new unsigned char [6] {0x51,((InBlockNumber>>24)&0xff),((InBlockNumber>>16)&0xff),((InBlockNumber>>8)&0xff),((InBlockNumber)&0xff),0x00};

  #ifdef debug
  for(int a=0;a<6;a++){
    Serial.println(buf[a],HEX);  
  }
  #endif
  CSPI.Write(buf,6); 

  unsigned char state=0;
  int loops = 0;
  do{
    state=CSPI.Write(&inbuf,1);
    #ifdef debug
    Serial.print("state = 0x");
    Serial.println(state,HEX);
    #endif
    if(state==0x00){
      #ifdef debug
        Serial.println("SDCARD-CMD17 Ok");
      #endif
      delete [] buf;
      return true;      
    }
    
    loops++;
  }while(loops<100);

  delete [] buf;
  return false;

};

char DHSD_SDCARD::CMD41(){
  #ifdef debug
  Serial.println("SDCARD-CMD41");
  #endif
 
  unsigned char inbuf=0xff;
 
  unsigned char *  buf = new unsigned char [6] {0x69,0x40,0x00,0x00,0x00,0xFF};
  CSPI.Write(buf,6);

  // shift one byte
  CSPI.Write(&inbuf,1);  

  delete [] buf;
  return  CSPI.Write(&inbuf,1);  
;    
};

bool DHSD_SDCARD::CMD55(){
  
  #ifdef debug
  Serial.println("SDCARD-CMD55");
  #endif
 
  unsigned char inbuf=0xff;
 
  CSPI.Write(&inbuf,1);  

  unsigned char *  buf = new unsigned char [6] {0x77,0x00,0x00,0x00,0x00,0x00};

  CSPI.Write(buf,6);
 
  unsigned char state=0;
  int loops = 0;
  do{

    state=CSPI.Write(&inbuf,1);
      #ifdef debug
        Serial.print("SDCARD-CMD55 State=0x");
        Serial.println(state,HEX);
      #endif
    
    if(state==0x01){
      #ifdef debug
        Serial.println("SDCARD-CMD55 Ok");
      #endif
      delete [] buf;
      return true;      
    }
    loops++;
  }while(loops<1000);
      #ifdef debug
        Serial.println("SDCARD-CMD55 failed");
      #endif
  delete [] buf;
  return false;    
       
}

bool DHSD_SDCARD::CMD59(){
  
  #ifdef debug
  Serial.println("SDCARD-CMD55");
  #endif
 
  unsigned char inbuf=0xff;
 
  unsigned char *  buf = new unsigned char [6] {0x7B,0x00,0x00,0x00,0x00,0x00};

  CSPI.Write(buf,6);
 
  unsigned char state=0;
  int loops = 0;
  do{

    state=CSPI.Write(&inbuf,1);
    if(state==0xFF){
      #ifdef debug
        Serial.println("SDCARD-CMD59 Ok");
      #endif
      delete [] buf;
      return true;      
    }
    loops++;
  }while(loops<100);
  
  delete [] buf;
  return false;      
  
};

bool DHSD_SDCARD::Read(unsigned long InBlockNumber,unsigned char * OutBlock){

  unsigned char inbuf=0xff;
  

  CSPI.CSLow();
  
  for(char a=0;a<10;a++){
    CSPI.Write(&inbuf,1);
  }

  
  bool result=false;
  if(CMD17(InBlockNumber)){
    while (CSPI.Write(&inbuf,1)!=0xFE); // wait for data mark
  
  //    Serial.print ("Read data from sector ");
    //  Serial.println(InBlockNumber,DEC);
  
      char ByteCounter=0;
      for (int i=0; i<512; i++){
        OutBlock[i]=CSPI.Write(&inbuf,1); // get data byte by byte
        
    
        if(OutBlock[i]<=0x0f){
          Serial.print("0x0");
        }else{
          Serial.print("0x");
        }
        Serial.print(OutBlock[i],HEX);
        Serial.print(" ");
        if(ByteCounter==15){
        Serial.println();
          ByteCounter=0;  
        }else{
          ByteCounter++;  
        }
        
      }
      
      //Serial.println();
      //Serial.println();
      //Serial.println();
      //get CRC
      CSPI.Write(&inbuf,1); 
      CSPI.Write(&inbuf,1);
      result=true;
       
  };
  CSPI.CSHigh();
  
  return result;
};



