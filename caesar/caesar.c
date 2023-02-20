#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * Usage:
 * Program reads the input from stdin and prints the ciphered output to stdout.
 * To perform cipher operation, you must provide a seed (number of positions each character should be rotated by).
 * The seed must be provided as a first argument of the program (if not, default value is 10).
 * To decipher message, you must provide a seed with opposite value of the original seed.
 * (If message was ciphered by seed 4, it may be deciphered using seed -4).
*/

const int DEFAULT_SIZE = 8;
const int ALPHABET_LEN = 26;
const int UPPERCASE_OFFSET = 65;
const int LOWERCASE_OFFSET = 97;

void apply_cipher(int seed, char* buffer, int buf_len);
void *realloc_array(void *list, size_t *arr_size, size_t type_size);

int main(int argc, char** argv){
    int seed = 0;
    if (argc < 2 || !strlen(argv[1])){
        seed = 10;
    }
    else{
        seed = strtol(argv[1], NULL, 10) % ALPHABET_LEN;
    }

    //Handles negative seed
    if (seed < 0){
        seed += ALPHABET_LEN;
    }


    int inp_len = 0;
    size_t buffer_capacity = DEFAULT_SIZE;
    char* buffer = calloc(DEFAULT_SIZE, sizeof(char));
    if (!buffer){
        fprintf(stderr, "Allocation error!\n");
        return 1;
    }

    for (; (buffer[inp_len] = getchar()) != EOF; ++inp_len){
        if (inp_len == buffer_capacity - 1){
            if ((buffer = realloc_array(buffer, &buffer_capacity, sizeof(char)))){
                fprintf(stderr, "Reallocation error on buffer size %d bytes", inp_len);
                return 1;
            }
        }
    }

    apply_cipher(seed, buffer, inp_len);
    fwrite(buffer, sizeof(char), inp_len, stdout);
    free(buffer);
    return 0;

}

void apply_cipher(int seed, char* buffer, int buf_len){
    int offset = 0;
    for (int i = 0; i < buf_len; ++i){
        if (!isalpha(buffer[i])){
            continue;
        }
        offset = buffer[i] < LOWERCASE_OFFSET ? UPPERCASE_OFFSET : LOWERCASE_OFFSET;
        buffer[i] -= offset;
        buffer[i] += seed;
        buffer[i] %= ALPHABET_LEN;
        buffer[i] += offset;
    }
}

/**
 * @brief Reallocs array. If realloc fails, frees array and returns 0. 
 * Also increases arr_size
 * 
 * @param list Array to be reallocated
 * @param arr_size Pointer to array size, incremented before reallocation
 * @param type_size Size of data type stored in array
 * 
 * @return Reallocated list, NULL in case of error
 * */
void *realloc_array(void *list, size_t *arr_size, size_t type_size){
    void *temp_ptr = list;
    *arr_size += DEFAULT_SIZE;
    list = realloc(list, (*arr_size) * type_size);
    if (!(list)){
        free(temp_ptr);
        temp_ptr = NULL;
        return 0;
    }
    return list;
}