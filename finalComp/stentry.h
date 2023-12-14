#ifndef STENTRY_H
#define STENTRY_H
#include <stdio.h>
#include <string.h>
#include <iostream>

#define TYPE_SIZE 7

typedef enum
{
    STE_NONE, //0
    STE_INT,
    STE_STRING,
    STE_FLOAT,
    STE_DOUBLE,
    STE_CHAR,
    STE_CONST,
    STE_BOOLEAN,
}STE_TYPE;

static char *STE_TYPE_STR[TYPE_SIZE] = {"None","int","char*","float","double","char","const"};// It is not a good idea to put this here
static char  str[128];
class STEntry
{
public:

    char Name[64];
    STE_TYPE Type;  //it is an int
    STEntry *Next;
    int int_value;
    float  float_value;
    char* string_value;
    double double_value;
    char char_value;
    bool bool_value;
    STEntry() {
        Next = NULL;
        Type = STE_NONE;
        Name[0] =0; //empty String
    }
    STEntry(char *name, STE_TYPE type)
    {
        Next= NULL;
        Type = type;
        strcpy(Name, name);
    }
    char* toString()
    {
        if ((Type < STE_NONE) ||Type>STE_CHAR)  Type = STE_NONE;
        sprintf(str,"(%s,%s)",Name,STE_TYPE_STR[Type]);
        return str;
    }
    void print(FILE *fp)
    {
        fprintf(fp,"%s ", toString());
    }

    static STE_TYPE getType(char *str)
    {
        int i;
        for (i =0; i < TYPE_SIZE; i++){
            if(strcmp(STE_TYPE_STR[i],str) == 0) return ((STE_TYPE)i);
        }
        return STE_NONE;
    }

    int getIntValue (){
        return int_value;
    }

    float getFloatValue() {
        return float_value;
    }

    char* getStringValue() {
        return string_value;
    }

    double getDoubleValue() {
        return double_value;
    }

    char getCharValue() {
        return char_value;
    }

    void setIntValue(int new_value) {
        int_value = new_value;
    }

    void setFloatValue(float new_value) {
        float_value = new_value;
    }

    void setStringValue(const char* new_value) {
        strcpy(string_value, new_value);
    }

    void setDoubleValue(double new_value) {
        double_value = new_value;
    }

    void setCharValue(char new_value) {
        char_value = new_value;
    }


    void setBoolValue(bool new_value) {
        bool_value = new_value;
    }

    char *ste_name (){
        return Name;
    }

    STE_TYPE ste_var_type (){
        return Type;
    }
};

#endif // STENTRY_H
