#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

} spring;

void load_file_into_memory(char* file_name)
{
    FILE* input_file = fopen(file_name, "rb");   
    fseek(input_file, 0, SEEK_END);
    file_size = ftell(input_file) ;
    file_buffer = malloc(file_size * sizeof(char));
    fseek(input_file, 0, SEEK_SET);
    fread(file_buffer, sizeof(char), file_size, input_file);
    fclose(input_file);
  
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

            current_group -> group_size = atoi((char*)file_buffer[i]);
            current_group -> group_location = NULL;
            
            i += strcspn((char*)file_buffer[i], ",") + 1;
        }
    }

    for (int i = 0; i < number_of_springs; i++)
    {
        for (int j = 0; j < springs[i].number_of_contiguous_groups; j++)
        {
            if (springs[i].contiguous_groups[j].group_location == NULL)
            {
                if ()
                {
                    /* code */
                }
                
            }
            
        }
        
        
    }

    for (int i = 0; i < number_of_springs; i++)
    {
        free(springs[i].contiguous_groups);
        
    }
    
    free(springs);

    free(file_buffer);

    printf("Hello, World!");

    return 0;
}