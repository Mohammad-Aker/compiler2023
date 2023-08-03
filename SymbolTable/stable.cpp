#include "stable.h"

void STable::init(int size)
{
   Size = size;
   Table = new STList[size];
}
/**
 * @brief STable::STable : Default constructor, call init with DEFAULT_SIZE
 */
STable::STable()
{
    init(DEFAULT_SIZE);
}
/**
 * @brief STable::STable  constructor with Number of slots = size, call init with size
 * @param size
 */
STable::STable(unsigned long size)
{
    init(size);
}
/**
 * @brief STable::Reset   Recreate the Table by calling clear then call init
 * @param size
 */
void STable::Reset(unsigned long size)
{
    Clear();
    init(size);
}
/**
 * @brief STable::ElfHash : Hash Function
 * @param str  : Key which is the Variable Name to be added to the Symbol Table
 * @return
 */
unsigned long STable::ElfHash( char *str )
{
    unsigned long   h = 0, high; // h=0, g
    unsigned char *s = (unsigned char *) str;
    while ( *s )
    {
        h = ( h << 4 ) + *s++;  //1/4 of bits
        if ( high = h & 0xF0000000 )
            h ^= high >> 24; // h = h ^ (g >>24)
        h &= ~high; // in book this is h = h^g
    }
    return h%Size;
}
/**
 * @brief STable::AddEntry : Adds the Entry, call Hash to find index, then call Add to the List in Table[index] to add it
 * @param name : Name of Entry to be added
 * @param type : type of Entry. See the Main function for example how to find it
 * @return  true if added and false otherwise, the List Table[index] Already returns this for you
 */
bool STable::AddEntry(char *name, STE_TYPE type)
{
    unsigned long index = ElfHash(name);
    bool Added = Table[index].AddEntry(name, type);
    return Added;
}
/**
 * @brief STable::FindAndPrintEntry Finds and prints the Entry if it is found
 *                if Not found print the Message Not found, see the example result
 * @param name  : name to search for
 * @param fp  : File pointer to print : See Example in reults
 */
void  STable::FindAndPrintEntry(char *name, FILE *fp)// may be find and print entry
{
    unsigned long index = ElfHash(name);
    STEntry *ste = Table[index].FindEntry(name);
    if(ste) fprintf(fp,"%s: Found = %s\n", name, ste->toString()); // to be fixed , se
    else fprintf(fp,"%s: not found \n",name);

}
/**
 * @brief STable::PrintAll : Prints all Elements. Print the Count of each list and the Entries.
 *                            See the Sample Result (fout.txt) to format your output
 *                            Loop through the Slots (Lists) and Call Table[i].printAll
 * @param fp
 */

void STable::PrintAll(FILE *fp)
{
    unsigned long i;
    for (i =0; i < Size; i++)
    {
        fprintf(fp,"T[%d]: %d ste:\t", i, Table[i].Count());
        Table[i].PrintAll(fp);
    }

}
/**
 * @brief STable::Clear : Delete All Elements, use a loop and call the Table[i].clear. Then Delete the Array Table
 */
void STable::Clear()
{
    unsigned long i;
    for (i =0; i < Size; i++)
    {
        Table[i].Clear();
    }
    delete[]Table;

}
STable::~STable()
{
    Clear();
}


