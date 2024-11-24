#include <stdlib.h>
#include <stdio.h>

// Load file
FILE* open(char* file_name)
{
    return fopen(file_name, "rb");
}

char* getl(file)
{
    char* line = malloc(0);
    size_t line_len = 0;
    char c;

    while (c = fgetc(file) != '\n')
    {
        line = realloc()
    }
}

int main(int argc, char** argv)
{
    if (argc < 1) return 1;
    FILE* file = open(argv[1]);



    return 0;
}