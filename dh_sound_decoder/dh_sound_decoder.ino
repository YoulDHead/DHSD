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
  0x52,0x49,0x46,0x46,
  0x24,0xD0,0x0C,0x00,
  0x57,0x41,0x56,0x45,
  0x66,0x6D,0x74,0x20,
  0x10,0x00,0x00,0x00,
  0x01,0x00,0x01,0x00,
  0x44,0xAC,0x00,0x00,
  0x88,0x58,0x01,0x00,
  0x02,0x00,0x10,0x00,
  0x64,0x61,0x74,0x61,
  0x00,0xD0,0x0C,0x00  
  };

unsigned char * FeedBuffer1;
unsigned char * FeedBuffer2;

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
  unsigned char * buf = new unsigned char [512];

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

  // make feed buffers
  FeedBuffer1 = new unsigned char [512];
  FeedBuffer2 = new unsigned char [512];



  unsigned long Sectors0 = FS.GetNumberOfSectors(0);
  unsigned long Sectors1 = FS.GetNumberOfSectors(1);
  unsigned long TailBytes0 = FS.GetTailBytes(0);
  unsigned long TailBytes1 = FS.GetTailBytes(1);

  Serial.print("Sectors0=");
  Serial.println(Sectors0, DEC);
  Serial.print("Sectors1=");
  Serial.println(Sectors1, DEC);
  Serial.print("TailBytes0=");
  Serial.println(TailBytes0, DEC);
  Serial.print("TailBytes1=");
  Serial.println(TailBytes1, DEC);


  // 1 put Syntetic wave header into FeedBuffer;
  memcpy(FeedBuffer1, &SynteticWaveHeader, 44);





  while (flag) {
    flag = FS.ReadSectorFromFile(0, Sectors, buf);
    //   Serial.print("Sector ");
    //    Serial.println(Sectors,DEC);

    if (Sectors == 0) {
      memset(buf + 4, 0xff, 4);
      memset(buf + 40, 0xff, 4);
    }

    while (!VS.ReadyForFeed());

    if (Sectors == Sectors0) {
      VS.FeedData(buf, TailBytes0);
    } else {
      VS.FeedData(buf, 512);
    }
    flag3 = false;
    Sectors++;
    //   Serial.print("Loop ");

  }

  /*  int SkipCounter=0;

    if(FalseWavHeader){
      Serial.println("False Header");
      memset(InSoundData+4,0xff,4);
      memset(InSoundData+40,0xff,4);
    }


    int Chunks = 16;
    int LastChunkSize = 32;
    int SkipBytes=0;

    if(SkipWavHeader){
      Serial.println("Skip Header");
      SkipBytes=44;
      TailBytes = 468;

    }


  */

  while (1) {
    //    Serial.println(" stage 2");

    Sectors = 0;
    flag = true;
    bool flag2 = true;

    while (flag) {

      flag = FS.ReadSectorFromFile(1, Sectors, buf);
      Serial.print("Sector ");
      Serial.println(Sectors, DEC);

      while (!VS.ReadyForFeed());

      if (Sectors == Sectors1) {
        VS.FeedData(buf, TailBytes1);
      } else {
        if (Sectors == 0) {
          VS.FeedData(buf + 44, 468);
        } else {
          VS.FeedData(buf, 512);
        }
      }

      flag2 = false;
      Sectors++;
    }
  }
};

void loop() {
  // put your main code here, to run repeatedly:
  while (1);

};
