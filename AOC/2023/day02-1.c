#include <stdio.h>      // FILE, SEEK_END, SEEK_SET, fclose(), fread(), fseek(), ftell(), printf()
#include <stdlib.h>     // free(), malloc(), realloc()
#include <ctype.h>      // atoi()
#include <stdbool.h>    // bool, false, true
#include <string.h>     // memset()

#define RED_CUBE_LIMIT 12
#define GREEN_CUBE_LIMIT 13
#define BLUE_CUBE_LIMIT  14
#define MAX_CUBE_LIMIT (RED_CUBE_LIMIT + GREEN_CUBE_LIMIT + BLUE_CUBE_LIMIT)

typedef struct Subset
{
    int redCubes;
    int greenCubes;
    int blueCubes;

} Subset;

typedef struct Game
{
    bool possible;
    int ID;
    int numberOfSubsets;
    Subset* subsets;

} Game;

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

    int numberOfGames = 0;
    Game* games = NULL;

    for (size_t i = 0; i < fileSize; i++)
    {
        switch (buffer[i])
        {
        case 'G':
            games = realloc(games, (++numberOfGames) * sizeof(Game));
            games[numberOfGames - 1].possible = true;
            games[numberOfGames - 1].ID = atoi(&buffer[i += 5]);
            games[numberOfGames - 1].numberOfSubsets = 1;
            games[numberOfGames - 1].subsets = calloc(1, sizeof(Subset));
            i += 3;
            break;
        
        case 'r':
            while (buffer[i - 2] != ' ') { i--; }
            games[numberOfGames - 1].subsets[games[numberOfGames - 1].numberOfSubsets - 1].redCubes = atoi(&buffer[i - 2]);
            i += 3;
            break;
            
        case 'g':
            while (buffer[i - 2] != ' ') { i--; }
            games[numberOfGames - 1].subsets[games[numberOfGames - 1].numberOfSubsets - 1].greenCubes = atoi(&buffer[i - 2]);
            i += 5;
            break;
            
        case 'b':
            while (buffer[i - 2] != ' ') { i--; }
            games[numberOfGames - 1].subsets[games[numberOfGames - 1].numberOfSubsets - 1].blueCubes = atoi(&buffer[i - 2]);
            i += 4;
            break;
            
        case ';':
            games[numberOfGames - 1].subsets = realloc(games[numberOfGames - 1].subsets, ++games[numberOfGames - 1].numberOfSubsets * sizeof(Subset));
            memset(&games[numberOfGames - 1].subsets[games[numberOfGames - 1].numberOfSubsets - 1], 0, sizeof(Subset));
            i++;
            break;

        default:
            break;
        }
    }
    
    int total = 0;

    for (int i = 0; i < numberOfGames; i++)
    {
        for (int j = 0; j < games[i].numberOfSubsets; j++)
        {
            if (games[i].subsets[j].redCubes > RED_CUBE_LIMIT     || 
                games[i].subsets[j].greenCubes > GREEN_CUBE_LIMIT ||
                games[i].subsets[j].blueCubes > BLUE_CUBE_LIMIT   ||
                games[i].subsets[j].redCubes + games[i].subsets[j].greenCubes + games[i].subsets[j].blueCubes > MAX_CUBE_LIMIT)
            {
                games[i].possible = false;
            }
        }

        free(games[i].subsets);

        if (games[i].possible) { total += games[i].ID; }

    }
    
    free(games);
    printf("%d", total);
    return 0;
}