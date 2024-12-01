#include <stdio.h>  
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ROW_HEADER 9

typedef long millisecond;
typedef long long millimeter;
typedef long long millimetersPerMillisecond;

typedef struct Option
{
    millisecond timeHeld;
    millimetersPerMillisecond speed;
    millisecond timeLeft;
    millimeter distanceTravelled;

} Option;

typedef struct Race
{
    millisecond timeLimit;
    millimeter distanceRecord;

    int numberOfOptions;
    Option* options;
    
    int numberOfWinningOptions;
    Option* winningOptions;

} Race;


int main(int argc, char** argv)
{
    if (argc < 1) return 1;
    
    char* fileName = argv[1];
    FILE* inputFile = fopen(fileName, "r");   

    // Load file into memory
    fseek(inputFile, 0, SEEK_END);
    size_t fileSize = ftell(inputFile) ;
    char* fileBuffer = malloc(fileSize * sizeof(char));
    fseek(inputFile, 0, SEEK_SET);
    fread(fileBuffer, sizeof(char), fileSize, inputFile);
    fclose(inputFile);

    int numberOfRaces = 0;
    Race* races = NULL;

    Race* currentRace;

    int total = 1;

    size_t i = ROW_HEADER;
    while (!isalpha(fileBuffer[i]))
    {
        if (isdigit(fileBuffer[i]))
        {
            races = realloc(races, ++numberOfRaces * sizeof(Race));
            currentRace = &races[numberOfRaces - 1];
            (*currentRace).timeLimit = atoi(&fileBuffer[i]);
            (*currentRace).numberOfOptions = 0;
            (*currentRace).options = NULL;
            (*currentRace).numberOfWinningOptions = 0;
            (*currentRace).winningOptions = NULL;

            i += strspn(&fileBuffer[i], "1234567890");
        }

        i++;
    }

    numberOfRaces = 0;
    i += ROW_HEADER + 2;

    while (i < fileSize)
    {
        if (isdigit(fileBuffer[i]))
        {
            currentRace = &races[++numberOfRaces - 1];
            (*currentRace).distanceRecord = atoll(&fileBuffer[i]);

            for (int i = 0; i <= (*currentRace).timeLimit; i++)
            {
                if (i * ((*currentRace).timeLimit - i) > (*currentRace).distanceRecord)
                {
                    (*currentRace).numberOfWinningOptions++;
                }
            }

            total *= (*currentRace).numberOfWinningOptions;
            
            i += strspn(&fileBuffer[i], "1234567890");        
        }

        i++;
    }

    size_t lengthOfBuffer = 0;
    char * buffer = NULL;
    
    for (i = ROW_HEADER; !isalpha(fileBuffer[i]); i++)
    {
        if (isdigit(fileBuffer[i]))
        {
            buffer = realloc(buffer, ++lengthOfBuffer);
            buffer[lengthOfBuffer - 1] = fileBuffer[i];
        }
    }    

    buffer = realloc(buffer, ++lengthOfBuffer);
    buffer[lengthOfBuffer - 1] = '\n';   

    races[0].timeLimit = atoi(buffer);
    races[0].numberOfOptions = 0;
    races[0].numberOfWinningOptions = 0;

    lengthOfBuffer = 0;

    for (i += ROW_HEADER + 2; i < fileSize; i++)
    {
        if (isdigit(fileBuffer[i]))
        {
            buffer = realloc(buffer, ++lengthOfBuffer);
            buffer[lengthOfBuffer - 1] = fileBuffer[i];
        }
    }

    free(fileBuffer);
    
    buffer = realloc(buffer, ++lengthOfBuffer);
    buffer[lengthOfBuffer - 1] = '\n';   

    races[0].distanceRecord = atoll(buffer);
    
    free(buffer);

    currentRace = races;

    for (long long i = 0; i <= races[0].timeLimit; i++)
    {

        if ((races[0].timeLimit - i) * i > races[0].distanceRecord)
        {
            races[0].numberOfWinningOptions++;
        }
    }
    

    for (int i = 0; i < numberOfRaces; i++)
    {
        free(races[i].options);
        free(races[i].winningOptions);
    }
    
    printf("Total for races - %d\n", total);
    printf("Total for longer race - %d", races[0].numberOfWinningOptions);

    free(races);

    return 0;
 
}
    