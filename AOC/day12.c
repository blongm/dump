#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

size_t file_size;
unsigned char* file_buffer;

unsigned char* current_permutation;
unsigned char* last_permutation;

typedef struct group
{
    unsigned int group_size;
    unsigned char* group_location;

} group;

typedef struct spring
{
    size_t length_of_row;
    unsigned char* row;

    unsigned int number_of_contiguous_groups;
    group* contiguous_groups;

    int number_of_unknown_locations;
    unsigned char** unknown_locations;

    unsigned int number_of_known_operational_locations;
    unsigned char** known_operational_locations;

    int number_of_known_operational_and_unknown_locations;
    unsigned char** known_operational_and_unknown_locations;

    int arrangements;

    size_t length_of_row_unfolded;
    unsigned char* row_unfolded;
    
    int total_of_group_sizes;

    unsigned int max_group_size;
    

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

    // Priorotise exiting here as we can skip large chunks
    // Check  if operational groups aren't matching
    // But try and move blocks to the end
    for (size_t i = 0; i < spring -> number_of_known_operational_locations; i++)
    {
        if (*spring->known_operational_locations[i] != '#')
        {
            int number_of_known_operational_locations_to_be_moved = 0;

            for (unsigned char* j = spring -> known_operational_locations[i]; *j == '#' || *j == '.'; j++)
            {
                if (*j == '#')
                {
                    number_of_known_operational_locations_to_be_moved++;
                }
            }
        
            for (size_t j = i; spring -> known_operational_locations[j] == '#' || spring -> known_operational_locations[j] == '.'; j++)
                spring -> known_operational_locations[j] = j > spring -> known_operational_and_unknown_locations[spring -> number_of_known_operational_and_unknown_locations] - number_of_known_operational_locations_to_be_moved ? '#' : '.';
                
            return false;
        }
        
    }

    unsigned int number_of_contiguous_groups = 0;
    unsigned int* contiguous_groups = malloc(spring -> number_of_contiguous_groups * sizeof(unsigned int));
    
    // Calcualte grouping for this arrangement
    for (size_t i = strspn((char*)spring -> row, "."); spring -> row[i] == '#' || spring -> row[i] == '.'; i++)
    {
        // Detected start of grouping
        if (spring -> row[i] == '#')
        {
            // Update list of groups
            number_of_contiguous_groups++;

            //  Update group
            contiguous_groups[number_of_contiguous_groups - 1] = strspn((char*)&spring -> row[i], "#");
            i += contiguous_groups[number_of_contiguous_groups - 1];
    
        }
    }



      // First check if amount of groups is good
    if (number_of_contiguous_groups != spring -> number_of_contiguous_groups)
    {
//        free(contiguous_groups);
        return false;
    }


    // Then check if they match
    for (unsigned int i = 0; i < number_of_contiguous_groups; i++)
    {
        if (contiguous_groups[i] != spring -> contiguous_groups[i].group_size)
        {
            free(contiguous_groups);
            return false;
        }
    }
    
    // Validation passes so we return true;
    free(contiguous_groups);
    return true;
    
} 

void next_permutation(unsigned char* permutation, int length_of_permutation, group* groups, int number_of_groups)
{   
    if (memcmp(current_permutation, last_permutation, length_of_permutation) == 0)
    {
        return;
    }
    
    // Have we reached the end of the row?
    if (permutation[length_of_permutation - 1] == '#')
    {
        // Seek back to start of end block
        for (size_t i = length_of_permutation - 1; ; i--)
        {
            if (permutation[i] == '#' && permutation[i - 1] == '.')
            {
                // Seek back to end of penultimate block
                for (size_t j = i - 2; ; j--)
                {
                    if (permutation[j] == '#')
                    {
                        // Work out what group this is part of
                        int group_id = number_of_groups - 1;
                        size_t last_block_size = 0;

                        while (last_block_size < strspn((char*)&permutation[i], "#"))
                        {
                            last_block_size += groups[group_id].group_size;
                            group_id--;
                        }

                        // Move ENTIRE block right ->
                        permutation[j + 1 - groups[group_id].group_size] = '.';
                        permutation[j + 1] = '#';

                        // Move last block onto previous one
                        memset(&permutation[j + 2], '.', length_of_permutation - j - 2);
                        memset(&permutation[j + 2], '#', last_block_size);

                        return;
                    }
                }
            }
        }
    }
    else
    {
        for (size_t i = length_of_permutation - 1; ; i--)
        {
            if (permutation[i - 1] == '#')
            {
                permutation[i] = '#';
                permutation[i - groups[number_of_groups - 1].group_size] = '.';
            
                return;
            }
        }
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
        current_spring -> max_group_size = 0;

        i += current_spring -> length_of_row + 1;

        while (isdigit(file_buffer[i]))
        {
            current_spring -> number_of_contiguous_groups++;
            current_spring -> contiguous_groups = realloc(current_spring -> contiguous_groups, current_spring -> number_of_contiguous_groups * sizeof(group));

            current_group = &current_spring -> contiguous_groups[current_spring -> number_of_contiguous_groups - 1];

            current_group -> group_size = atoi((char*)&file_buffer[i]);
            current_group -> group_location = NULL;
            if (current_group -> group_size > current_spring -> max_group_size)
            {
                current_spring -> max_group_size = current_group -> group_size;
            }
            
            i += strspn((char*)&file_buffer[i], "1234567890") + 1;
        }
    }
   

   

   int sum = 0;

   for (int i = 0; i < number_of_springs; i++)
   {
        printf("Current Spring: %d\n", i + 1);
        printf("%.*s\n", (int)strcspn((char*)springs[i].row, "\r"),springs[i].row);

        springs[i].number_of_unknown_locations = 0;
        springs[i].unknown_locations = NULL;
        springs[i].number_of_known_operational_locations = 0;
        springs[i].known_operational_locations = NULL;
        springs[i].total_of_group_sizes  = 0;
        

        // Get all ??? and ###
        springs[i].unknown_locations = NULL;
        springs[i].known_operational_and_unknown_locations = NULL;
        springs[i].number_of_known_operational_and_unknown_locations = 0;

        for (size_t j = 0; j < springs[i].length_of_row; j++)
        {
            if (springs[i].row[j] == '?')
            {
                springs[i].number_of_unknown_locations++;
                springs[i].unknown_locations = realloc(springs[i].unknown_locations, springs[i].number_of_unknown_locations * sizeof(unsigned char*));
                springs[i].unknown_locations[springs[i].number_of_unknown_locations - 1] = &springs[i].row[j];
            }
            if (springs[i].row[j] != '.')
            {
                springs[i].number_of_known_operational_and_unknown_locations++;
                springs[i].known_operational_and_unknown_locations = realloc(springs[i].known_operational_and_unknown_locations, springs[i].number_of_known_operational_and_unknown_locations * sizeof(unsigned char*));
                springs[i].known_operational_and_unknown_locations[springs[i].number_of_known_operational_and_unknown_locations - 1] = &springs[i].row[j];
                
                if (springs[i].row[j] == '#')
                {
                    springs[i].number_of_known_operational_locations++;
                    springs[i].known_operational_locations = realloc(springs[i].known_operational_locations, springs[i].number_of_known_operational_locations * sizeof(unsigned char*));
                    springs[i].known_operational_locations[springs[i].number_of_known_operational_locations - 1] = &springs[i].row[j];
                }
                
            }
            
        }    

        for (unsigned int j = 0; j < springs[i].number_of_contiguous_groups; j++)
        {
            springs[i].total_of_group_sizes += springs[i].contiguous_groups[j].group_size;
        }
        
        springs[i].number_of_known_operational_locations = 0;

        for (size_t j = 0; j < springs[i].length_of_row; j++)
        {
            if (springs[i].row[j] == '#')
            {
                springs[i].number_of_known_operational_locations++;
            }
            
        }


        current_permutation = realloc(current_permutation, springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations + 1);
        memset(current_permutation, '.', springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations);
        memset(current_permutation, '#', springs[i].total_of_group_sizes);

        last_permutation = realloc(last_permutation, springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations + 1);
        memset(last_permutation, '#', springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations);
        memset(last_permutation, '.', springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations - (springs[i].total_of_group_sizes));

        // I LOVE NULL-TERMINATED STRINGS!
        current_permutation[springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations] = '\0';
        last_permutation[springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations] = '\0';

        springs[i].arrangements = 0;

        while (memcmp(current_permutation, last_permutation, springs[i].number_of_unknown_locations) != 0)
        {

            printf("Current Permutation: %s\n", current_permutation);

            for (int j = 0; j < springs[i].number_of_known_operational_and_unknown_locations; j++)
            {
                *springs[i].known_operational_and_unknown_locations[j] = current_permutation[j];
            }
            


            if (validate_groups(&springs[i]))
            {
                printf("%.*s\n", (int)strcspn((char*)springs[i].row, "\r"),springs[i].row);
                springs[i].arrangements++;
            }

            //printf("Next permutation... \n");
            next_permutation(current_permutation, springs[i].number_of_known_operational_and_unknown_locations, springs[i].contiguous_groups, springs[i].number_of_contiguous_groups);
            
        }

        for (int j = 0; j < springs[i].number_of_known_operational_and_unknown_locations; j++)
        {
            *springs[i].known_operational_and_unknown_locations[j] = current_permutation[j];
        }
            
        if (validate_groups(&springs[i]))
        {
            printf("%.*s\n", (int)strcspn((char*)springs[i].row, "\r"),springs[i].row);
            springs[i].arrangements++;
        }      

        sum += springs[i].arrangements;
        printf("Count = %d\n", springs[i].arrangements);

        // Set last allocation to 0 so no previous # got left behind
        memset(current_permutation, 0, springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations);
        memset(last_permutation, 0, springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations);
        

    }

    printf("Sum of counts = %d\n", sum);

    // Unfolded
    free(current_permutation);
    free(last_permutation);

    
    current_permutation = NULL;
    last_permutation = NULL;
    
    sum = 0;

    for (int i = 0; i < number_of_springs; i++)
   {
        printf("Lines %d\n", i + 1);

        springs[i].length_of_row_unfolded = ((springs[i].length_of_row + 1) * 5) - 1;
        springs[i].row_unfolded = malloc(springs[i].length_of_row_unfolded );

        // Resest Changed data back to '?'
        for (int j = 0; j < springs[i].number_of_unknown_locations; j++)
        {
            *springs[i].unknown_locations[j] = '?';
        }

        // Resest Changed data back to '#'
        for (unsigned int j = 0; j < springs[i].number_of_known_operational_locations; j++)
        {
            *springs[i].known_operational_locations[j] = '#';
        }

        springs[i].number_of_unknown_locations = 0;
        

        for (size_t j = 0; j < springs[i].length_of_row_unfolded; j += springs[i].length_of_row + 1)
        {
            memcpy(&springs[i].row_unfolded[j], springs[i].row, springs[i].length_of_row);
        }
        for (size_t j = 1; j < 5; j++)
        {
            springs[i].row_unfolded[j * springs[i].length_of_row + j - 1] = '?';
        }
        
        springs[i].row = springs[i].row_unfolded;
        

        // Repeat Groups
        springs[i].contiguous_groups = realloc(springs[i].contiguous_groups, springs[i].number_of_contiguous_groups * 5 * sizeof(group));

        
        for (unsigned int j = 0; j < springs[i].number_of_contiguous_groups * 5; j++)
        {
            springs[i].contiguous_groups[j].group_location = NULL;
            springs[i].contiguous_groups[j].group_size = springs[i].contiguous_groups[j % springs[i].number_of_contiguous_groups].group_size;
        }
        

        // Solve the nonogram
        // First character of row
        current_group = &springs[i].contiguous_groups[0];

        if (current_group -> group_location == NULL && springs[i].row[0] == '#')
        {
            memset(&springs[i].row[0], '#', current_group -> group_size);
            springs[i].row[current_group -> group_size] = '.';
            current_group -> group_location = springs[i].row;
        }

        // extend first # found
        for (size_t j = 0; j <= current_group -> group_size ; j++)
        {
            if (springs[i].row[j] == '#')
            {
                memset (&springs[i].row[j], '#', current_group -> group_size - j);
            }
        }
        

        // Last character of row
        current_group = &springs[i].contiguous_groups[springs[i].number_of_contiguous_groups - 1];

        if (current_group -> group_location == NULL && springs[i].row[springs[i].length_of_row_unfolded - 1] == '#')
        {
            springs[i].row[springs[i].length_of_row_unfolded - current_group -> group_size - 1] = '.';
            memset(&springs[i].row[springs[i].length_of_row_unfolded - current_group -> group_size], '#', current_group -> group_size);
        }

        // extend last # found
        for (size_t j = 1; j <= current_group -> group_size ; j++)
        {
            if (springs[i].row[springs[i].length_of_row_unfolded - j] == '#')
            {
                memset(&springs[i].row[springs[i].length_of_row_unfolded - (current_group -> group_size - j) - 1], '#', current_group -> group_size - j);
            }
        }
        
        current_group = &springs[i].contiguous_groups[0];

        // ?# 1,   =   .#
        if (current_group -> group_location == NULL && springs[i].contiguous_groups[0].group_size == 1 && memcmp(springs[i].row, "?#", 2) == 0)
        {
            memcpy(springs[i].row, ".#", 2);
            current_group -> group_location = &springs[i].row[1];
            
        }
        
        // ?#? ,1,  ==  .#.
        for (unsigned int j = 0; j < springs[i].number_of_contiguous_groups; j++)
        {
            current_group = &springs[i].contiguous_groups[j];

            if (current_group -> group_location != NULL  && current_group -> group_size == 1)
            {
                memcpy(&current_group -> group_location[-1], ".#.", 3);
            }
        }
        
        //  .#????  3   =   ..###.?
        for(size_t j = 0; &springs[i].row[j] < (unsigned char*)strpbrk((char*)springs[i].row, "?"); j++)
        {
            if (springs[i].row[j] == '#')
            {
                for (unsigned int k = 0; k < springs[i].number_of_contiguous_groups; k++)
                {
                    current_group = &springs[i].contiguous_groups[k];

                    if (current_group -> group_location == NULL)
                    {
                        current_group -> group_location = &springs[i].row[j];
                        memset(current_group -> group_location, '#', current_group -> group_size);
                        springs[i].row[j + current_group -> group_size] = '.';
                        j += current_group -> group_size;
                        break;
                    }
                    else if (current_group -> group_location == &springs[i].row[j])
                    {
                        j += current_group -> group_size;
                        break;
                    }
                }
            }
        }

        for (size_t j = 0; j < springs[i].length_of_row_unfolded; j++)
        {
            if (springs[i].row[j] == '#')
            {
                if (strspn((char*)&springs[i].row[j], "#") == springs[i].max_group_size)
                {
                    springs[i].row[j + springs[i].max_group_size] = '.';
                    springs[i].row[j - 1] = '.';
                }
                
                j += strspn((char*)&springs[i].row[j], "#");
            }
        }
        
        

        // Get all ??? and ###
        free(springs[i].unknown_locations);
        springs[i].unknown_locations = NULL;
        free(springs[i].known_operational_and_unknown_locations);
        springs[i].known_operational_and_unknown_locations = NULL;
        springs[i].number_of_known_operational_and_unknown_locations = 0;
        springs[i].number_of_known_operational_locations = 0;

        for (size_t j = 0; j < springs[i].length_of_row_unfolded; j++)
        {
            if (springs[i].row[j] == '?')
            {
                springs[i].number_of_unknown_locations++;
                springs[i].unknown_locations = realloc(springs[i].unknown_locations, springs[i].number_of_unknown_locations * sizeof(unsigned char*));
                springs[i].unknown_locations[springs[i].number_of_unknown_locations - 1] = &springs[i].row[j];
            }
            if (springs[i].row[j] != '.')
            {
                springs[i].number_of_known_operational_and_unknown_locations++;
                springs[i].known_operational_and_unknown_locations = realloc(springs[i].known_operational_and_unknown_locations, springs[i].number_of_known_operational_and_unknown_locations * sizeof(unsigned char*));
                springs[i].known_operational_and_unknown_locations[springs[i].number_of_known_operational_and_unknown_locations - 1] = &springs[i].row[j];
            
                 if (springs[i].row[j] == '#')
                {
                    springs[i].number_of_known_operational_locations++;
                    springs[i].known_operational_locations = realloc(springs[i].known_operational_locations, springs[i].number_of_known_operational_locations * sizeof(unsigned char*));
                    springs[i].known_operational_locations[springs[i].number_of_known_operational_locations - 1] = &springs[i].row[j];
                }           
            }
            
        }    
        

        springs[i].arrangements = 0;

        
        springs[i].contiguous_groups = realloc(springs[i].contiguous_groups, 5 * springs[i].number_of_contiguous_groups * sizeof(group));
        
        for (unsigned int j = springs[i].number_of_contiguous_groups; j < 5 * springs[i].number_of_contiguous_groups; j++)
        {
            springs[i].contiguous_groups[j] = springs[i].contiguous_groups[j % springs[i].number_of_contiguous_groups];
        }
        
        springs[i].number_of_contiguous_groups *= 5;

        springs[i].total_of_group_sizes  = 0;

        for (unsigned int j = 0; j < springs[i].number_of_contiguous_groups; j++)
        {
            springs[i].total_of_group_sizes += springs[i].contiguous_groups[j].group_size;
        }
        

        
        current_permutation = realloc(current_permutation, springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations);
        memset(current_permutation, '.', springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations);
        memset(current_permutation, '#', springs[i].total_of_group_sizes);

        last_permutation = realloc(last_permutation, springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations);
        memset(last_permutation, '#', springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations);
        memset(last_permutation, '.', springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations - (springs[i].total_of_group_sizes));

        printf("%s\n", springs[i].row_unfolded);

        // Repeat until reached last permutation
        while (memcmp(current_permutation, last_permutation, springs[i].number_of_unknown_locations) != 0)
        {
            printf("Current Permutation: %s\n", current_permutation);

            // Update data with current permutation
            for (int j = 0; j < springs[i].number_of_known_operational_and_unknown_locations; j++)
            {
                *springs[i].known_operational_and_unknown_locations[j] = current_permutation[j];
            }
            
            // Check if current permutation matches the groups
            if (validate_groups(&springs[i]))
            {
                printf("Valid Group: %s\n", springs[i].row);
                springs[i].arrangements++;
            }

            // Calcualte next permutation
            next_permutation(current_permutation, springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations, springs[i].contiguous_groups, springs[i].number_of_contiguous_groups);

           

            if (springs[i].known_operational_locations[0] < (unsigned char*)strpbrk((char*)springs[i].row, "#"))
            {
                break;
            }
        }

        for (int j = 0; j < springs[i].number_of_unknown_locations; j++)
        {
            *springs[i].unknown_locations[j] = current_permutation[j];
        }
            
        if (validate_groups(&springs[i]))
        {
            springs[i].arrangements++;
        }      
        
        sum += springs[i].arrangements;

        // Set last allocation to 0 so no previous # got left behind
        memset(current_permutation, 0, springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations);
        memset(last_permutation, 0, springs[i].number_of_unknown_locations + springs[i].number_of_known_operational_locations);


    }

    

    for (int i = 0; i < number_of_springs; i++)
    {
        free(springs[i].contiguous_groups);
        free(springs[i].unknown_locations);
        free(springs[i].row);
        
    }
    
    free(springs);

    free(file_buffer);

    
    printf("Sum of counts when unfolded = %d\n", sum);

    return 0;
}