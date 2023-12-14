#include <string>
#include <iostream>
#include "parser.h"
#include "ast.cpp"
#include "scanner.h"


Parser::Parser(FileDescriptor *fdt, STable *symboltable) {
    SCANNER temp(fdt);
    scanner=temp;
    symbolTable= symboltable;
    currentToken = scanner.Scan();
}

void Parser::consume(LEXEM_TYPE expectedType) {
    if (currentToken->type == expectedType) {
        printf("%s \n",TOKEN_NAMES[currentToken->type]);
        currentToken = scanner.Scan();
        if(currentToken->type==lx_error){
            exit(0);
        }
    } else {
        printf("Line %d : Expected token is %s",scanner.Fd->getLineNum() + 1, TOKEN_NAMES[expectedType]);
        exit(0);
    }
}

ast_list * Parser::parseProgram() {
    return parseDeclList();
}

ast_list * Parser::parseDeclList(){
    ast_list * s = new ast_list;
    s->head = parseDecl();
    s->tail = parseDeclListTail();
    return s;
}

ast_list * Parser::parseDeclListTail(){
    if(currentToken->type == lx_semicolon){
        consume(lx_semicolon);
        ast_list * s = new ast_list;
        s = parseDeclList();
        return s;
    }
    else{
       if(currentToken->type==lx_eof)
           exit(0);
       else
        consume(lx_semicolon);

    }
    return nullptr;
}

AST* Parser::parseDecl(){
    if(currentToken->type == kw_var){
        consume(kw_var);
        char * name = new char[64];
        strcpy(name, currentToken->str_ptr);
        consume(lx_identifier);
        consume(lx_colon);
        STE_TYPE type = parseType();
        bool added = symbolTable->AddEntry(name, type);
        if(added == false){
            printf("Line %d : Entry %s Already exist\n",scanner.Fd->getLineNum() + 1, name);
            exit(0);
        }
        STEntry* entry = symbolTable->FindEntry(name);
        AST* node = make_ast_node(2, ast_var_decl, entry, type);
        return node;
    }
    else if(currentToken->type == kw_constant){
        consume(kw_constant);
        char * name = new char[64];
        strcpy(name, currentToken->str_ptr);
        consume(lx_identifier);
        consume(lx_eq);
        AST* expr = parseExpr();
        int value = eval_ast_expr(fd ,expr);
        symbolTable->AddEntry(name, STE_CONST);
        STEntry * entry = symbolTable->FindEntry(name);
        return make_ast_node(2, ast_const_decl, entry, value);
    }
    // im here
    else if(currentToken->type == kw_function){
        consume(kw_function);
        char * name = new char[64];
        if(currentToken->type==lx_identifier)
            strcpy(name,currentToken->str_ptr);
        consume(lx_identifier);
        ste_list * formals = new ste_list;
        formals = parseFormalList();
        consume(lx_colon);
        STE_TYPE type = parseType();
        AST* body = parseBlock(true);
        bool added = symbolTable->AddEntry(name, type);
        if(added == false){
            printf("Line %d : Entry %s Already exist\n",scanner.Fd->getLineNum() + 1, name);
            exit(0);
        }
        STEntry* entry = symbolTable->FindEntry(name);
        return make_ast_node(4, ast_routine_decl, entry, formals,type, body);
    }
    else if(currentToken->type == kw_procedure){
        consume(kw_procedure);
        char * name = new char[64];
        if(currentToken->type==lx_identifier)
            strcpy(name,currentToken->str_ptr);
        consume(lx_identifier);
        ste_list * formals = new ste_list;
        formals = parseFormalList();
        AST* body = parseBlock(false);
        symbolTable->AddEntry(name, STE_NONE);
        STEntry* entry = symbolTable->FindEntry(name);
        return make_ast_node(4, ast_routine_decl, entry, formals, STE_NONE, body);
    }
    return nullptr;
}

STE_TYPE Parser::parseType() {
    if (currentToken->type == kw_integer || currentToken->type == kw_boolean || currentToken->type == kw_string) {
        if(currentToken->type == kw_integer){
            consume(currentToken->type);
            return STE_INT;
        }
        else if(currentToken->type == kw_boolean){
            consume(currentToken->type);
            return STE_BOOLEAN;
        }
        else if(currentToken->type == kw_string){
            consume(currentToken->type);
            return STE_STRING;
        }
    }
}

ste_list* Parser::parseFormalList() {
    consume(lx_lparen);
    ste_list * list =  parseFormalListTail();
    consume(lx_rparen);
    return list;
}

ste_list * Parser::parseFormalListTail() {
    if (currentToken->type == lx_identifier) {
        symbolTable = symbolTable->enter_scope(symbolTable);
        return parseFormals();
    }
    return nullptr;
}

ste_list* Parser::parseFormals() {
    char * name = new char[64];
    if(currentToken->type==lx_identifier)
        strcpy(name,currentToken->str_ptr);
    consume(lx_identifier);
    consume(lx_colon);
    STE_TYPE type = parseType();
    bool added = symbolTable->AddEntry(name, type);
    if(added == false){
        printf("Line %d : Entry %s Already exist\n",scanner.Fd->getLineNum() + 1, name);
        exit(0);
    }
    STEntry* entry = symbolTable->FindEntry(name);
    ste_list *s = new ste_list;
    s->head = entry;
    s->tail = parseFormalsPrime();
    return s;
}

ste_list* Parser::parseFormalsPrime() {
    if (currentToken->type == lx_comma) {
        consume(lx_comma);
        return parseFormals();
    }
    return nullptr;
}

AST* Parser::parseStmt() {
    if (currentToken->type == kw_while) {
        consume(kw_while);
        AST* predicate = parseExpr();
        consume(kw_do);
        AST* body = parseStmt();
        consume(kw_od);
        return make_ast_node(2, ast_while, predicate, body);
    } else if (currentToken->type == kw_for) {
        consume(kw_for);
        char * name = new char[64];
        if(currentToken->type==lx_identifier)
            strcpy(name,currentToken->str_ptr);
        STEntry* entry = symbolTable->FindEntry(name);
        consume(lx_identifier);
        consume(lx_colon_eq);
        AST* lower_bound = parseExpr();
        consume(kw_to);
        AST* upper_bound = parseExpr();
        consume(kw_do);
        AST* body = parseStmt();
        consume(kw_od);
        return make_ast_node(4, ast_for, entry, lower_bound, upper_bound, body);
    } else if (currentToken->type == kw_read) {
        consume(kw_read);
        consume(lx_lparen);
        char * name = new char[64];
        if(currentToken->type==lx_identifier)
            strcpy(name,currentToken->str_ptr);
        STEntry* entry = symbolTable->FindEntry(name);
        consume(lx_identifier);
        consume(lx_rparen);
        return make_ast_node(1, ast_read, entry);
    } else if (currentToken->type == kw_write) {
        consume(kw_write);
        consume(lx_lparen);
        char * name = new char[64];
        if(currentToken->type==lx_identifier)
            strcpy(name,currentToken->str_ptr);
        STEntry* entry = symbolTable->FindEntry(name);
        consume(lx_identifier);
        consume(lx_rparen);
        return make_ast_node(1, ast_write, entry);
    } else if (currentToken->type == kw_return) {
        consume(kw_return);
        consume(lx_lparen);
        AST* expr = parseExpr();
        consume(lx_rparen);
        return make_ast_node(1, ast_return, expr);
    } else if (currentToken->type == lx_identifier) {
        char * name = new char[64];
        if(currentToken->type==lx_identifier)
            strcpy(name,currentToken->str_ptr);
        STEntry* entry = symbolTable->FindEntry(name);
        consume(lx_identifier);
        return parseIdTail(entry);
    } else if (currentToken->type == kw_if) {
        consume(kw_if);
        AST * predicate = parseExpr();
        consume(kw_then);
        AST * conseq = parseStmt();
        AST * altern = parseIfTail();
        return make_ast_node(3, ast_if, predicate, conseq, altern);
    } else if (currentToken->type == kw_begin) {
        return parseBlock(false);
    }
    return nullptr;
}

AST* Parser::parseIfTail() {
    if (currentToken->type == kw_fi) {
        consume(kw_fi);
        return nullptr;
    } else if (currentToken->type == kw_else) {
        consume(kw_else);
        AST * altern = parseStmt();
        consume(kw_fi);
        return altern;
    }
}

AST* Parser::parseIdTail(STEntry * entry) {
    if (currentToken->type == lx_lparen) {
        ast_list * arg_list = parseArgList();
        return make_ast_node(2, ast_call, entry, arg_list);
    } else if (currentToken->type == lx_colon_eq) {
        consume(lx_colon_eq);
        AST* rhs = parseExpr();
        return make_ast_node(2, ast_assign, entry, rhs);
    }
    return nullptr;
}

AST * Parser::parseBlock(bool flage) {
    consume(kw_begin);
    if(!flage)
        symbolTable = symbolTable->enter_scope(symbolTable);
    ste_list * vars  = parseVarDeclList();
    ast_list * stmts = parseStmtList();
    consume(kw_end);
    symbolTable = symbolTable->exit_scope(symbolTable);
    return make_ast_node(2, ast_block, vars, stmts);
}

ste_list * Parser::parseVarDeclList() {
    ste_list * list = new ste_list;
    STEntry * var = parseVarDecl();
    list->head = var;
    list->tail = parseVarDeclListTail();
    if(list->head == nullptr){
        return nullptr;
    }
    return list;
}

ste_list * Parser::parseVarDeclListTail() {
    if (currentToken->type == lx_semicolon) {
        consume(lx_semicolon);
        return parseVarDeclList();
    }
    return nullptr;
}

STEntry * Parser::parseVarDecl() {
    if (currentToken->type == kw_var){
        consume(kw_var);
        char * name = new char[64];
        if(currentToken->type==lx_identifier)
            strcpy(name,currentToken->str_ptr);
        consume(lx_identifier);
        consume(lx_colon);
        STE_TYPE type = parseType();
        bool added = symbolTable->AddEntry(name, type);
        if(added == false){
            printf("Line %d : Entry %s Already exist\n",scanner.Fd->getLineNum() + 1, name);
            exit(0);
        }
        return symbolTable->FindEntry(name);
    }
    return nullptr;
}

ast_list * Parser::parseStmtList() {
    ast_list * list = new ast_list;
    AST* stmt = parseStmt();
    list->head = stmt;
    list->tail = parseStmtListTail();
    if(stmt == nullptr){
        return nullptr;
    }
    return list;
}

ast_list * Parser::parseStmtListTail() {
    if (currentToken->type == lx_semicolon) {
        consume(lx_semicolon);
        return parseStmtList();
    }
    return nullptr;
}

ast_list * Parser::parseArgList() {
    if(currentToken->type == lx_lparen){
        consume(lx_lparen);
        return parseArgListTail();
    }
    return nullptr;
}

ast_list * Parser::parseArgListTail() {
    if (currentToken->type == lx_rparen) {
        consume(lx_rparen);
        return nullptr;
    } else {
        ast_list * list = parseArgs();
        consume(lx_rparen);
        return list;
    }
}

ast_list * Parser::parseArgs() {
    ast_list * list = new ast_list;
    AST * expr = parseExpr();
    list->head = expr;
    list->tail = parseArgsTail();
    return list;
}

ast_list * Parser::parseArgsTail() {
    if (currentToken->type == lx_comma) {
        consume(lx_comma);
        return parseArgs();
    }
    return nullptr;
}

AST * Parser::parseExpr() {
    AST * larg = parseExpr1();
    return parseExprTail(larg);
}

AST * Parser::parseExprTail(AST * larg) {
    if (currentToken->type == kw_and || currentToken->type == kw_or) {
        AST_type type;
        if(currentToken->type == kw_and){
            type = ast_and;
        } else if (currentToken->type == kw_or){
            type = ast_or;
        }
        consume(currentToken->type);
        AST * rarg = parseExpr1();
        AST * node = make_ast_node(3, type, larg, rarg, type);
        return parseExprTail(node);
    }
    return larg;
}

AST * Parser::parseExpr1() {
    AST * larg = parseExpr2();
    return parseExpr1Tail(larg);
}

AST * Parser::parseExpr1Tail(AST * larg) {
    if (currentToken->type == lx_eq || currentToken->type == lx_neq
        || currentToken->type == lx_lt|| currentToken->type == lx_gt
        || currentToken->type == lx_le|| currentToken->type == lx_ge) {

        LEXEM_TYPE tokenType = currentToken->type;
        AST_type type;
        if(tokenType == lx_eq){
            type = ast_eq;
        } else if (tokenType == lx_neq) {
            type = ast_neq;
        } else if (tokenType == lx_lt) {
            type = ast_lt;
        } else if (tokenType == lx_gt) {
            type = ast_gt;
        } else if (tokenType == lx_le) {
            type = ast_le;
        } else if (tokenType == lx_ge) {
            type = ast_ge;
        }
        consume(currentToken->type);
        AST * rarg = parseExpr2();
        AST * node = make_ast_node(3, type, larg, rarg, type);
        return parseExpr1Tail(node);
    }
    return larg;
}


AST * Parser::parseExpr2() {
    AST * larg = parseExpr3();
    return parseExpr2Tail(larg);
}

AST * Parser::parseExpr2Tail(AST * larg) {
    if (currentToken->type == lx_plus || currentToken->type == lx_minus) {
        AST_type type;
        if(currentToken->type == lx_plus){
            type = ast_plus;
        } else if(currentToken->type == lx_minus){
            type = ast_minus;
        }
        consume(currentToken->type);
        AST * rarg = parseExpr3();
        AST * node = make_ast_node(3, type, larg, rarg, type);
        return parseExpr2Tail(node);
    }
    return larg;
}

AST * Parser::parseExpr3() {
    AST * larg = parseExpr4();
    return parseExpr3Tail(larg);
}

AST * Parser::parseExpr3Tail(AST * larg) {
    if(currentToken->type == lx_star || currentToken->type == lx_slash){
        AST_type type;
        if (currentToken->type == lx_star){
            consume(lx_star);
            type = ast_times;
        }
        else if(currentToken->type == lx_slash) {
            consume(lx_slash);
            type = ast_divide;
        }
        AST * rarg = parseExpr4();
        AST* node = make_ast_node(3, type, larg, rarg, type);
        return parseExpr3Tail(node);
    }
    return larg;
}

AST * Parser::parseExpr4() {
    AST_type type = ast_none;
    if (currentToken->type == lx_minus){
        consume(lx_minus);
        type = ast_uminus;
    } else if (currentToken->type == kw_not) {
        consume(kw_not);
        type = ast_not;
    }
    ast_node * expr = parseExpr5();
    if(type == ast_none){
        return expr;
    }
    return make_ast_node(2, type, expr, type);
}

AST * Parser::parseExpr5() {
    if (currentToken->type == lx_lparen) {
        consume(lx_lparen);
        AST * expr = parseExpr();
        consume(lx_rparen);
        return expr;
    } else {
        return parseValue();
    }
}

AST* Parser::parseValue() {
    if (currentToken->type == lx_identifier) {
        char * name = new char[64];
        if(currentToken->type==lx_identifier)
            strcpy(name,currentToken->str_ptr);
        STEntry* entry = symbolTable->FindEntry(name);
        consume(lx_identifier);
        ast_list * list = parseValueTail();
        if(list == nullptr) {
            return make_ast_node(1, ast_var, entry);
        }
        return make_ast_node(2, ast_call, entry, list);
    } else if (currentToken->type == lx_integer){
        int value = currentToken->value;
        consume(lx_integer);
        return make_ast_node(1, ast_integer, value);
    } else if( currentToken->type == lx_string ){
        char * s = currentToken->str_ptr;
        consume(lx_string);
        return make_ast_node(1, ast_string, s);
    } else if(currentToken->type == lx_float){
        float value = currentToken->float_value;
        consume(lx_float);
        return make_ast_node(1, ast_float, value);
    } else if( currentToken->type == kw_true){
        consume(kw_true);
        return make_ast_node(1, ast_boolean, 1);
    } else if( currentToken->type == kw_false){
        consume(kw_false);
        return make_ast_node(1, ast_boolean, 0);
    }else {
        return nullptr;
    }
}
ast_list * Parser::parseValueTail() {
    return parseArgList();
}