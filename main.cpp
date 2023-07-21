#include <iostream>
#include "fd.h"
#include "scanner.h"

int main()
{
    // Open the input file
    const char *inputFileName = "input.comp2023";
    FileDescriptor fd(inputFileName);

    // Check if the file was opened successfully
    if (!fd.isOpen())
    {
        fd.reportError("Failed to open the input file.");
        return 1;
    }

    // Create a scanner object
    SCANNER scanner(&fd);

    // Scan and print the tokens
    TOKEN *token;
    while ((token = scanner.Scan())->type != lx_eof)
    {
        switch (token->type)
        {
        case lx_identifier:
            std::cout << "Identifier: " << token->str_ptr << std::endl;
            break;
        case lx_integer:
            std::cout << "Integer: " << token->value << std::endl;
            break;
        case lx_string:
            std::cout << "String: " << token->str_ptr << std::endl;
            break;
        case lx_float:
            std::cout << "Float: " << token->float_value << std::endl;
            break;
        // Add cases for other token types if needed
        default:
            std::cout << "Token Type: " << token->type << std::endl;
        }
    }

    // Close the input file
    fd.close();

    return 0;
}