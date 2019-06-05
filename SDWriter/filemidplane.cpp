#include "filemidplane.h"

FileMidplane::FileMidplane()
{



}



bool FileMidplane::Init(std::string InDevicePath){

    if(FS.Init(InDevicePath)){
        return true;
    }

    return false;
};

bool FileMidplane::WriteFiles(){

    bool result=true;


    if(FileList.size()>0){

      // iterate files and write it to SDCard
        for (unsigned int a=0; a<FileList.size(); a++){

            DHFSFile FileToWrite;
            FileToWrite.SetFileName(FileList.at(a));
            FileToWrite.SetFileNumber(FS.GetNextFileNumber());

            FS.AddFile(FileToWrite);

        }

        FS.WriteToSD();

    }else{
        result = false;
    }
    return result;
};


void FileMidplane::AddFilesToList(std::string InFilePathName){

    FileList.push_back(InFilePathName);

};

void FileMidplane::ClearFilesList(){

    FileList.resize(0);
    FS.Clear();


};
