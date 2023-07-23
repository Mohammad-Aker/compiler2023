#include <iostream>
#include "FileDescriptor.cpp"
#include "scanner.cpp"


int main()
{
    // Open the input file
    char inputFileName[] = "C:\\Users\\shadi\\CLionProjects\\compiler\\main";
    FileDescriptor fd(inputFileName);

//    SCANNER s(&fd);

    // Check if the file was opened successfully
     if (!fd.isOpen())
     {
         printf("file is not open correctly");
         return 1;
     }

    // Create a scanner object
    SCANNER scanner(&fd);
     // Scan and print the tokens
    TOKEN *token;
    while ((token = scanner.Scan())->type != lx_eof)
    {
        if(token->type == lx_error){
            break;
        }
        std::cout << "Token Type: "<< TOKEN_NAMES[token->type];
        if(token->type == lx_integer){
            std::cout << ", Token Value: "<< token->value;
        }
        else if(token->type == lx_float){
            std::cout << ", Token Value: "<< token->float_value;
        }
        else if(token->type == lx_identifier || token->type == lx_string){
            std::cout << ", Token Value: "<< token->str_ptr;
        }

        std::cout << std::endl;
    }
//     Close the input file
    fd.close();

    return 0;
}