#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

size_t file_size;
unsigned char* file_buffer;

typedef struct Map
{
    int x;
    int y;

} Map;

typedef enum direction
{
    NORTH,
    NORTH_EAST,
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    NORTH_WEST
} direction;

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

unsigned char** find_adjacent_tiles(Map Map, unsigned char* tile)
{
    unsigned char** adjacent_tiles = malloc(8 * sizeof(char*));
    

    adjacent_tiles[NORTH]       = tile >= file_buffer + Map.x ? &tile[-Map.x] : NULL;       
    adjacent_tiles[NORTH_EAST]  = tile >= file_buffer + Map.x ? &tile[-Map.x + 1] : NULL;  
    adjacent_tiles[EAST]        = tile != file_buffer + file_size - 1 ? tile + 1 : NULL;      
    adjacent_tiles[SOUTH_EAST]  = tile <= file_buffer + file_size - Map.x - 1 ? &tile[Map.x + 1] : NULL;    
    adjacent_tiles[SOUTH]       = tile <= file_buffer + file_size - Map.x ? &tile[Map.x] : NULL;        
    adjacent_tiles[SOUTH_WEST]  = tile <= file_buffer + file_size - Map.x ? &tile[Map.x - 1] : NULL;    
    adjacent_tiles[WEST]        = tile != file_buffer ? &tile[-1] : NULL;        
    adjacent_tiles[NORTH_WEST]  = tile >= file_buffer + Map.x + 1 ? &tile[-Map.x - 1] : NULL;   


    return adjacent_tiles;

}

int main(int argc, char** argv)
{   
    if (argc < 1) return 1;
    load_file_into_memory(argv[1]);

    Map Map;
    Map.x = strspn((char*)file_buffer, "|-LJ7F.S") + 2;
    Map.y = 1;

    unsigned char* current_position;

    for (size_t i = 0; i < file_size; i++)
    {
        if (iscntrl(file_buffer[i]))
        {
            Map.y++;
            i++;
        }
        else if (file_buffer[i] == 'S')
        {
            current_position = &file_buffer[i];
        }
    }

    direction facing_direction;
    unsigned char** adjacent_tiles = find_adjacent_tiles(Map, current_position);
    
    
    if (adjacent_tiles[NORTH] != NULL && (*adjacent_tiles[NORTH] == '7' ||  *adjacent_tiles[NORTH] == '|' || *adjacent_tiles[NORTH] == 'F'))
    {
        facing_direction = NORTH;
    }
        
    else if (adjacent_tiles[EAST] != NULL && (*adjacent_tiles[EAST] == 'J' ||  *adjacent_tiles[EAST] == '-' || *adjacent_tiles[EAST] == '7'))
    {
        facing_direction = EAST;
    }

    else if (adjacent_tiles[SOUTH] != NULL && (*adjacent_tiles[SOUTH] == 'L' ||  *adjacent_tiles[SOUTH] == '|' || *adjacent_tiles[SOUTH] == 'J'))
    {
        facing_direction = SOUTH;
    }

    else if (adjacent_tiles[WEST] != NULL && (*adjacent_tiles[WEST] == 'F' ||  *adjacent_tiles[WEST] == '-' || *adjacent_tiles[WEST] == 'L'))
    {
        facing_direction = WEST;
    }

    current_position = adjacent_tiles[facing_direction];

    int steps_taken = 1;

    while (*current_position != 'S')
    {

        switch (facing_direction)
        {
            case NORTH:

                if (*current_position == '7') facing_direction = WEST; 
                else if (*current_position == 'F') facing_direction = EAST; 
                break;
            
            case EAST:

                if (*current_position == 'J') facing_direction = NORTH; 
                else if (*current_position == '7') facing_direction = SOUTH; 
                break;
            
            case SOUTH:

                if (*current_position == 'L') facing_direction = EAST; 
                else if (*current_position == 'J') facing_direction = WEST; 
                break;

            case WEST:

                if (*current_position == 'F') facing_direction = SOUTH; 
                else if (*current_position == 'L') facing_direction = NORTH; 
                break;

            default:

                break;
        }

        free(adjacent_tiles);
        adjacent_tiles = find_adjacent_tiles(Map, current_position);
        *current_position = 'S';
        current_position = adjacent_tiles[facing_direction];
        steps_taken++;
        
    }

    printf("Steps Taken - %d\n", steps_taken / 2);

    for

    free(adjacent_tiles);

    return 0;
}