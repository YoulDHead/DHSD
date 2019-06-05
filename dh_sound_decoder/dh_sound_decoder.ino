#include "DHSD_SDCARD.h"
#include "DHSD_VS1053.h"

//######################################## MAIN DEFS ###################################
#define debug

DHSD_SPI SD_SPI;
DHSD_SPI VS_SPI;
//DHSD_SPI VS_CSPI;


int sd_cs = PA4; //chip select for sd card
int vs_cs = PB14; // chip select for vs1053b data XDCS
int vs_dcs = PB15; // chip select fro vs1053b command XCS

int vs_xreset = PB13;

DHSD_SDCARD SDCARD;
DHSD_VS1053 VS;
//######################################## MAIN CODE ###################################

void SetupSPI(){

  SD_SPI.Init(sd_cs,-1);
//  VS_SPI.Init(vs_cs,vs_dcs);
  //VS_CSPI.Init(vs_command_cs);
    
};


void setup() {
  // Setup serial
  delay(2000);
  Serial.begin(115200);
 
  Serial.println("Started");

  // Setup SPI 
  Serial.println("init SPI");
  SetupSPI();
  Serial.println("init SD card");
  unsigned char * buffer = new unsigned char [512];
  if(SDCARD.Init(&SD_SPI)){

 
    for(int i=0;i<4;i++){
    Serial.println("Read!");
      if(SDCARD.Read(i*0x200,buffer)){
        int k=0;
       
      };
    Serial.println("Read end!");
  

    }

  //VS.Init(&VS_SPI,vs_xreset);
  }
  
};

void loop() {
  // put your main code here, to run repeatedly:
while(1);

};
