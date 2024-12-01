#include <stdio.h>  // SEEK_END, SEEK_SET, File, fclose(), fopen(), fread(), fseek(), printf()
#include <stdlib.h> // atoi(), free(), malloc()
#include <string.h> // strcspn()
#include <ctype.h>  // isdigit(), iscntrl();

typedef struct Card
{
    int ID;
    int countOfWinnningNumbers;
    int countOfScratchedNumbers;
    int* winningNumbers;
    int* scratchedNumbers;
    int matchingNumbers;
    int copies;

} Card;

int main(int argc, char** argv)
{
    if (argc < 1) return 1;
    
    char* fileName = argv[1];
    FILE* inputFile = fopen(fileName, "rb");   

    // Load file into memory
    fseek(inputFile, 0, SEEK_END);
    size_t fileSize = ftell(inputFile) ;
    char* buffer = malloc(fileSize * sizeof(char));
    fseek(inputFile, 0, SEEK_SET);
    fread(buffer, sizeof(char), fileSize, inputFile);
    fclose(inputFile);

    int countOfCards = 0;
    Card* cards = NULL;

    int countOfWinnningNumbers = 0;
    int countOfScratchedNumbers = 0;
    int* winningNumbers = NULL;
    int* scratchedNumbers = NULL;

    int cardPoints = 0;
    int totalPoints = 0;

    for (size_t i = 0; i < fileSize; i++)
    {
        if (buffer[i] == 'C')
        {
            // Add new card to deck
            cards = realloc(cards, ++countOfCards * sizeof(Card));
            // Set info for new card
            i += 5;
            cards[countOfCards - 1].ID = atoi(&buffer[i]);
            cards[countOfCards - 1].copies = 1;
            cards[countOfCards - 1].matchingNumbers = 0;
            // move past ':'
            i += strcspn(&buffer[i], ":");

            // Repeat untill scratched are reached
            while (buffer[i] != '|')
            {
                // if there is a number, add to winning numbers
                if (isdigit(buffer[i]))
                {
                    winningNumbers = realloc(winningNumbers, ++countOfWinnningNumbers * sizeof(int));
                    winningNumbers[countOfWinnningNumbers - 1] = atoi(&buffer[i]);
                    i += strspn(&buffer[i], "12345678890");
                }

                i++;
            }

            // Repeat untill new line is reached
            while (!iscntrl(buffer[i]))
            {
                // if there is a number, add to scratched numbers
                if (isdigit(buffer[i]))
                {
                    scratchedNumbers = realloc(scratchedNumbers, ++countOfScratchedNumbers * sizeof(int));
                    scratchedNumbers[countOfScratchedNumbers - 1] = atoi(&buffer[i]);
                    i += strspn(&buffer[i], "12345678890");
                }   
                
                i++;

            }

            // total up points
            for (int i = 0; i < countOfWinnningNumbers; i++)
            {
                for (int j = 0; j < countOfScratchedNumbers; j++)
                {
                    if (winningNumbers[i] == scratchedNumbers[j])
                    {
                        cards[countOfCards - 1].matchingNumbers++;
                        cardPoints = cardPoints == 0 ? 1 : cardPoints * 2;
                    }
                }      
            }
            
            // move numbers and sizes into struct
            cards[countOfCards - 1].countOfWinnningNumbers = countOfWinnningNumbers;
            cards[countOfCards - 1].countOfScratchedNumbers = countOfScratchedNumbers;
            cards[countOfCards - 1].winningNumbers = malloc(countOfWinnningNumbers * sizeof(int));
            cards[countOfCards - 1].scratchedNumbers = malloc(countOfScratchedNumbers * sizeof(int));
            memcpy(cards[countOfCards - 1].winningNumbers, winningNumbers,countOfWinnningNumbers * sizeof(int));
            memcpy(cards[countOfCards - 1].scratchedNumbers, scratchedNumbers, countOfScratchedNumbers * sizeof(int));

            totalPoints += cardPoints;
            cardPoints = countOfWinnningNumbers = countOfScratchedNumbers = 0;

            free(winningNumbers);
            free(scratchedNumbers);
            winningNumbers = scratchedNumbers = NULL;
        }
    }
    
    free(buffer);

    int totalCards = 0;

    for (int i = 0; i < countOfCards; i++)
    {
        for (int j = 1; j <= cards[i].matchingNumbers; j++)
        {
            if (i + j < countOfCards)
            {
                cards[i + j].copies += cards[i].copies;
            }
        }

        totalCards += cards[i].copies;

    }
    

    printf("Total Points = %d\n", totalPoints);
    printf("Total Scratch Cards = %d", totalCards);

    return 0;
 
}
    