#include <stdio.h>      // FILE, SEEK_END, SEEK_SET, fclose(), fread(), fseek(), ftell(), printf()
#include <stdlib.h>     // free(), malloc(), realloc()
#include <ctype.h>      // atoi()
#include <stdbool.h>    // bool, false, true
#include <string.h>     // memset()

size_t lineLength;
size_t fileSize;

bool findRightSidedAdjacentParts(char* buffer, size_t position)
{   
    if ((position + lineLength  < fileSize      &&  !isdigit(buffer[position + lineLength   + 1]) && buffer[position + lineLength   + 1] != '.' && !iscntrl(buffer[position + lineLength +   1])) ||
        (position               < fileSize      &&  !isdigit(buffer[position                + 1]) && buffer[position                + 1] != '.' && !iscntrl(buffer[position +                1])) ||
        (position               > lineLength    &&  !isdigit(buffer[position - lineLength   + 1]) && buffer[position - lineLength   + 1] != '.' && !iscntrl(buffer[position - lineLength +   1])) ||
        (isdigit(buffer[position + 1]) && findRightSidedAdjacentParts(buffer, position + 1)))
    {
        return true;
    }

    return false;
}

int main(int argc, char** argv)
{
    if (argc < 1) { return 1; }
    
    char* fileName = argv[1];
    FILE* inputFile = fopen(fileName, "rb");   

    // Load file into memory
    fseek(inputFile, 0, SEEK_END);
    fileSize = ftell(inputFile) ;
    char* buffer = malloc(fileSize * sizeof(char));
    fseek(inputFile, 0, SEEK_SET);
    fread(buffer, sizeof(char), fileSize, inputFile);
    fclose(inputFile);

    lineLength = strcspn(buffer, "\n") + 1;

    int total = 0;;

    for (size_t i = 0; i < fileSize; i++)
    {
        if (isdigit(buffer[i]))
        {
            if ((i + lineLength < fileSize && (
                    (   !isdigit(buffer[i + lineLength])        && buffer[i + lineLength]       != '.')                                             ||
                    (   !isdigit(buffer[i + lineLength - 1])    && buffer[i + lineLength - 1]   != '.' && !iscntrl(buffer[i + lineLength - 1]))))   ||
                (i > 0 &&    
                        !isdigit(buffer[i - 1])                 && buffer[i - 1]                != '.' && !iscntrl(buffer[i - 1]))                  ||
                (i > lineLength && (
                    (   !isdigit(buffer[i - lineLength - 1])    && buffer[i - lineLength - 1]   != '.' && !iscntrl(buffer[i - lineLength - 1]))     ||
                    (   !isdigit(buffer[i - lineLength])        && buffer[i - lineLength]       != '.')                                             ||
                    (   !isdigit(buffer[i - lineLength+ 1])     && buffer[i - lineLength + 1]   != '.' && !iscntrl(buffer[i - lineLength + 1]))))   ||
                findRightSidedAdjacentParts(buffer, i))
            {
                printf("%d ", atoi(&buffer[i]));
                total += atoi(&buffer[i]);
            }

            i += strspn(&buffer[i], "1234567890");

        }
    }

    free(buffer);
    printf("%d", total);
    return 0;
        
}
    
    
    
