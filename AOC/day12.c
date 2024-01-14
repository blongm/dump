#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

size_t file_size;
unsigned char* file_buffer;

typedef struct group
{
    int group_size;
    unsigned char* group_location;

} group;

typedef struct spring
{
    int length_of_row;
    unsigned char* row;
    int number_of_contiguous_groups;
    group* contiguous_groups;
    int number_of_unknown_locations;
    unsigned char** unknown_locations;
    int arrangements;

    int length_of_row_unfolded;
    unsigned char* row_unfolded;
    int number_of_unknown_locations_unfolded;
    unsigned char** unknown_locations_unfolded;
    int arrangements_unfolded;

} spring;

void load_file_into_memory(char* file_name)
{
    FILE* input_file = fopen(file_name, "rb");   
    fseek(input_file, 0, SEEK_END);
    file_size = ftell(input_file) ;
    file_buffer = malloc(file_size * sizeof(unsigned char));
    fseek(input_file, 0, SEEK_SET);
    fread(file_buffer, sizeof(unsigned char), file_size, input_file);
    fclose(input_file);
  
}

bool validate_groups(spring* spring)
{
    int number_of_contiguous_groups = 0;
    group* contiguous_groups = NULL;
    group* current_group = NULL;

    for (size_t i = strspn((char*)spring -> row, "."); spring -> row[i] == '#' || spring -> row[i] == '.'; i++)
    {
        if (spring -> row[i] == '#')
        {
            number_of_contiguous_groups++;
            contiguous_groups = realloc(contiguous_groups, number_of_contiguous_groups * sizeof(group));
            current_group = &contiguous_groups[number_of_contiguous_groups - 1];
            current_group -> group_location = &spring -> row[i];
            current_group -> group_size = strspn((char*)&spring -> row[i], "#");
            i += current_group -> group_size;
        }
    }
    
    if (number_of_contiguous_groups != spring -> number_of_contiguous_groups)
    {
        free(contiguous_groups);
        return false;
    }
    for (int i = 0; i < number_of_contiguous_groups; i++)
    {
        if (contiguous_groups[i].group_size != spring -> contiguous_groups[i].group_size)
        {
            free(contiguous_groups);
            return false;
        }
    }
    
    free(contiguous_groups);
    return true;
    
} 

void next_permutation(char* permutation, int length_of_permutation)
{
    if (length_of_permutation == 0)
    {
        return;
    }
    
    if (permutation[0] == '.')
    {
        permutation[0] = '#';
    }
    else
    {
        permutation[0] = '.';
        next_permutation(&permutation[1], length_of_permutation - 1);
    }
    
}

int main(int argc, char** argv)
{   
    if (argc < 1) return 1;
    load_file_into_memory(argv[1]);

    int number_of_springs = 0;
    spring* springs = NULL;
    spring* current_spring;
    group* current_group;

    for (size_t i = 0; i < file_size; i++)
    {
        number_of_springs++;
        springs = realloc(springs, number_of_springs * sizeof(spring));
        
        current_spring = &springs[number_of_springs - 1];
        current_spring -> length_of_row = strspn((char*)&file_buffer[i], "#.?");
        current_spring -> row = &file_buffer[i];
        current_spring -> number_of_contiguous_groups = 0;
        current_spring -> contiguous_groups = NULL;

        i += current_spring -> length_of_row + 1;

        while (isdigit(file_buffer[i]))
        {
            current_spring -> number_of_contiguous_groups++;
            current_spring -> contiguous_groups = realloc(current_spring -> contiguous_groups, current_spring -> number_of_contiguous_groups * sizeof(group));

            current_group = &current_spring -> contiguous_groups[current_spring -> number_of_contiguous_groups - 1];

            current_group -> group_size = atoi((char*)&file_buffer[i]);
            current_group -> group_location = NULL;
            
            i += strspn((char*)&file_buffer[i], "1234567890") + 1;
        }
    }

    /*
    for (int i = 0; i < number_of_springs; i++)
    {
        // First character of row
        current_group = &springs[i].contiguous_groups[0];

        if (current_group -> group_location == NULL && springs[i].row[0] == '#')
        {
            memset(&springs[i].row[0], '#', current_group -> group_size);
            springs[i].row[current_group -> group_size] = '.';
            current_group -> group_location = springs[i].row;
        }

        // Last character of row
        current_group = &springs[i].contiguous_groups[springs[i].number_of_contiguous_groups - 1];

        if (current_group -> group_location == NULL && springs[i].row[springs[i].length_of_row - 1] == '#')
        {
            springs[i].row[springs[i].length_of_row - current_group -> group_size - 1] = '.';
            memset(&springs[i].row[springs[i].length_of_row - current_group -> group_size], '#', current_group -> group_size);
        }
        
        // Further optimisations?
        
    }
    */
   
   char* current_permuation = NULL;
   char* last_permutation = NULL;
   

   int sum = 0;

   for (int i = 0; i < number_of_springs; i++)
   {
        springs[i].number_of_unknown_locations = 0;
        springs[i].unknown_locations = NULL;

        // Find out where every unknown location is
        for (int j = 0; j < springs[i].length_of_row; j++)
        {
            if (springs[i].row[j] == '?')
            {
                springs[i].number_of_unknown_locations++;
                springs[i].unknown_locations = realloc(springs[i].unknown_locations, springs[i].number_of_unknown_locations * sizeof(unsigned char*));
                springs[i].unknown_locations[springs[i].number_of_unknown_locations - 1] = &springs[i].row[j];
            }   
        }    

        current_permuation = realloc(current_permuation, springs[i].number_of_unknown_locations);
        memset(current_permuation, '.', springs[i].number_of_unknown_locations);

        last_permutation = realloc(last_permutation, springs[i].number_of_unknown_locations);
        memset(last_permutation, '#', springs[i].number_of_unknown_locations);


        for (int j = 0; j < springs[i].number_of_unknown_locations; j++)
        {
            *springs[i].unknown_locations[j] = current_permuation[j];
        }

        springs[i].arrangements = 0;

        while (memcmp(current_permuation, last_permutation, springs[i].number_of_unknown_locations) != 0)
        {
            for (int j = 0; j < springs[i].number_of_unknown_locations; j++)
            {
                *springs[i].unknown_locations[j] = current_permuation[j];
            }
            

            if (validate_groups(&springs[i]))
            {
                springs[i].arrangements++;
            }

            next_permutation(current_permuation, springs[i].number_of_unknown_locations);
        }

        for (int j = 0; j < springs[i].number_of_unknown_locations; j++)
        {
            *springs[i].unknown_locations[j] = current_permuation[j];
        }
            
        if (validate_groups(&springs[i]))
        {
            springs[i].arrangements++;
        }      
        
        sum += springs[i].arrangements;

    }

    free(current_permuation);
    free(last_permutation);
   

    for (int i = 0; i < number_of_springs; i++)
    {
        free(springs[i].contiguous_groups);
        free(springs[i].unknown_locations);
        
    }
    
    free(springs);

    free(file_buffer);

    printf("Sum of counts = %d\n", sum);


    return 0;
}