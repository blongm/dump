#include <stdio.h>  // FILE, SEEK_END, SEEK_SET, fclose(), fread(), fseek(), ftell(), printf()
#include <stdlib.h> // free(), malloc()
#include <ctype.h>  // atoi(), isdigit()
#include <string.h> // memset()

int main(int argc, char** argv)
{
    if (argc < 1) { return 1; }
    
    char* fileName = argv[1];
    FILE* inputFile = fopen(fileName, "r");   

    // Load file into memory
    fseek(inputFile, 0, SEEK_END);
    size_t fileSize = ftell(inputFile);
    char* buffer = malloc(fileSize * sizeof(char));
    fseek(inputFile, 0, SEEK_SET);
    fread(buffer, sizeof(char), fileSize, inputFile);
    fclose(inputFile);

    // Empty string for the 2 character digit
    char numberString[2] = "\0\0";
    int total = 0;

    for (size_t i = 0; i < fileSize; i++)
    {
        if (isdigit(buffer[i]))
        {   
            numberString[1] = buffer[i];

            if (numberString[0] == '\0')
            {
                numberString[0] = buffer[i];
            }   
        }

        else if (buffer[i] == '\n')
        {
            total += atoi(numberString);
            memset(numberString, '\0', sizeof(numberString));
        }
    }
    
    total += atoi(numberString);

    free(buffer);
    printf("%d", total);
    return 0;
}