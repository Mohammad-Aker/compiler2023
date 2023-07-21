#include "scanner.h"
#include <cstdlib>
#include <cstring>
#include <cctype> 


SCANNER::SCANNER()
{
    Fd = nullptr;
}

TOKEN *SCANNER::Scan()
{
    char ch;
    TOKEN *tok = new TOKEN;
    tok->str_ptr = nullptr;

    while ((ch = Fd->getChar()) != EOF)
    {
        // Handle whitespace characters
        if (isspace(ch))
        {
            // Skip whitespace characters
            continue;
        }

        // Handle comments
        if (ch == '#')
        {
            Fd->ungetChar(ch); // Put back '#' for skip_comments() to handle
            skip_comments();
            continue;
        }

        // Handle integers and floats
        if (isdigit(ch) || ch == '-')
        {
            Fd->ungetChar(ch); // Put back the character for get_int() to handle
            tok = get_int();
            break;
        }

        // Handle strings
        if (ch == '"')
        {
            Fd->ungetChar(ch); // Put back the double quote for get_string() to handle
            tok = get_string();
            break;
        }

        // Handle identifiers and keywords
        if (isalpha(ch) || ch == '_')
        {
            Fd->ungetChar(ch); // Put back the character for get_id() to handle
            tok = get_id();
            break;
        }

        // Handle special characters and operators
        switch (ch)
        {
        case '(':
            tok->type = lx_Iparen;
            break;
        case ')':
            tok->type = lx_rparen;
            break;
        // Add handling for other special characters and operators here
        default:
            // Handle unrecognized characters as an error
            Fd->reportError("Unrecognized character");
            exit(EXIT_FAILURE);
        }
        break;
    }

    // Return EOF token if the end of file is reached
    if (ch == EOF)
    {
        tok->type = lx_eof;
        tok->value = 0;
        tok->float_value = 0.0;
    }

    return tok;
}

void SCANNER::skip_comments()
{

    char ch;
    while ((ch = Fd->getChar()) != EOF)
    {
        if (ch == '#' && Fd->getChar() == '#')
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

TOKEN* SCANNER::get_id(FileDescriptor* fd, char firstChar)
{

    char ch;
    char *str = new char[Fd->bufSize];
    int index = 0;

    // Read characters until an invalid character is found
    while ((ch = Fd->getChar()) != EOF)
    {
        if (isalnum(ch) || ch == '_')
        {
            str[index++] = ch;
        }
        else
        {
            Fd->ungetChar(ch); // Put back the invalid character
            break;
        }
    }

    str[index] = '\0';  
    TOKEN *tok = new TOKEN;

   
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
        // If it's an identifier, set the type to lx_identifier and store the string
        tok->type = lx_identifier;
        tok->str_ptr = strdup(str); // strdup() duplicates the string, so we don't have to worry about deallocating it here
    }

    delete[] str; // Free the memory allocated for the string
    return tok;
}

TOKEN* SCANNER::get_string(FileDescriptor* fd) {

{
    
    char ch;
    char *str = new char[Fd->bufSize];
    int index = 0;

    bool escaped = false; // To handle escape sequences

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
            // Handle escape sequences
            if (escaped)
            {
                // If the previous character was also an escape character, add it to the string
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

    delete[] str; // Free the memory allocated for the string
    return tok;
}

TOKEN* SCANNER::get_int(FileDescriptor* fd, char firstChar)
{

    char ch;
    char *str = new char[Fd->bufSize];
    int index = 0;

    bool is_float = false; 
    bool is_negative = false;

  
    ch = Fd->getChar();
    if (ch == '-')
    {
        is_negative = true;
        str[index++] = ch;
    }
    else
    {
        Fd->ungetChar(ch); // Put back the character if it's not a negative sign
    }

    // Read digits until an invalid character is found
    while ((ch = Fd->getChar()) != EOF)
    {
        if (isdigit(ch))
        {
            str[index++] = ch;
        }
        else if (ch == '.')
        {
            // If a dot is found, it's a float, not an integer
            is_float = true;
            str[index++] = ch;
        }
        else
        {
            Fd->ungetChar(ch); // Put back the invalid character
            break;
        }
    }

    str[index] = '\0'; // Null-terminate the string
    TOKEN *tok = new TOKEN;

    if (is_float)
    {
        // If it's a float, set the type to lx_float and convert the string to float
        tok->type = lx_float;
        tok->float_value = is_negative ? -atof(str) : atof(str);
    }
    else
    {
        // If it's an integer, set the type to lx_integer and convert the string to integer
        tok->type = lx_integer;
        tok->value = is_negative ? -atoi(str) : atoi(str);
    }

    delete[] str; // Free the memory allocated for the string
    return tok;
}