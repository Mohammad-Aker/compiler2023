#include <stdlib.h>
#include "stable.h"
#include <ctime>
#include <iostream>
#include "parser.h"
#include "ast.h"

int main()
{
    FileDescriptor *fd = new FileDescriptor("C:\\Users\\mohammadaker\\CLionProjects\\FinalComp\\t");
    FILE *fp;
    fp = fopen("C:\\Users\\mohammadaker\\CLionProjects\\FinalComp\\out.txt", "w");
    STable *table = new STable();
    Parser *p = new Parser(fd, table);
    ast_list * code = p->parseProgram();
    while (code != nullptr){
        print_ast_node(fp, code->head);
        code = code->tail;
    }
    delete [] code;
    return 0;
}

