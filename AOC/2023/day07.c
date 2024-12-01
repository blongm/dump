#include <stdio.h>  
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define HAND_SIZE 5
#define MAX_PRIME 439
#define NUMBER_OF_CARDS 13

typedef char card;

int numberOfHands;

typedef enum typeOfHand
{
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND,
    UNKNOWN
    
} typeOfHand;

typedef card cards[HAND_SIZE];

typedef struct Hand
{
    typeOfHand typeOfHand;
    cards cards;
    int bid;
    int rank;
    int winnings;
    long long primeValue;

} Hand;

typedef Hand* hands;

bool isPrime(int n)
{
    if (n == 2 || n == 3)
        return true;

    if (n <= 1 || n % 2 == 0 || n % 3 == 0)
        return false;

    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }

    return true;
}

int sortHands(const void *a, const void *b)
{
    if ((*(const Hand*)a).typeOfHand == (*(const Hand*)b).typeOfHand)
    {
        for (size_t i = 0; i < HAND_SIZE ; i++)
        {
            if ((*(const Hand*)a).cards[i] != (*(const Hand*)b).cards[i])
            {
                // Covers all digits
                if (isdigit((*(const Hand*)a).cards[i]) || isdigit((*(const Hand*)b).cards[i]))
                    return (*(const Hand*)a).cards[i] - (*(const Hand*)b).cards[i];

                // Covers Aces and Tens which are in the wrong order but still either the biggest or smallest non-number card
                else if (   (*(const Hand*)a).cards[i] == 'A' || (*(const Hand*)b).cards[i] == 'A' || 
                            (*(const Hand*)a).cards[i] == 'T' || (*(const Hand*)b).cards[i] == 'T')
                    return -((*(const Hand*)a).cards[i] - (*(const Hand*)b).cards[i]);

                // Covers Jacks, which are always less the King or Queen
                else if ((*(const Hand*)a).cards[i] == 'J' || (*(const Hand*)b).cards[i] == 'J')
                    return (*(const Hand*)a).cards[i] - (*(const Hand*)b).cards[i];

                // Covers King and Queen which are in the wrong order but are the only two left
                return -((*(const Hand*)a).cards[i] - (*(const Hand*)b).cards[i]);
            } 
        }
        // Hands are duplicates
        return 0;
    }
    // Hands are different
    return (*(const Hand*)a).typeOfHand - (*(const Hand*)b).typeOfHand;
}

int sortHandsWithJokers(const void *a, const void *b)
{
    Hand* HandA = (Hand*)a;
    Hand* HandB = (Hand*)b;

    printf("Comparing %.5s and %.5s\n", HandA->cards, HandB->cards);

    if (HandA -> typeOfHand == HandB -> typeOfHand)
    {
        for (size_t i = 0; i < HAND_SIZE ; i++)
        {
            if (HandA -> cards[i] != HandB -> cards[i])
            {
                // Covers Jokers
                if (HandA -> cards[i] == 'J' || HandB -> cards[i] == 'J')
                    if (HandA -> cards[i] == 'J') return -1;
                    else return 1;

                // Covers all digits
                else if (isdigit(HandA -> cards[i]) || isdigit(HandB -> cards[i]))
                    return HandA -> cards[i] - HandB -> cards[i];

                // Covers Aces and Tens which are in the wrong order but still either the biggest or smallest non-number card
                else if (   HandA -> cards[i] == 'A' || HandB -> cards[i] == 'A' || 
                            HandA -> cards[i] == 'T' || HandB -> cards[i] == 'T')
                    return -(HandA -> cards[i] - HandB -> cards[i]);

                // Covers King and Queen which are in the wrong order but are the only two left
                return -(HandA -> cards[i] - HandB -> cards[i]);
            } 
        }
        // Hands are duplicates
        return 0;
    }
    // Hands are different
    return HandA -> typeOfHand - HandB -> typeOfHand;    
}


int main(int argc, char** argv)
{
    if (argc < 1) return 1;
    
    char* fileName = argv[1];
    FILE* inputFile = fopen(fileName, "rb");   

    // Load file into memory
    fseek(inputFile, 0, SEEK_END);
    size_t fileSize = ftell(inputFile) ;
    unsigned char* fileBuffer = malloc(fileSize * sizeof(char));
    fseek(inputFile, 0, SEEK_SET);
    fread(fileBuffer, sizeof(char), fileSize, inputFile);
    fclose(inputFile);

    // Problem
    numberOfHands = 0;
    // Array of pointers to Hand
    hands set[NUMBER_OF_CARDS] = {NULL};

    // generate prime numbers
    int primes['T' - '2' + 1] = {0};

    for (int i = 0; i <= MAX_PRIME; i++)
    {
        if (isPrime(i))
        {
            for (int j = 0; j <= 'T' - '2'; j++)
            {
                if (primes[j] == 0)
                {
                    primes[j] = i;
                    break;
                }
            }
        }
    }

    char typesOfCards[] = "J23456789TQKA";

    // load hands from file
    Hand* currentHand;

    for (size_t i = 0; i < fileSize; i += strspn((char*)&fileBuffer[i], "1234567890") + 2)
    {
        numberOfHands++;

        for (int j = 0; j < NUMBER_OF_CARDS; j++)
        {
            set[j] = realloc(set[j], numberOfHands * sizeof(Hand));
            currentHand = &set[j][numberOfHands - 1];

            // load cards 
            for (size_t k = 0; k < HAND_SIZE; k++)
            {
                currentHand -> cards[k] = fileBuffer[i + k] == 'J' ? typesOfCards[j] : fileBuffer[i + k];
            }

            // Calulcate prime value
            currentHand -> primeValue = 1;

            for (size_t k = 0; k < HAND_SIZE; k++)
            {
                currentHand -> primeValue *= primes[currentHand -> cards[k] - '2'];
            }

            // Determine Hand size based on prime value
            currentHand -> typeOfHand = UNKNOWN;

            for (size_t k = 0; k < HAND_SIZE; k++)
            {
                
                if (currentHand -> typeOfHand == UNKNOWN 
                && currentHand -> primeValue % (long long)pow(primes[currentHand -> cards[k] - '2'], 5) == 0)
                {
                    currentHand -> typeOfHand = FIVE_OF_A_KIND;
                }

                else if (currentHand -> typeOfHand == UNKNOWN 
                && currentHand -> primeValue % (long long)pow(primes[currentHand -> cards[k] - '2'], 4) == 0)
                {
                    currentHand -> typeOfHand = FOUR_OF_A_KIND;
                }

                else if (currentHand -> typeOfHand == ONE_PAIR
                && currentHand -> primeValue % (long long)pow(primes[currentHand -> cards[k] - '2'], 3) == 0)
                {
                currentHand -> typeOfHand = FULL_HOUSE;
                }
                
                else if (currentHand -> typeOfHand == THREE_OF_A_KIND
                && currentHand -> primeValue % (long long)pow(primes[currentHand -> cards[k] - '2'], 3) != 0
                && currentHand -> primeValue % (long long)pow(primes[currentHand -> cards[k] - '2'], 2) == 0)
                {
                currentHand -> typeOfHand = FULL_HOUSE;
                }

                else if (currentHand -> typeOfHand == TWO_PAIR
                && currentHand -> primeValue % (long long)pow(primes[currentHand -> cards[k] - '2'], 3) == 0)
                {
                currentHand -> typeOfHand = FULL_HOUSE;
                }

                else if (currentHand -> typeOfHand == UNKNOWN
                && currentHand -> primeValue % (long long)pow(primes[currentHand -> cards[k] - '2'], 3) == 0)
                {
                currentHand -> typeOfHand = THREE_OF_A_KIND;
                }

                else if (currentHand -> typeOfHand == ONE_PAIR
                && currentHand -> primeValue % (long long)pow(primes[currentHand -> cards[k] - '2'], 2) == 0)
                {
                    for (size_t l = 0; l < HAND_SIZE; l++)
                    {
                        if (currentHand -> cards[k] != currentHand -> cards[l] &&
                        currentHand -> primeValue % (long long)pow(primes[currentHand -> cards[l] - '2'], 2) == 0)
                        {
                            currentHand -> typeOfHand = TWO_PAIR;
                        }
                    }
                }

                else if (currentHand -> typeOfHand == UNKNOWN 
                && currentHand -> primeValue % (long long)pow(primes[currentHand -> cards[k] - '2'], 2) == 0)
                {
                currentHand -> typeOfHand = ONE_PAIR;
                }
            }

            if (currentHand -> typeOfHand == UNKNOWN)
                currentHand -> typeOfHand = HIGH_CARD;

        }

        // load bid
        i += HAND_SIZE + 1;
        for (int j = 0; j < NUMBER_OF_CARDS; j++)
        {
            currentHand = &set[j][numberOfHands - 1];
            currentHand -> bid = atoi((char*)&fileBuffer[i]);
        }
    }
    
    free(fileBuffer);

    // sort Set
    Hand* normalSet = malloc(numberOfHands * sizeof(Hand));
    memcpy(normalSet, set[0], numberOfHands * sizeof(Hand));
    qsort(normalSet, numberOfHands, sizeof(Hand), sortHands);

    int totalWinnings = 0;

    // Rank 'em John!
    for (int i = 0; i < numberOfHands; i++)
    {
        normalSet[i].rank = i + 1;
        //printf("Current Hand - %.5s Type of Hand - %d\n", set[i].cards, set[i].typeOfHand);
        totalWinnings += normalSet[i].winnings =  normalSet[i].bid * normalSet[i].rank;
        //printf("Winnings - %d * %d = %d\n", set[i].bid, set[i].rank, set[i].winnings);
    }

    

    printf("Total Winnings - %d\n", totalWinnings);

    // With Jokers
    Hand currentSet[NUMBER_OF_CARDS];

    for (int i = 0; i < numberOfHands; i++)
    {
        for (int j = 0; j < NUMBER_OF_CARDS; j++)
        {
            currentSet[j] = set[j][i];
        }

        qsort(currentSet, NUMBER_OF_CARDS, sizeof(Hand), sortHandsWithJokers);
        
        set[0][i].typeOfHand = (currentSet[NUMBER_OF_CARDS - 1]).typeOfHand;
        set[0][i].bid = (currentSet[NUMBER_OF_CARDS - 1]).bid;

    }

   // sort Set
    printf("Sorting with Jokers...\n");
    memcpy(normalSet, set[0], numberOfHands * sizeof(Hand));
    qsort(normalSet, numberOfHands, sizeof(Hand), sortHandsWithJokers);

    totalWinnings = 0;

    // Rank 'em John!
    for (int i = 0; i < numberOfHands; i++)
    {
        normalSet[i].rank = i + 1;
        printf("Current Hand - %.5s Type of Hand - %d\n", normalSet[i].cards, normalSet[i].typeOfHand);
        totalWinnings += normalSet[i].winnings =  normalSet[i].bid * normalSet[i].rank;
        printf("Winnings - %d * %d = %d\n", normalSet[i].bid, normalSet[i].rank, normalSet[i].winnings);
    }


    free(normalSet);
    
    printf("Total Winnings with Jokers - %d\n", totalWinnings);

    return 0;

}