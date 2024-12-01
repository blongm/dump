#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define EXPANSION 1000000

size_t file_size;
unsigned char* file_buffer;

// ギャラクシーー!
typedef struct galaxy
{
    unsigned char* galaxy;
    int x;
    int y;
    int x_old;
    int y_old;

} galaxy;

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

int sort_galaxies_by_x(const void *a, const void *b)
{
    galaxy* galaxy_a = (galaxy*)a;
    galaxy* galaxy_b = (galaxy*)b;

    return (galaxy_a -> x > galaxy_b -> x) - (galaxy_a -> x < galaxy_b -> x);
}

int main(int argc, char** argv)
{   
    if (argc < 1) return 1;
    load_file_into_memory(argv[1]);

    int number_of_galaxies = 0;
    galaxy* galaxies = NULL;

    galaxy* current_galaxy = NULL; 
    int current_x = 0;
    int current_y = 0;

    int current_y_old = 0;

    // Record all galxies
    for (size_t i = 0; i < file_size; i++)
    {
        if (file_buffer[i] == '#')
        {
            number_of_galaxies++;
            galaxies = realloc(galaxies, number_of_galaxies * sizeof(galaxy));

            current_galaxy = &galaxies[number_of_galaxies - 1];
            current_galaxy -> galaxy = &file_buffer[i];
            current_galaxy -> x = current_x;
            current_galaxy -> y = current_y;
            current_galaxy -> x_old = current_x;
            current_galaxy -> y_old = current_y_old;
        }
        else if (iscntrl(file_buffer[i]))
        {
            //  Is next row empty? 
            if (strspn((char*)&file_buffer[i + 2], ".") == strspn((char*)&file_buffer[i + 2], ".#"))
            {
                current_y++;
                current_y_old += EXPANSION - 1;
                i += strspn((char*)&file_buffer[i], ".") + 1;
            }

            current_y++;
            current_y_old++;
            current_x = -1;
            i++;
        }
        
        current_x++;
    }

    free(file_buffer);

    qsort(galaxies, number_of_galaxies, sizeof(galaxy), sort_galaxies_by_x);

    // Count through all numbers (columns)
    for (int i = 0; i < galaxies[number_of_galaxies - 1].x; i += 2)
    {
        // Check each galaxy
        for (int j = 0; j < number_of_galaxies; j++)
        {
            // Found matching glaxy so go to next row
            if (i == galaxies[j].x)
            {
                i++;
                j = 0;
            }
        }

        // No matching galaxy so move all later galaxies forward
        for (int j = 0; j < number_of_galaxies; j++)
        {
            if (galaxies[j].x > i)
            {
                galaxies[j].x++;
            }
        }
        
    }
    
    // Older Galaxies(part 2)
    // Count through all numbers (columns)
    for (int i = 0; i < galaxies[number_of_galaxies - 1].x_old; i += EXPANSION)
    {
        // Check each galaxy
        for (int j = 0; j < number_of_galaxies; j++)
        {
            // Found matching galaxy so go to next row
            if (i == galaxies[j].x_old)
            {
                i++;
                j = 0;
            }
        }

        // No matching galaxy so move all later galaxies forward
        for (int j = 0; j < number_of_galaxies; j++)
        {
            if (galaxies[j].x_old > i)
            {
                galaxies[j].x_old += EXPANSION - 1;
            }
        }
        
    }

    int sum_of_lengths = 0;
    long long sum_of_lengths_old = 0;

    // Every galaxy except the last one
    for (int i = 0; i < number_of_galaxies - 1; i++)
    {
        // Every galaxy after this one
        for (int j = i + 1; j < number_of_galaxies; j++)
        {
            sum_of_lengths += galaxies[j].x - galaxies[i].x + abs(galaxies[j].y - galaxies[i].y);
            sum_of_lengths_old += galaxies[j].x_old - galaxies[i].x_old + abs(galaxies[j].y_old - galaxies[i].y_old);
            printf("Current sum = %lld\n", sum_of_lengths_old);
        }
    }
    
    free(galaxies);

    printf("Sum of lengths = %d\n", sum_of_lengths);
    printf("Sum of lengths for galaxies = %lld\n", sum_of_lengths_old);

    return 0;
}