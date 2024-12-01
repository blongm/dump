#include <stdio.h>      // FILE, SEEK_END, SEEK_SET, fclose(), fread(), fseek(), ftell(), printf()
#include <stdlib.h>     // free(), malloc(), realloc()
#include <ctype.h>      // atoi()
#include <stdbool.h>    // bool, false, true
#include <string.h>     // memset()

char* buffer;
char* currentGear;
char* partNumbers[2];

char* findIntegerBeginning(size_t position)
{
    if (isdigit(buffer[position - 1])) return findIntegerBeginning(position - 1); 
    else return &buffer[position];
}

void findPartNumbersForGear(size_t position)
{
    if      (partNumbers[1] == NULL)                                                                                partNumbers[1] = findIntegerBeginning(position); 
    else if (partNumbers[0] == NULL && partNumbers[1] != findIntegerBeginning(position))                            partNumbers[0] = findIntegerBeginning(position);
    else if (partNumbers[0] != findIntegerBeginning(position) && partNumbers[1] != findIntegerBeginning(position))  currentGear = NULL;
}


int main(int argc, char** argv)
{
    if (argc < 1) return 1;
    
    char* fileName = argv[1];
    FILE* inputFile = fopen(fileName, "rb");   

    // Load file into memory
    fseek(inputFile, 0, SEEK_END);
    size_t fileSize = ftell(inputFile) ;
    buffer = malloc(fileSize * sizeof(char));
    fseek(inputFile, 0, SEEK_SET);
    fread(buffer, sizeof(char), fileSize, inputFile);
    fclose(inputFile);

    size_t lineLength = strcspn(buffer, "\n") + 1;

    int total = 0;

    partNumbers[0] = partNumbers[1] = NULL;

    for (size_t i = 0; i < fileSize; i++)
    {
        if (buffer[i] == '*')
        {
            currentGear = &buffer[i];

            if (i < fileSize && isdigit(buffer[i + 1])) partNumbers[0] = &buffer[i + 1]; 

            if (i < fileSize - lineLength   && isdigit(buffer[i + lineLength  + 1 ])) findPartNumbersForGear(i + lineLength   + 1 );
            if (i < fileSize - lineLength   && isdigit(buffer[i + lineLength      ])) findPartNumbersForGear(i + lineLength       );
            if (i < fileSize - lineLength   && isdigit(buffer[i + lineLength  - 1 ])) findPartNumbersForGear(i + lineLength   - 1 );
            if (i > 0                       && isdigit(buffer[i               - 1 ])) findPartNumbersForGear(i +              - 1 );
            if (i > lineLength              && isdigit(buffer[i - lineLength  - 1 ])) findPartNumbersForGear(i - lineLength   - 1 );
            if (i > lineLength              && isdigit(buffer[i - lineLength      ])) findPartNumbersForGear(i - lineLength       );
            if (i > lineLength              && isdigit(buffer[i - lineLength  + 1 ])) findPartNumbersForGear(i - lineLength   + 1 );

            total = currentGear !=  NULL ? total + atoi(partNumbers[0]) * atoi(partNumbers[1]) : total;
            partNumbers[0] = partNumbers[1] = NULL;

        }
    }

    free(buffer);
    printf("%d", total);
    return 0;
        
}
    
    
    
