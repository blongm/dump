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
    char* line = malloc(0);
    char c;

    while ((c = fgetc(file)) != '\n')
    {
        add_chr(line, &line_len, c);

        if (c == EOF)
        {
            return line;
        }
    }

    add_chr(line, &line_len, '\0');

    return line;
}

int main(int argc, char** argv)
{
    if (argc < 1) return 1;
    FILE* file = open(argv[1]);

    char* line = malloc(0);

    while ((line = getl(file))[0] != EOF)
    {
        printf("%s\n", line);
        free(line);
    }

    printf("%s\n", line);
    free(line);
    fclose(file);

    return 0;
}