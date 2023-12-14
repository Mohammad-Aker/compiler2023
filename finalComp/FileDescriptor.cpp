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
    currentChar = (char)fgetc(fp);
    buffer[char_number] = currentChar;
    if(currentChar == 10){
        char_number = 0;
        line_number++;
    }
    else
        char_number++;
    buffer[char_number+1] = '\0';
    return currentChar;
}

void FileDescriptor::reportError (char* msg){
    line_number++;
    printf("Error %s in line %d \n",msg, line_number);
    printf("Error -> %s\n",buffer);
    for(int i = 0; i < char_number; i++){
        printf(" ");
    }
    printf("         ^ \n");
}

void FileDescriptor::ungetChar (char c){
    char_number--;
    if(c == 10){
        line_number--;
    }
    fseek(fp, -1, SEEK_CUR);
}

int FileDescriptor::getBufferSize(){
    return bufSize;
}
