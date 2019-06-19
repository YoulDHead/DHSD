#include "DHSD_SDCARD.h"
#include "DHSD_VS1053.h"
#include "DHSD_DHFS.h"

//######################################## MAIN DEFS ###################################
#define debug

DHSD_SPI SD_SPI;
DHSD_SPI VS_SPI;
//DHSD_SPI VS_CSPI;


int sd_cs = PA4; //chip select for sd card
int vs_cs = PB14; // chip select for vs1053b data XDCS
int vs_dcs = PB15; // chip select fro vs1053b command XCS
int vs_dreq = PB12; // data ready for vs1053b

int vs_xreset = PB13;

unsigned char  SynteticWaveHeader [44] = {
  0x52, 0x49, 0x46, 0x46,
  0x24, 0xD0, 0x0C, 0x00,
  0x57, 0x41, 0x56, 0x45,
  0x66, 0x6D, 0x74, 0x20,
  0x10, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x01, 0x00,
  0x44, 0xAC, 0x00, 0x00,
  0x88, 0x58, 0x01, 0x00,
  0x02, 0x00, 0x10, 0x00,
  0x64, 0x61, 0x74, 0x61,
  0x00, 0xD0, 0x0C, 0x00
};

unsigned char * FeedBuffer1;
unsigned char * FeedBuffer2;
bool FeedBufferSwitch;

unsigned char * FirstBuffer;
unsigned char * SecondBuffer;

unsigned char * ResultBuffer;

bool SynteticHeader;

DHSD_DHFS FS;

DHSD_VS1053 VS;
//######################################## MAIN CODE ###################################

void SetupSPI() {

  SD_SPI.Init(sd_cs, -1);
  VS_SPI.Init(vs_cs, vs_dcs);
  ////  VS_CSPI.Init(vs_command_cs);

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


  if (FS.Init(&SD_SPI)) {
    Serial.println("FS ok");
  }

  VS.Init(&VS_SPI, vs_xreset, vs_dreq);
  VS.SineTest();
  VS.StartPlayer();
  unsigned long Sectors = 0;
  bool flag = true;
  bool flag3 = false;
  Serial.println("stage 1");

  // make feed & result buffers
  FeedBuffer1 = new unsigned char [512];
  FeedBuffer2 = new unsigned char [512];

  FeedBufferSwitch = false;

  FirstBuffer = new unsigned char [512];
  SecondBuffer = new unsigned char [512];
  ResultBuffer = new unsigned char [512];

  SynteticHeader = true;

};

void loop() {

  // main sound loop
  bool MSLoop = true;

  char FirstChannelFile = 0;
  bool FirstChannelEnabled = true;


  char SecondChannelFile = 1;
  bool SecondChannelEnabled = false;

  unsigned long FirstSectors = FS.GetNumberOfSectors(FirstChannelFile);
  unsigned long SecondSectors = FS.GetNumberOfSectors(SecondChannelFile);
  unsigned long FirstTailBytes = FS.GetTailBytes(FirstChannelFile);
  unsigned long SecondTailBytes = FS.GetTailBytes(SecondChannelFile);

  unsigned long FirstSectorsProcessed = 0;
  unsigned long SecondSectorsProcessed = 1;

  while (MSLoop) {

    // clear result buffer
    memset(ResultBuffer, 0x00, 512);

    // process first logical channel

    if (FirstChannelEnabled) {
      Serial.println("First enabled");
      Serial.print("FirstChannelFile=");
      Serial.println(FirstChannelFile, DEC);

      Serial.print("FirstSectorsProcessed=");
      Serial.println(FirstSectorsProcessed, DEC);
      Serial.print("FirstSectors=");
      Serial.println(FirstSectors, DEC);
  
      FirstChannelEnabled = FS.ReadSectorFromFile(FirstChannelFile, FirstSectorsProcessed, FirstBuffer);
      if (FirstSectorsProcessed == (FirstSectors)) {
        FirstChannelEnabled = false;
        //SecondChannelEnabled=true;*/
      }

      if (FirstSectorsProcessed == 0)    {
        memcpy(ResultBuffer, FirstBuffer + 44, 468);
      } else {

        if (FirstSectorsProcessed == (FirstSectors)) {

          memcpy(ResultBuffer, FirstBuffer, FirstTailBytes);
        } else {
          memcpy(ResultBuffer, FirstBuffer, 512);
        }


      }
      
      FirstSectorsProcessed++;

    }
    if (SecondChannelEnabled) {
      Serial.println("Second enabled");
      Serial.print("SecondChannelFile=");
      Serial.println(SecondChannelFile, DEC);

      Serial.print("SecondSectorsProcessed=");
      Serial.println(SecondSectorsProcessed, DEC);
      Serial.print("SecondSectors=");
      Serial.println(SecondSectors, DEC);
    
      SecondChannelEnabled = FS.ReadSectorFromFile(SecondChannelFile, SecondSectorsProcessed, SecondBuffer);
      if (SecondSectorsProcessed == (SecondSectors-1)) {
        //SecondChannelEnabled = false;
        SecondSectorsProcessed=1;
      }

      if (SecondSectorsProcessed == 0)    {
        memcpy(ResultBuffer, SecondBuffer + 44, 468);
      } else {

        if (SecondSectorsProcessed == SecondSectors) {

          memcpy(ResultBuffer, SecondBuffer, SecondTailBytes);
        } else {
          memcpy(ResultBuffer, SecondBuffer, 512);
        }


      }
      SecondSectorsProcessed++;

    }

    if(SynteticHeader){
      // put syntetic wave header into FeedBuffer;
      memcpy(ResultBuffer, &SynteticWaveHeader, 44);
      SynteticHeader=false;
    }

    // process second logical channel

    // feed buffer
    VS.FeedData(ResultBuffer, 512);

  }
};
