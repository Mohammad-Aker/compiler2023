#include "scanner.h"
#include <cstdlib>
#include <cstring>
#include <ctype.h>

SCANNER::SCANNER()
{
    Fd = nullptr;
}

SCANNER::SCANNER(FileDescriptor* fd){
      Fd = fd; 
}

TOKEN* SCANNER::get_int()
{

    char ch;
    char *str = new char[Fd->getBufferSize()];
    int index = 0;

    bool is_float = false; 
    bool errorFlage = false;
  
    ch = Fd->getChar();
    if (ch == '-')
    {
        str[index++] = ch;
        if(!isdigit(Fd->getChar())){
            Fd->ungetChar(ch);
            TOKEN *tok = new TOKEN;
            tok->str_ptr = nullptr;
            tok->type = lx_minus;
            return tok;
        } else
            Fd->ungetChar(ch);
    }
    else
    {
        Fd->ungetChar(ch);
    }
    while ((ch = Fd->getChar()) != EOF)
    {
        if (isdigit(ch))
        {
            str[index++] = ch;
        }
        else if (ch == '.')
        {
            is_float = true;
            str[index++] = ch;
        }
        else if(ch == ';' || ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r'){
            Fd->ungetChar(ch);
            break;
        }
        else if(isalpha(ch))
        {
            char* error = "Identifier cannot start with a number";
            Fd->ungetChar(ch);
            Fd->reportError(error);
            errorFlage = true;
            break;
        } else{
            Fd->ungetChar(ch);
            break;
        }
    }

    str[index] = '\0';
    TOKEN *tok = new TOKEN;
    LEXEM_TYPE type;
    if(errorFlage){
        tok->type = lx_error;
        tok->str_ptr = nullptr;
        return tok;
    }

    if (is_float)
    {
        if(str[index - 1] == '.'){
            char* error = "Bad floating point number";
            Fd->ungetChar(ch);
            Fd->reportError(error);
            tok->type = lx_error;
            tok->str_ptr = nullptr;
            return tok;
        }
        tok->type = lx_float;
        tok->float_value = atof(str);
    }
    else
    {
        tok->type = lx_integer;
        tok->value = atoi(str);
    }

    delete[] str;
    return tok;
}

void SCANNER::skip_comments()
{
    char ch;
    while ((ch = Fd->getChar()) != EOF)
    {
        if (ch == '#' && Fd->getChar() == '#' || ch == 10)
            break;
    }
}

bool SCANNER::check_keyword(const char *str)
{
    int low = 0;
    int high = keys - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        int cmp = strcmp(keyword[mid], str);
        if (cmp == 0)
            return true;
        if (cmp < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return false;
}

TOKEN* SCANNER::get_id()
{

    char ch;
    char *str = new char[Fd->getBufferSize()];
    int index = 0;
    bool errorFlage = false;
    while ((ch = Fd->getChar()) != EOF)
    {
        if (isalnum(ch) || ch == '_')
        {
            str[index++] = ch;

        }
        else if(ch == ';' || ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r'|| ch == '*' || ch == '/' || ch == '+' || ch == '-'|| ch == ')' || ch == '(' || ch == ':' || ch == ','){
            Fd->ungetChar(ch);
            break;
        }
        else
        {
            char* error = "Illegal character";
            Fd->reportError(error);
            errorFlage = true;
            break;
        }
    }


    str[index] = '\0';  
    TOKEN *tok = new TOKEN;

    if(errorFlage){
        tok->type = lx_error;
        tok->str_ptr = nullptr;
        return tok;
    }

    if (check_keyword(str))
    {
        int keyword_index = -1;
        for (int i = 0; i < keys; i++)
        {
            if (strcmp(keyword[i], str) == 0)
            {
                keyword_index = i;
                break;
            }
        }

        tok->type = key_type[keyword_index];
    }
    else
    {
        tok->type = lx_identifier;
        tok->str_ptr = strdup(str);
    }

    delete[] str;
    return tok;
}

TOKEN* SCANNER::get_string()
{
    
    char ch;
    char *str = new char[Fd->getBufferSize()];
    int index = 0;

    bool escaped = false;

    while ((ch = Fd->getChar()) != EOF)
    {
        if (ch == '"')
        {
            if (!escaped)
                break; 
            else
            {
                str[index++] = ch; 
                escaped = false;
            }
        }
        else if (ch == '\\')
        {
            if (escaped)
            {
                str[index++] = ch;
                escaped = false;
            }
            else
            {
                escaped = true;
            }
        }
        else
        {
            str[index++] = ch;
            escaped = false;
        }
    }

    str[index] = '\0'; 
    TOKEN *tok = new TOKEN;
    tok->type = lx_string;
    tok->str_ptr = strdup(str); 

    delete[] str;
    return tok;
}

TOKEN *SCANNER::Scan()
{
    char ch;
    TOKEN *token = new TOKEN;
    token->str_ptr = nullptr;

    while ((ch = Fd->getChar()) != EOF)
    {
        if (isspace(ch))
        {
            continue;
        }

        if (ch == '#')
        {
            if(Fd->getChar() == '#'){
            Fd->ungetChar(ch);
            skip_comments();
            continue;
            }
        }

        if (isdigit(ch) || ch == '-')
        {
            Fd->ungetChar(ch);
            token = get_int();
            break;
        }

        if (ch == '"')
        {
            token = get_string();
            break;
        }

        if (isalpha(ch) || ch == '_')
        {
            Fd->ungetChar(ch);
            token = get_id();
            break;
        }

        if (ch == ':'){
            if((ch = Fd->getChar()) == '='){
                token->type = lx_colon_eq;
                break;
            } else {
                token->type = lx_colon;
                Fd->ungetChar(ch);
                break;
            }
        }

        if(ch == '!'){
            if((ch = Fd->getChar()) == '='){
                token->type = lx_neq;
                break;
            }
            else {
                Fd->ungetChar(ch);
                break;
            }
        }

        if(ch == '<'){
            if((ch = Fd->getChar()) == '='){
                token->type = lx_le;
                break;
            }
            else {
                token->type = lx_lt;
                break;
            }
        }

        if(ch == '>'){
            if((ch = Fd->getChar()) == '='){
                token->type = lx_ge;
                break;
            }
            else {
                token->type = lx_gt;
                break;
            }
        }

        switch (ch)
        {
            case '(':
                token->type = lx_lparen;
                break;
            case ')':
                token->type = lx_rparen;
                break;
            case '[':
                token->type = lx_lbracket;
                break;
            case ']':
                token->type = lx_rbracket;
                break;
            case '{':
                token->type = lx_lparen;
                break;
            case '}':
                token->type = lx_rparen;
                break;
            case '.':
                token->type = lx_dot;
                break;
            case ';':
                token->type = lx_semicolon;
                break;
            case ',':
                token->type = lx_comma;
                break;
            case '+':
                token->type = lx_plus;
                break;
            case '-':
                token->type = lx_minus;
                break;
            case '*':
                token->type = lx_star;
                break;
            case '/':
                token->type = lx_slash;
                break;
            case '=':
                token->type = lx_eq;
                break;
            case '>':
                token->type = lx_gt;
                break;
            default:
                char* error = "Unrecognized character\0";
                Fd->ungetChar(ch);
                Fd->reportError(error);
                exit(EXIT_FAILURE);
        }
        break;
    }

    if (ch == EOF)
    {
        token->type = lx_eof;
        token->value = 0;
        token->float_value = 0.0;
    }

    return token;
}