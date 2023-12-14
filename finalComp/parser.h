//
// Created by mohammadaker on 8/15/2023.
//

#ifndef COMPPARSER_PARSER_H
#define COMPPARSER_PARSER_H
#include "scanner.h"
#include "STable.h"
#include "ast.h"


class Parser {
private:
    SCANNER scanner;
    TOKEN* currentToken;
    FileDescriptor *fd;
    STable *symbolTable;
    ast_list *code;

    void consume(LEXEM_TYPE expectedType);
    ast_list * parseDeclList();
    AST* parseDecl();
    ast_list * parseDeclListTail();
    ste_list * parseFormalsPrime();
    ste_list * parseFormals();
    ste_list * parseFormalList();
    ste_list * parseFormalListTail();
    STE_TYPE parseType();
    AST* parseStmt();
    AST* parseIfTail();
    AST * parseIdTail(STEntry * entry);
    ast_list * parseArgList();
    ast_list * parseArgListTail();
    ast_list * parseArgs();
    ast_list * parseArgsTail();
    AST* parseBlock(bool flage = false);
    ste_list* parseVarDeclList();
    ste_list* parseVarDeclListTail();
    STEntry * parseVarDecl();
    ast_list * parseStmtList();
    ast_list * parseStmtListTail();
    AST* parseExpr();
    AST* parseExprTail(AST * larg);
    AST* parseExpr1();
    AST* parseExpr1Tail(AST * larg);
    AST* parseExpr2();
    AST* parseExpr2Tail(AST * larg);
    AST* parseExpr3();
    AST* parseExpr3Tail(AST * larg);
    AST* parseExpr4();
    AST* parseExpr5();
    AST* parseValue();
    ast_list * parseValueTail();

public:
    Parser(FileDescriptor *fdt, STable *symboltable);
    ast_list * parseProgram();


};




#endif //COMPPARSER_PARSER_H
