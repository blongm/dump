#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct list
{
    int* arr;
    size_t size;
    size_t width;

} list;

void* last(list list)
{
    return &list.arr[list.size/list.width - 1];
}

// Load file.
FILE* open(char* file_name)
{
    return fopen(file_name, "rb");
}

// Add char to string.
char* add_chr(char* arr, size_t* arr_size, char chr)
{
    *arr_size = (*arr_size) + sizeof(char);
    arr = realloc(arr, (*arr_size) * sizeof(char));
    arr[(*arr_size)/sizeof(char) - 1] = chr;
    return arr;
}

// Add pointer to array of pointers
void** add_ptr(void** arr, size_t* arr_size, void* ptr)
{
    *arr_size += sizeof(ptr);
    arr = realloc(arr, (*arr_size) * sizeof(void*));
    arr[(*arr_size)/sizeof(void*) - 1] = ptr;
    return arr;
}

// Add int to array of ints.
int* add_int(int* arr, size_t* arr_size, int i)
{
    *arr_size = (*arr_size) + sizeof(int);
    arr = realloc(arr, (*arr_size) * sizeof(int));
    arr[(*arr_size)/sizeof(int) - 1] = i;
    return arr;    
}

// Get line from file.
// Returns pointer to line.
char* getl(FILE* file)
{
    size_t line_len = 0;
    char* line = NULL;
    char c;

    while ((c = fgetc(file)) != '\n' && !feof(file))
    {
        line = add_chr(line, &line_len, c);

        if (c == EOF)
        {
            return line;
        }
    }

    line = add_chr(line, &line_len, '\0');

    return line;
}

// Splits a string by another string
char** split(char* arr, char* split)
{
    char** list = NULL;
    size_t list_size = 0;

    list = (char**)add_ptr((void**)list, &list_size, arr);

    char* substring;
    while ((substring = strstr(arr, split)) != NULL)
    {
        list = (char**)add_ptr((void**)list, &list_size, substring);

        arr = substring + sizeof(split);
    }

    return list;
}

int comp(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    return (arg1 > arg2) - (arg1 < arg2);
}

int main(int argc, char** argv)
{
    if (argc < 1) return 1;
    FILE* file = open(argv[1]);

    list list_a = {NULL, 0, sizeof(int)};
    list list_b = {NULL, 0, sizeof(int)};

    while (!feof(file))
    {
        char* line = getl(file);

        char** splitl = split(line, "   ");

        list_a.arr = add_int(list_a.arr, &list_a.size, atoi(splitl[0]));
        list_b.arr = add_int(list_b.arr, &list_b.size, atoi(splitl[1]));

        free(splitl);
        free(line);
    }

    fclose(file);

    qsort(list_a.arr, list_a.size/list_a.width, list_a.width, comp);
    qsort(list_b.arr, list_b.size/list_b.width, list_b.width, comp);

    int dist = 0;

    int count = 0;
    int similarity = 0;;

    for (size_t i = 0; i < list_a.size/list_a.width; i++)
    {
        dist += abs(list_a.arr[i] - list_b.arr[i]);

        count = 0;

        for (size_t j = 0; j < list_b.size/list_b.width; j++)
        {

            if (list_a.arr[i] == list_b.arr[j])
            {
                count++;
            }
        }

        similarity += list_a.arr[i] * count;
        
    }

    free(list_a.arr);
    free(list_b.arr);

    printf("Dist: %d\n", dist);
    printf("Similarity: %d", similarity);
    

    return 0;
}