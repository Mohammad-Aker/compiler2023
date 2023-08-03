#ifndef STENTRY_H
#define STENTRY_H
#include <stdio.h>
#include <string.h>
#define TYPE_SIZE 6

typedef enum
{
    STE_NONE, //0
    STE_INT,
    STE_STRING,
    STE_FLOAT,
    STE_DOUBLE,
    STE_CHAR,
}STE_TYPE;

static char *STE_TYPE_STR[TYPE_SIZE] = {"None","int","char*","float","double","char"};// It is not a good idea to put this here
static char  str[128];
class STEntry
{
public:

    char Name[64];
    STE_TYPE Type;  //it is an int
    STEntry *Next;
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
};

#endif // STENTRY_H
