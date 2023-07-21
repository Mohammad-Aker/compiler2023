#include "fd.h"
#include <string.h>

char* FileDescriptor::getFileName () {
    
    if(isOpen())
        return file;
    else {
        strcpy(error, "Please open the file first");
        return error;
    }
}

bool FileDescriptor::isOpen () {
    if(fp == NULL)
        return false;
    else 
        return true;
}

char* FileDescriptor::getCurrLine(){
    return buffer;
}

int FileDescriptor::getLineNum(){
    return line_number;
}

int FileDescriptor::getCharNum(){
    return char_number;
}

FileDescriptor::FileDescriptor(char *fileName){
    buffer = new char[bufSize];
    file   = new char[FILENAME_MAX];
    error  = new char[bufSize];
    
    bufSize     = BUFFER_SIZE;
    file        = fileName;
    char_number = 0;
    line_number = 0;
    char* filePath = new char[strlen(fileName) + 5];
    strcpy(filePath, fileName);
    strcat(filePath, ".txt"); 
    fp = fopen(filePath,"r");
    if(fp == NULL){
        strcpy(error, "Please open the file first");
    }
}

FileDescriptor::~FileDescriptor(){
    buffer = NULL;
    file   = NULL;
    error  = NULL;

    if (fp != nullptr) {
        fclose(fp);
    }
}

FileDescriptor::FileDescriptor(){
    //char temp[20];
    //strcpy(temp, "main");
    //FileDescriptor(temp);
    char fileName[5] = {'m','a','i','n'};

    buffer = new char[bufSize];
    file   = new char[FILENAME_MAX];
    error  = new char[bufSize];
    
    bufSize     = BUFFER_SIZE;
    file        = fileName;
    char_number = 0;
    line_number = 0;

    char* filePath = new char[strlen(fileName) + 5];
    strcpy(filePath, fileName);
    strcat(filePath, ".txt");
    fp = fopen(filePath,"r");
    if(fp == NULL){
        strcpy(error, "Please open the file first");
    } 
}

void FileDescriptor::close(){
        if (fp != nullptr) 
            fclose(fp);
}

char FileDescriptor::getChar(){
    
    char currentChar;
    currentChar = fgetc(fp);
    if(currentChar == '\n'){
        char_number = 0;
        line_number++;
    }
    else
        char_number++;

    return currentChar;
}

void FileDescriptor::reportError (char* msg){
    printf("%s",error);
}

void FileDescriptor::ungetChar (char c){
    char_number--;
}
