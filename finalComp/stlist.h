#ifndef STLIST_H
#define STLIST_H
#include "stentry.h"

class STList
{
private:
    int Counter;  // counts the Node n the List
    STEntry *Head; // Should be Initialized to Null in the Constructor
public:

    STList();
    STEntry *FindEntry(char *name); // return NULL if Not found
    bool AddEntry(char *name, STE_TYPE type);//Adds an entry if the Node Does Not exist
    void PrintAll(FILE *fp);
    int Count();
    void Clear();
    ~STList();
};

#endif // STLIST_H
