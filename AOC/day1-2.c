#include <stdio.h>  // FILE, SEEK_END, SEEK_SET, fclose(), fread(), fseek(), ftell(), printf()
#include <stdlib.h> // free(), malloc()
#include <ctype.h>  // atoi(), isdigit()
#include <string.h> // memset()

void setDigit(char* numberString, char value)
{
    numberString[1] = value;

    if (numberString[0] == '\0')
    {
        numberString[0] = value;
    }   
}

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
            setDigit(numberString, buffer[i]);
        }

        else if (buffer[i] == '\n')
        {
            total += atoi(numberString);
            memset(numberString, '\0', sizeof(numberString));
        }
        
        else if (&buffer[i] == strstr(&buffer[i], "one"))   { setDigit(numberString, '1'); }
        else if (&buffer[i] == strstr(&buffer[i], "two"))   { setDigit(numberString, '2'); }
        else if (&buffer[i] == strstr(&buffer[i], "three")) { setDigit(numberString, '3'); }
        else if (&buffer[i] == strstr(&buffer[i], "four"))  { setDigit(numberString, '4'); }
        else if (&buffer[i] == strstr(&buffer[i], "five"))  { setDigit(numberString, '5'); }
        else if (&buffer[i] == strstr(&buffer[i], "six"))   { setDigit(numberString, '6'); }
        else if (&buffer[i] == strstr(&buffer[i], "seven")) { setDigit(numberString, '7'); }
        else if (&buffer[i] == strstr(&buffer[i], "eight")) { setDigit(numberString, '8'); }
        else if (&buffer[i] == strstr(&buffer[i], "nine"))  { setDigit(numberString, '9'); }
        
    }
    
    total += atoi(numberString);

    free(buffer);
    printf("%d", total);
    return 0;
}