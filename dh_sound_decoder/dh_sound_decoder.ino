#include "DHSD_SDCARD.h"
#include "DHSD_VS1053.h"
#include "DHSD_DHFS.h"
#include "DHSD_FileReader.h"

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
  0xFF, 0xFF, 0xFF, 0xFF,
  0x57, 0x41, 0x56, 0x45,
  0x66, 0x6D, 0x74, 0x20,
  0x10, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x01, 0x00,
  0x44, 0xAC, 0x00, 0x00,
  0x88, 0x58, 0x01, 0x00,
  0x02, 0x00, 0x10, 0x00,
  0x64, 0x61, 0x74, 0x61,
  0xFF, 0xFF, 0xFF, 0xFF
};

unsigned char * FeedBuffer1;
unsigned char * FeedBuffer2;

bool BufSelector;
bool LockBuf1;
bool LockBuf2;

bool LockFlag;

unsigned char * FirstBuffer;
unsigned char * SecondBuffer;

unsigned char * ResultBuffer;

bool SynteticHeader;
bool FirstTime;
int OutSize;
DHSD_DHFS FS;

DHSD_VS1053 VS;

DHSD_FileReader FR1;


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
  //VS.SineTest();
  VS.StartPlayer();


  // make feed & result buffers
  FeedBuffer1 = new unsigned char [512];
  FeedBuffer2 = new unsigned char [512];

  FirstBuffer = new unsigned char [512];
  SecondBuffer = new unsigned char [512];
  ResultBuffer = new unsigned char [512];

  BufSelector = false;
  LockBuf1 = false;
  LockBuf2 = false;

  FirstTime = true;
  FR1.Init(&FS, 0, false);



};

void FeedData() {


  if (!BufSelector) {
    Serial.println("Feed first");
    // feed buffer
    VS.FeedData(FeedBuffer1, 512);
    byte b = 0;
    for (int a = 0; a < 512; a++) {
      Serial.print(" 0x");
      Serial.print(FeedBuffer1[a], HEX);
      b++;
      if (b == 16) {
        Serial.println();
        b = 0;
      }

    }
    Serial.println();
    memset(FeedBuffer1, 0, 512);
    BufSelector = true;
  
  }else{
  
    Serial.println("Feed second");
    VS.FeedData(FeedBuffer2, 512);
    byte b = 0;
    for (int a = 0; a < 512; a++) {
      Serial.print(" 0x");
      Serial.print(FeedBuffer2[a], HEX);
      b++;
      if (b == 16) {
        Serial.println();
        b = 0;
      }

    }
    Serial.println();
    memset(FeedBuffer2, 0, 512);
    BufSelector = false;
  }

}


void loop() {

  /* LockBuf1 = true;
      LockBuf2 = false;
      BufSelector = true;
  */
  FirstTime=false;
  if (FirstTime) {
    memcpy(ResultBuffer, SynteticWaveHeader, 44);

    int OutSize = 0;
    FR1.ReadNext(FirstBuffer, 468, &OutSize);
    memcpy(ResultBuffer + 44, FirstBuffer, 468);

    FirstTime = false;
  } else {

    if (FR1.ReadNext(FirstBuffer, 512, &OutSize)) {
      memcpy(ResultBuffer, FirstBuffer, 512);
    } else {
      memcpy(ResultBuffer, FirstBuffer, OutSize);
    }
  }

  if (!BufSelector) {
    Serial.println("Fill first");
    memcpy(FeedBuffer1, ResultBuffer, 512);

  } else {
    Serial.println("Fill second");
    memcpy(FeedBuffer2, ResultBuffer, 512);
  }



  FeedData();
  /*
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
          //SecondChannelEnabled=true;
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
        if (SecondSectorsProcessed == (SecondSectors - 1)) {
          //SecondChannelEnabled = false;
          SecondSectorsProcessed = 1;
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

      if (SynteticHeader) {
        // put syntetic wave header into FeedBuffer;
        memcpy(ResultBuffer, &SynteticWaveHeader, 44);
        SynteticHeader = false;
      }

      // process second logical channel

      // Feed prepared data
      FeedData();
    }*/
};
