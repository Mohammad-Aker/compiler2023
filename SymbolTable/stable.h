#ifndef STABLE_H
#define STABLE_H
#include "stlist.h"
#define DEFAULT_SIZE 19
class STable
{
private:
    unsigned long Size;
    STList *Table;   // Dynamic Array of size = Size
    void init(int size);
    unsigned long ElfHash(char *str);
public:
    STable();
    ~STable();
    STable(unsigned long size);
    void Reset(unsigned long size);

    bool AddEntry(char *name, STE_TYPE type);
    void FindAndPrintEntry(char *name, FILE *fp);//finds and prints the Entry if it exist
    void PrintAll(FILE *fp);
    void Clear();
};

#endif // STABLE_H
