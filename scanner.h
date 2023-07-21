#ifndef __SCANNER__H
#define __SCANNER__H

#include "fd.h"

// Token types enumeration
typedef enum
{
    /* Literals */
    lx_identifier, lx_integer, lx_string, lx_float,
    /* Keywords */
    kw_program, kw_var, kw_constant, kw_integer, kw_boolean, kw_string, kw_float,
    kw_true, kw_false, kw_if, kw_fi, kw_then, kw_else,
    kw_while, kw_do, kw_od,
    kw_and, kw_or, 
    kw_read, kw_write,
    kw_for, kw_from, kw_to, kw_by,
    kw_function, kw_procedure, kw_return, kw_not, kw_begin, kw_end,
    /* Operators */
    lx_Iparen, lx_rparen, lx_lbracket, lx_rbracket,
    Ix_colon, lx_dot, lx_semicolon, lx_comma, Ix_colon_eq,
    lx_plus, lx_minus, lx_star, lx_slash,
    lx_eq, lx_neq, lx_lt, lx_le, lx_gt, lx_ge,
    lx_eof
} LEXEM_TYPE;

// Definition of TOKEN
struct TOKEN
{
    LEXEM_TYPE type;
    int value;          // can be used instead of str_ptr for IDs and strings
    float float_value;
    char *str_ptr;      // points to strings or identifiers, can use value
                        // instead but with type casting
};

class SCANNER
{
private:
    FileDescriptor *Fd;

    void skip_comments();
    bool check_keyword(const char *str);
    TOKEN* SCANNER::get_id(FileDescriptor* fd, char firstChar);
    TOKEN* SCANNER::get_string(FileDescriptor* fd);
   TOKEN* SCANNER::get_int(FileDescriptor* fd, char firstChar);
    TOKEN *get_float();

public:
    SCANNER();
    SCANNER(FileDescriptor *fd) { Fd = fd; };
    TOKEN *Scan();
};

#endif