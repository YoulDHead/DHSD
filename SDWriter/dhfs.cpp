#include "dhfs.h"

DHFSFile::DHFSFile(){

    FileNumber=0;
    FirstSectorNumber=0;
    NumberOfSectors=0;
    TailBytesNumber=0;


};

bool DHFSFile::SetFileNumber(unsigned int InFileNumber){
    if(InFileNumber<=0xffff){
        FileNumber=InFileNumber;
        return true;
    }
    return false;
};

bool DHFSFile::SetFirstSectorNumber(unsigned int InFirstSectorNumber){
    if(InFirstSectorNumber<=0xffffff){
        FirstSectorNumber=InFirstSectorNumber;
        return true;
    }
    return false;
};

bool DHFSFile::SetNumberOfSectors(unsigned int InNumberOfSectors){
    if(InNumberOfSectors<=0xffffff){
        NumberOfSectors=InNumberOfSectors;
        return true;
    }
    return false;
};

bool DHFSFile::SetTailBytesNumber(unsigned int InTailBytesNumber){
    if(InTailBytesNumber<=0xffff){
        TailBytesNumber=InTailBytesNumber;
        return true;
    }
    return false;
};

bool DHFSFile::SetFileName(std::string InFileName){

    if(InFileName.size()>0){
        FileName=InFileName;
        return true;
    }
    return false;
};

unsigned int DHFSFile::GetFileNumber(){
    return FileNumber;
};         // 2 bytes

unsigned int DHFSFile::GetFirstSectorNumber(){
    return FirstSectorNumber;
};  // 3 bytes

unsigned int DHFSFile::GetNumberOfSectors(){
    return NumberOfSectors;
};    // 3 bytes

unsigned int DHFSFile::GetTailBytesNumber(){
    return TailBytesNumber;
};    // 2 bytes

std::string DHFSFile::GetFileName(){
    return FileName;
};

DHFS::DHFS()
{
    SdInit=false;

};

void DHFS::Clear(){

    Files.resize(0);

}

bool DHFS::Init(std::string InDevPath)
{

    NextFileNumber=0;
    std::wstring stemp = SDCard.s2ws(InDevPath);
    LPCWSTR DevPath = stemp.c_str();

    if(SDCard.OpenDevice(DevPath)){

        SdInit=true;
        return true;

    }

    return true;
};

void DHFS::AddFile(DHFSFile InFile){

    Files.push_back(InFile);


};

unsigned int DHFS::GetNextFileNumber(){

    NextFileNumber++;
    return NextFileNumber-1;

}

bool DHFS::WriteToSD(){

    unsigned int FirstSector = 2;
    for(unsigned int a=0;a<Files.size();a++){


        Files.at(a).SetFirstSectorNumber(FirstSector);

        FILE * FileDesc = fopen(Files.at(a).GetFileName().c_str(),"rb");

        if(FileDesc!=NULL){

            unsigned int Sectors = 0;
            unsigned int OutBufPos = 0;
            char * OutBuf = new char [512];
            memset(OutBuf,0,512);

            char * Buf = new char [512];
            memset(Buf,0,512);
            int BytesRead=1;

            while(BytesRead>0){
                BytesRead = fread(Buf, 1, 512, FileDesc);

                if(BytesRead>0){
                    for(int b=0;b<BytesRead;b++){

                        if(OutBufPos==511){
                            // write sector
                            SDCard.WriteSector(512,FirstSector,OutBuf);
                            OutBufPos = 0;
                            FirstSector++;
                            Sectors++;
                        }else{
                            OutBuf[OutBufPos]=Buf[b];
                            OutBufPos++;
                        }
                    }
                }else{

                    if(OutBufPos>0){
                        SDCard.WriteSector(512,FirstSector,OutBuf);
                        FirstSector++;
                        Sectors++;

                        Files.at(a).SetNumberOfSectors(Sectors);
                        Files.at(a).SetTailBytesNumber(OutBufPos);
                        OutBufPos = 0;
                        Sectors=0;
                    }
                }

            }
            fclose(FileDesc);
        }

    }


    // write DHFS header
    char * buf =new char [512];
    memset(buf,0,512);

    const char FSMark [5] {"DHFS"};

    memcpy(buf,&FSMark,4);

    buf[4]=1;

    int pos = 4;
    for(int a=0;a<Files.size();a++){

        unsigned int filenum = Files.at(a).GetFileNumber();
        unsigned int secnum = Files.at(a).GetFirstSectorNumber();
        unsigned int numofsec = Files.at(a).GetNumberOfSectors();
        unsigned int tailbytes = Files.at(a).GetTailBytesNumber();

        memcpy((buf+pos),&filenum,sizeof(unsigned int));
        pos+=sizeof(unsigned int);
        memcpy((buf+pos),&secnum,sizeof(unsigned int));
        pos+=sizeof(unsigned int);
        memcpy((buf+pos),&numofsec,sizeof(unsigned int));
        pos+=sizeof(unsigned int);
        memcpy((buf+pos),&tailbytes,sizeof(unsigned int));
        pos+=sizeof(unsigned int);
    }

    SDCard.WriteSector(512,0,buf);

    return true;

}
