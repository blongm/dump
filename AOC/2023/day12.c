#include <stdlib.h>
#include <stdio.h>

// Load file
void file_open(char* file_name)
{
    return file_open(file_name, "rb");
}


int main(int argc, char** argv)
{
    if (argc < 1) return 1;
    FILE* file = file_open(argv[1]);

    return 0;
}