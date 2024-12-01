#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUMBER_OF_MAPS 7

typedef struct Mapping
{
    unsigned long destinationRangeStart;
    unsigned long sourceRangeStart;
    unsigned long rangeLength;

} Mapping;

typedef struct Map
{
    char* destination;
    char* source;
    int  numberOfMappings;
    Mapping* mappings;

} Map;


unsigned long map(unsigned long input, Map Map)
{
    for (int i = 0; i < Map.numberOfMappings; i++)
    {
        if (input >= Map.mappings[i].sourceRangeStart && input <= Map.mappings[i].sourceRangeStart + Map.mappings[i].rangeLength)
        {
            //printf(" at line %d\nInput %ld\nSource %ld -> %ld\nDest %ld -> %ld\nReturn Value %ld\n", i + 1, input, Map.mappings[i].sourceRangeStart, Map.mappings[i].sourceRangeStart + Map.mappings[i].rangeLength, Map.mappings[i].destinationRangeStart, Map.mappings[i].destinationRangeStart + Map.mappings[i].rangeLength , input - Map.mappings[i].sourceRangeStart + Map.mappings[i].destinationRangeStart);
            return input - Map.mappings[i].sourceRangeStart + Map.mappings[i].destinationRangeStart;
        }
    }
    //printf("\nNo mapping\nInput %ld\nReturn Value %ld\n", input, input);
    return input;
}

unsigned long findLocation(unsigned long seed, Map* maps)
{
    char* source = "seed";

    for (int i = 0; i < NUMBER_OF_MAPS; i++)
    {
        if (strcmp(maps[i].source, source) == 0)
        {
            //printf("\nMapping %ld through %s -> %s", seed, maps[i].source, maps[i].destination);
            seed = map(seed, maps[i]);
            source = maps[i].destination;
            i = -1;
        }
    }

    return seed;
    
}

int main(int argc, char** argv)
{
    if (argc < 1) return 1;
    
    char* fileName = argv[1];
    FILE* inputFile = fopen(fileName, "r");   

    // Load file into memory
    fseek(inputFile, 0, SEEK_END);
    size_t fileSize = ftell(inputFile) ;
    char* buffer = malloc(fileSize * sizeof(char));
    fseek(inputFile, 0, SEEK_SET);
    fread(buffer, sizeof(char), fileSize, inputFile);
    fclose(inputFile);

    // get seeds
    int numberOfSeeds = 0;
    unsigned long* seeds = NULL;

    for (size_t i = 0; buffer[i] != '\n'; i++)
    {
        if (isdigit(buffer[i]))
        {
            seeds = realloc(seeds, ++numberOfSeeds * sizeof(unsigned long));
            seeds[numberOfSeeds - 1] = atoll(&buffer[i]);
            i += strspn(&buffer[i], "1234567890");
        }
    }

    // get all seeds
    int numberOfSeedRanges = 0;
    unsigned long* seedRangeStarts = NULL;
    unsigned long* seedRangeLengths = NULL;
    
    for (size_t i = 0; buffer[i] != '\n'; i++)
    {
        if (isdigit(buffer[i]))
        {
            // inital seed
            seedRangeStarts = realloc(seedRangeStarts, ++numberOfSeedRanges * sizeof(unsigned long));
            seedRangeLengths = realloc(seedRangeLengths, numberOfSeedRanges * sizeof(unsigned long));
            seedRangeStarts[numberOfSeedRanges - 1] = atoll(&buffer[i]);
            i += strspn(&buffer[i], "1234567890") + 1;

            // range
            seedRangeLengths[numberOfSeedRanges - 1] = atoll(&buffer[i]);
            i += strspn(&buffer[i], "1234567890");

        }
    }

    // get maps;
    Map maps[NUMBER_OF_MAPS] = {0};

    size_t i = strcspn(buffer, "\n");
    int map = -1;
    while (i < fileSize)
    {
        if (isalpha(buffer[i]) && map < NUMBER_OF_MAPS - 1)
        {
            map++;
            // read source
            printf("At map %d, ", map);
            printf("Reallocating 0x%p to size %lld\n", maps[map].source, strcspn(&buffer[i], "-") + 1);
            maps[map].source = realloc(maps[map].source, strcspn(&buffer[i], "-") + 1);
            strncpy(maps[map].source, &buffer[i], strcspn(&buffer[i], "-"));
            maps[map].source[strcspn(&buffer[i], "-")] = '\0';
            
            // skip the -to-
            i += strcspn(&buffer[i], "-") + 4;

            //read destination
            maps[map].destination = realloc(maps[map].destination, strcspn(&buffer[i], " ") + 1);
            strncpy(maps[map].destination, &buffer[i], strcspn(&buffer[i], " "));
            maps[map].destination[strcspn(&buffer[i], " ")] = '\0';

            i += strcspn(&buffer[i], ":");

        }

        while (isdigit(buffer[i]) && !iscntrl(buffer[i]))
        {
            maps[map].numberOfMappings++;
            maps[map].mappings = realloc(maps[map].mappings, maps[map].numberOfMappings * sizeof(Mapping));

            maps[map].mappings[maps[map].numberOfMappings - 1].destinationRangeStart = atoll(&buffer[i]);
            i += strspn(&buffer[i], "1234567890") + 1;

            maps[map].mappings[maps[map].numberOfMappings - 1].sourceRangeStart = atoll(&buffer[i]);
            i += strspn(&buffer[i], "1234567890") + 1;

            maps[map].mappings[maps[map].numberOfMappings - 1].rangeLength = atoll(&buffer[i]);
            i += strspn(&buffer[i], "1234567890");
        }

        i++;

    }
    
    free(buffer);

    unsigned long lowestLocation = ULONG_MAX;

    for (int i = 0; i < numberOfSeeds; i++)
    {
        if (findLocation(seeds[i], maps) < lowestLocation)
        {
            lowestLocation = findLocation(seeds[i], maps);
            printf("\nCurrent Lowest Location is %lu\n", lowestLocation);
        }
        
    }

    unsigned long lowestLocationForAllSeeds = ULONG_MAX;
    for (int i = 0; i < numberOfSeedRanges; i++)
    {
        for (unsigned long j = 0; seedRangeStarts[i] + j < seedRangeStarts[i] + seedRangeLengths[i] - 1; j++)
        {
            //printf("Seed %lu\n", seedRangeStarts[i] + j);
            if (findLocation(seedRangeStarts[i] + j, maps) < lowestLocationForAllSeeds)
                {
                    lowestLocationForAllSeeds = findLocation(seedRangeStarts[i] + j, maps);
                    printf("Current Lowest Location is %lu\n\n", lowestLocationForAllSeeds);
                }
        }
    }    

    // free memory
    free(seeds);
    free(seedRangeStarts);
    free(seedRangeLengths);
    for (int i = 0; i < NUMBER_OF_MAPS; i++)
    {
        free(maps[i].source);
        free(maps[i].destination);
        free(maps[i].mappings);
    }
    

    printf("\nLowest Location is %ld", lowestLocation);
    printf("\nLowest Location for all seeds is %ld\n", lowestLocationForAllSeeds);

    return 0;
 
}
    