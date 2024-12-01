#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

size_t file_size;
unsigned char* file_buffer;

typedef struct Sequence
{
    int* sequence;
    int sequence_length;

} Sequence;

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


int* calculate_differences(int* sequence, int sequence_length)
{
    int length_of_differences = (sequence_length) - 1;
    int* differences = malloc(length_of_differences * sizeof(int));

    for (int i = 0; i < length_of_differences; i++)
    {
        differences[i] = sequence[i + 1] - sequence[i];
    }

    return differences;
}

int calculate_next_value(int* sequence, int sequence_length)
{
    int length_of_differences = (sequence_length) - 1;
    int* differences = calculate_differences(sequence, sequence_length);

    for (int i = 0; i < length_of_differences; i++)
    {
        if (differences[i] != 0)
        {
            return sequence[sequence_length - 1] + calculate_next_value(differences, length_of_differences);
        }
    }

    int next_value = sequence[sequence_length - 1] + differences[length_of_differences - 1];

    free(differences);

    return next_value;

}

int calculate_previous_value(int* sequence, int sequence_length)
{
    int length_of_differences = (sequence_length) - 1;
    int* differences = calculate_differences(sequence, sequence_length);

    for (int i = 0; i < length_of_differences; i++)
    {
        if (differences[i] != 0)
        {
            return sequence[0] - calculate_previous_value(differences, length_of_differences);
        }
    }

    int next_value = sequence[0] - differences[0];

    free(differences);

    return next_value;

}

int main(int argc, char** argv)
{   
    if (argc < 1) return 1;
    load_file_into_memory(argv[1]);

    Sequence* Sequences = NULL;
    int number_of_sequences = 0;

    Sequence* Current_Sequence = NULL;

    int sum_of_next_values = 0;
    int sum_of_previous_values = 0;

    for (size_t i = 0; i < file_size; i++)
    {
        // New sequence
        number_of_sequences++;
        Sequences = realloc(Sequences, number_of_sequences * sizeof(Sequence));
        Sequences[number_of_sequences - 1].sequence = NULL;
        Sequences[number_of_sequences - 1].sequence_length = 0;

        Current_Sequence = &Sequences[number_of_sequences - 1];

        while (isdigit(file_buffer[i]) || file_buffer[i] == '-')
        {
            Current_Sequence -> sequence_length++;
            Current_Sequence -> sequence = realloc(Current_Sequence -> sequence, Current_Sequence -> sequence_length * sizeof(int));
            Current_Sequence -> sequence[Current_Sequence -> sequence_length - 1] = atoi((char*)&file_buffer[i]);

            i += strspn((char*)&file_buffer[i], "1234567890-") + 1;
        }

        sum_of_next_values += calculate_next_value(Current_Sequence -> sequence, Current_Sequence -> sequence_length);
        sum_of_previous_values += calculate_previous_value(Current_Sequence -> sequence, Current_Sequence -> sequence_length);

        free(Current_Sequence -> sequence);
    }

    free(Sequences);
    
    printf("Sum of next values - %d\n", sum_of_next_values);
    printf("Sum of previous values - %d\n", sum_of_previous_values);

    return 0;
}