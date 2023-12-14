#include <string>
#include "stable.h"
#include "functional"
#include <stdio.h>
#include <string.h>
#include <iostream>


void STable::init(int size)
{
    next = NULL;
    Size = size;
    Table = new STList[size];
}
/**
 * @brief STable::STable : Default constructor, call init with DEFAULT_SIZE
 */
STable::STable()
{
    number_entries = DEFAULT_SIZE;
    foldCase = 0;
    init(DEFAULT_SIZE);
}
/**
 * @brief STable::STable  constructor with Number of slots = size, call init with size
 * @param size
 */
STable::STable(unsigned long size, int foldCase)
{
    number_entries = size;
    this->foldCase = foldCase;
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
    number_entries = size;
    number_hits = 0;
    number_probes = 0;
    max_search_dist = -1;
}
/**
 * @brief STable::ElfHash : Hash Function
 * @param str  : Key which is the Variable Name to be added to the Symbol Table
 * @return
 */
unsigned long STable::ElfHash( char *str )
{
    unsigned long   h = 0, high; // h=0, g
//    unsigned char *s = (unsigned char *) str;
//    while ( *s )
//    {
//        h = ( h << 4 ) + *s++;  //1/4 of bits
//        if ( high = h & 0xF0000000 )
//            h ^= high >> 24; // h = h ^ (g >>24)
//        h &= ~high; // in book this is h = h^g
//    }
//    std::hash<std::string> hasher;
//    unsigned int h = hasher(str);
//    while (*str) {
//        h = (h * 31) + *str;
//        str++;
//    }
//    unsigned int h = 5381;
//
    while (*str) {
        h = ((h << 5) + h) + *str; // hashValue * 33 + *str
        str++;
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
    if(foldCase){
        char* temp;
        temp = name;
        while (*temp) {
            *temp = tolower(*temp);
            temp++;
        }
    }
    unsigned long index = ElfHash(name);
    bool Added = Table[index].AddEntry(name, type);
    if(Added){
        number_probes++;
    }
    if(Table[index].Count() == 1 && Added){
        number_hits++;
    }
    if(Table[index].Count() > max_search_dist){
        max_search_dist = Table[index].Count();
    }
    return Added;
}
/**
 * @brief STable::FindAndPrintEntry Finds and prints the Entry if it is found
 *                if Not found print the Message Not found, see the example result
 * @param name  : name to search for
 * @param fp  : File pointer to print : See Example in reults
 */
STEntry * STable::FindInScope(char *name)// may be find and print entry
{
    unsigned long index = ElfHash(name);
    STEntry *ste = Table[index].FindEntry(name);
    if(ste) {
        return ste;
    } // to be fixed , se
    else {
        return nullptr;
    }

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
    print_symbol_stats(fp);

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

void STable::print_symbol_stats (FILE *fp){
    fprintf(fp,"\nNumber of entries in table: %d\n", number_entries);
    fprintf(fp,"Number of probes into table: %d\n", number_probes);
    fprintf(fp,"Number of probes that immediately found entry: %d\n", number_hits);
    fprintf(fp,"Maximum entries searched: %d\n", max_search_dist);
}


STable * STable::enter_scope(STable *s) {
    STable *newScope = new STable();
    newScope->next = s;
    return newScope;
}

STable * STable::exit_scope(STable *s) {
    s = s->next;
    return  s;
}

STEntry* STable::FindEntry(char *name) {
    STable * temp = this;
    STEntry * entry;
    while (temp != nullptr){
        entry = temp->FindInScope(name);
        if(entry != nullptr){
            return entry;
        }
        temp = temp->next;
    }
    return entry;
}

void STable::FindAndPrintEntry(char *name, FILE *fp) {
    STEntry* entry = FindEntry(name);
    if(entry != nullptr)
        fprintf(fp,"\nfound\n");
    else
        fprintf(fp,"\nnot found\n");
}
