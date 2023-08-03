#include <stdlib.h>
#include "stable.h"
#include <ctime>
/**
 * @brief getIdetifier: reads the next Identifier, and its type from the file
 * @param fp
 * @param str    : Identifier is stored here
 * @param type   : stores the type read
 * @return
 */
int  getIdetifier(FILE* fp, char *str, STE_TYPE& type)
{
    char s1[64],s2[64],s3[64];
    int n = fscanf(fp,"%s%s%s", s1,s2,s3);

    //printf ("(n= %d,%s,%s,%s)\n", n, s1, s2, s3);

    strcpy(str,s2);
    type = STEntry::getType(s1);  // notice how we call a static function or data member
   // printf ("-(%d,%s)\n",(int)type, str); to make sure  it is reading correctly
    return n;
}
/**
 * @brief getRandChar  Returns a random char
 * @param isFirst: is First= true character returned is a random char in 'A' --- 'Z'
 *                 else returns a 'A' ---'Z' or '0' -- '9'
 * @return
 */
char getRandChar(bool IsFirst=false)
{
    int index =0;
    char ch;
    if (IsFirst) index = rand()%26;
    else index = rand()%(26+10) ; // 10 for 0--9
    if(index <  26) ch = 'A' + index;
    else ch = '0' + (index - 26);
    return ch;
}
/**
 * @brief getRandIdentifier: generates a random 8-char variable name
 * @param str: Stores the generated string
 * @param type: Type of variable generated, random from the types defined in STentry.h
 */
void getRandIdentifier (char *str, STE_TYPE& type)
{
    int i;
    str[0] = getRandChar(true);
    for (i = 1 ; i < 8; i++)
    {
        str[i] = getRandChar();
    }
    str[i]=0;//NULL terminate
    type = (STE_TYPE)(rand()%TYPE_SIZE);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    STable  ST;

    STE_TYPE type ;
    char str[64];
    fp = fopen("t.txt","r");
    if(!fp) return 0;
    FILE *fout = fopen("out.txt","w");   //set fout = stdout for screen output

    fprintf(fout, "\n**(Test 1)**: Reading from the file t.txt and creating a symnol table with size = 19(default)\n\n");
    int n;
    bool Added;
    while (!feof(fp))
    {
        n = getIdetifier(fp,str,type);
        if(n == 3)
        {
           Added = ST.AddEntry(str,type);
           if(!Added)fprintf(fout,"Entry %s Already Exists\n",str );
        }
    }
    fclose(fp);


    fprintf(fout,"\n==============\n");
    ST.PrintAll(fout);
    fprintf(fout,"\n==============\n");

    fprintf(fout, "\n\n**(Test 2)**: Searching for some Variables in the Symobol Table Created\n\n");
    ST.FindAndPrintEntry("variable1111", fout);
    ST.FindAndPrintEntry("jklm", fout);
    ST.FindAndPrintEntry("x12", fout);
    ST.FindAndPrintEntry("xx12", fout);

    fprintf(fout, "\n\n**(Test 3)**: Creatig 8 Symbol Tables with different Slot Size and Different Sizes\n");
    fprintf(fout, "            : Random Variable Names are generated\n");
    srand (time(0));

    int k, i;
    int Sizes[10]         = {29, 29, 17, 23, 37, 31,101, 101, 101,101};
    int ElementsCount[10] = {100, 100, 100, 100,100, 150, 300, 100, 200, 50} ;

    STable ST2(Sizes[8]);//Initil table with 29 elements
    k = 0;
    for(k = 0; k < 10; k++)
    {
        if(k !=0 ) ST2.Reset(Sizes[k]);
        for(i=0; i < ElementsCount[k]; i++) // create a 100 element table
        {
            getRandIdentifier (str, type);
            Added = ST2.AddEntry(str,type);
            if(!Added)fprintf(fout,"Entry %s Already Exists\n",str );

        }
        fprintf(fout,"\n===== Table (%d) =======\n", k);
        ST2.PrintAll(fout);

    }
    fprintf(fout,"\n------------------done-------------------\n");
    fclose(fout);
    return 0;
}

