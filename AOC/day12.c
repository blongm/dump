#include <stdlib.h>
#include <stdio.h>

// Load file
FILE* open(char* file_name)
{
    return fopen(file_name, "rb");
}

void add_chr(char* arr, size_t* arr_size, char chr)
{
    *arr_size = (*arr_size) + 1;
    arr = realloc(arr, (*arr_size) * sizeof(char));
    arr[(*arr_size) - 1] = chr;
}

char* getl(FILE* file)
{
    size_t line_len = 0;
    char* line = realloc(line, 0);
    char c;

    while ((c = fgetc(file)) != '\n')
    {
        add_chr(line, &line_len, c);
    }

    add_chr(line, &line_len, '\0');

    return line;
}

int main(int argc, char** argv)
{
    if (argc < 1) return 1;
    FILE* file = open(argv[1]);

    char* line;

    while ((line = getl(file))[0] != '\0')
    {
        printf("%s", line);
        free(line);
    }

    fclose(file);

    return 0;
}