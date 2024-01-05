#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

#define ELEMENT_LENGTH 3

typedef unsigned char* buffer;
typedef unsigned char element[3];
typedef struct Node Node;
typedef struct Node
{
    element thisElement;
    element leftElement;
    Node* pointerToLeftNode;
    element rightEelement;
    Node* pointerToRightNode;
    int instructionIndex;
  

} Node;

size_t bufferSize;
buffer fileBuffer;
int numberOfInstructions;
unsigned char* instructions;
int numberOfNodes;
Node* Nodes;
int instructionIndex;
int numberOfStartElements;
int numberOfEndElements;
int numberOfStartNodes;
int numberOfEndNodes;
int numberOfElementsChecked;
element* elementsChecked;
int numberOfInstructionIndexes;
int* InstructionIndexes;



void loadFileIntoMemory(char* fileName)
{
    FILE* inputFile = fopen(fileName, "rb");   
    fseek(inputFile, 0, SEEK_END);
    bufferSize = ftell(inputFile) ;
    fileBuffer = malloc(bufferSize * sizeof(char));
    fseek(inputFile, 0, SEEK_SET);
    fread(fileBuffer, sizeof(char), bufferSize, inputFile);
    fclose(inputFile);
  
}

void loadElements(Node* Node, buffer fileBuffer)
{
    size_t i = 0;
    memcpy(Node -> thisElement, &fileBuffer[i], ELEMENT_LENGTH);
    i += ELEMENT_LENGTH + 4;
    memcpy(Node -> leftElement, &fileBuffer[i], ELEMENT_LENGTH);
    i += ELEMENT_LENGTH + 2;
    memcpy(Node -> rightEelement, &fileBuffer[i], ELEMENT_LENGTH);
}

int findEndElement(element current, element goal, int stepsTaken)
{
    if (memcmp(current, goal, ELEMENT_LENGTH) == 0) return stepsTaken;
    for (int i = 0; i < numberOfNodes; i++)
    {
        if (memcmp(Nodes[i].thisElement, current, ELEMENT_LENGTH) == 0)
        {
            switch (instructions[instructionIndex])
            {
                case 'L':
                    instructionIndex = instructionIndex != numberOfInstructions - 1 ? instructionIndex + 1 : 0;
                    return findEndElement(Nodes[i].leftElement, goal, ++stepsTaken);

                case 'R':
                    instructionIndex = instructionIndex != numberOfInstructions - 1 ? instructionIndex + 1 : 0;
                    return findEndElement(Nodes[i].rightEelement, goal, ++stepsTaken);
            }
        }
    }

    return -1;
}

bool elementFound(unsigned char* element, int instructionIndex)
{
    if (elementsChecked == NULL) { return false;}

    for (int i = 0; i < numberOfElementsChecked; i++)

        if (memcmp(elementsChecked[i], element, ELEMENT_LENGTH) == 0 && InstructionIndexes[i] == instructionIndex) 
        {
            return true;
        }

    return false;
}

long long  gcd(long long a, long long  b)
{
    if (b != 0)
    {
        return gcd(b, a % b);
    }
    else
    {
        return a;
    }
}

long long lcm(unsigned long long a, unsigned long long b)
{
    return a * (b / gcd(a, b));
}

unsigned long long findEndElements(Node** startNodes, int numberOfStartNodes, unsigned long long stepsTaken)
{   
    Node** currentNodes = malloc(numberOfStartNodes * sizeof(Node*));
    memcpy(currentNodes, startNodes, numberOfStartNodes * sizeof(Node*));
    
    int numberOfSteps = 0;
    unsigned long long* steps = NULL;

    elementsChecked = NULL;
    InstructionIndexes = NULL;

    unsigned long long int* lengths = malloc(numberOfStartNodes * sizeof(unsigned long long));
    unsigned long long answer = 1;
    unsigned long long current_gcd = 0;

    for (int i = 0; i < numberOfStartNodes; i++)
    {
        printf("No. %d\n", i);

        while (currentNodes[i] -> thisElement[2] != 'Z' || !elementFound(currentNodes[i] -> thisElement, instructionIndex))
        {   
            if (currentNodes[i] -> thisElement[2] == 'Z')
            {
                printf("%.3s ends in 'Z'\n", currentNodes[i] -> thisElement);

                elementsChecked = realloc(elementsChecked, ++numberOfElementsChecked * sizeof(element));
                memcpy(elementsChecked[numberOfElementsChecked - 1], currentNodes[i] -> thisElement, ELEMENT_LENGTH);
                
                InstructionIndexes = realloc(InstructionIndexes, ++numberOfInstructionIndexes * sizeof(int));
                InstructionIndexes[numberOfInstructionIndexes - 1] = instructionIndex;

                steps = realloc(steps, ++numberOfSteps * sizeof(unsigned long long));
                steps[numberOfSteps - 1] = stepsTaken;
            }


            switch (instructions[instructionIndex])
            {
                case 'L':
                    currentNodes[i] = currentNodes[i] -> pointerToLeftNode;
                    break;

                case 'R':
                    currentNodes[i] = currentNodes[i] -> pointerToRightNode;
                    break;
            }

            instructionIndex = instructionIndex != numberOfInstructions - 1 ? instructionIndex + 1 : 0;
            stepsTaken++;


        }

        elementsChecked = realloc(elementsChecked, ++numberOfElementsChecked * sizeof(element));
        memcpy(elementsChecked[numberOfElementsChecked - 1], currentNodes[i] -> thisElement, ELEMENT_LENGTH);
        
        InstructionIndexes = realloc(InstructionIndexes, ++numberOfInstructionIndexes * sizeof(int));
        InstructionIndexes[numberOfInstructionIndexes - 1] = instructionIndex;

        steps = realloc(steps, ++numberOfSteps * sizeof(unsigned long long));
        steps[numberOfSteps - 1] = stepsTaken;



        printf("Diff = %lld\n0th = %lld\nValue = %lld\n\n", steps[numberOfSteps - 1] - steps[0], 2 * steps[0] - steps[numberOfSteps - 1],  steps[0]); 
        
        lengths[i] = steps[0];

        current_gcd = gcd(current_gcd, steps[0]);

        
        stepsTaken = 0;
        numberOfSteps = 0;
        numberOfElementsChecked = 0;
        numberOfInstructionIndexes = 0;
        instructionIndex = 0;
        free(steps);
        free(elementsChecked);
        free(InstructionIndexes);

        steps = NULL;
        elementsChecked = NULL;
        InstructionIndexes = NULL;
        
    
    }

    printf("GCD = %lld\n", current_gcd);

    for (int i = 0; i < numberOfStartNodes; i++)
    {
        answer = lcm(answer, lengths[i]);
    }
    

    return answer;

}

int main(int argc, char** argv)
{   
    if (argc < 1) return 1;
    loadFileIntoMemory(argv[1]);

    // Organise data in memory
    instructions = NULL;
    Nodes = NULL;

    // Iterate through memory
    // Load instructions
    for (size_t i = 0; isalpha(fileBuffer[i]); i++)
    {
        instructions = realloc(instructions, ++numberOfInstructions);
        instructions[numberOfInstructions - 1] = fileBuffer[i];
    }

    // Load Nodes
    for (size_t i = numberOfInstructions + 4; i < bufferSize; i = i + 18)
    {
        Nodes = realloc(Nodes, ++numberOfNodes * sizeof(Node));
        loadElements(&Nodes[numberOfNodes - 1], &fileBuffer[i]);
    }       

    free(fileBuffer);

    // Solve Problem
    // Part 1
    element* startElements = malloc(sizeof(element)); 
    memcpy(startElements, "AAA", ELEMENT_LENGTH);
    element* endElements = malloc(sizeof(element)); 
    memcpy(endElements, "ZZZ", ELEMENT_LENGTH);
    instructionIndex = 0;

    unsigned long long stepsTaken = findEndElement(startElements[0], endElements[0], 0);

    free(startElements);
    free(endElements);

    printf("Steps Taken - %lld\n\n", stepsTaken);

    // Part 2
    // Make pointers to Nodes
    for (int i = 0; i < numberOfNodes; i++)
    {
        for (int j = 0; j < numberOfNodes; j++)
        {
            if (memcmp(Nodes[j].thisElement, Nodes[i].leftElement, 3) == 0)

                Nodes[i].pointerToLeftNode = &Nodes[j];

            if (memcmp(Nodes[j].thisElement, Nodes[i].rightEelement, 3) == 0)

                Nodes[i].pointerToRightNode = &Nodes[j];
            
        }
    }

    Node** startNodes = NULL;
    Node** endNodes = NULL;
    for (int i = 0; i < numberOfNodes; i++)
    {
        switch (Nodes[i].thisElement[2])
        {
            case 'A':
                startNodes = realloc(startNodes, ++numberOfStartNodes * sizeof(Node*));
                startNodes[numberOfStartNodes - 1] = &Nodes[i];
                break;
            
            case 'Z':
                endNodes = realloc(endNodes, ++numberOfEndNodes * sizeof(Node*));
                endNodes[numberOfEndNodes - 1] = &Nodes[i];
                break;
        }
    }



    stepsTaken = findEndElements(startNodes, numberOfStartNodes, 0);
    
    printf("Steps Taken by Ghosts - %lld\n", stepsTaken);
    
    return 0;
}