// created by Aleksander

/* Чтобы показать информацию о пришедшем письме, нужно сначала её найти.
 * Для этого в файле письма необходимо найти специальные заголовки.
 *
 * Составить программу построчной фильтрации текста.
 * Суть фильтра — отбор строк, начинающихся с одного из следующих выражений: «Date:», «From:»,«To:», «Subject:».
 * Текстовые строки подаются на стандартный ввод программы, результат программы должен подаваться на стандартный вывод.
 *
 * Процедура фильтрации должна быть оформлена в виде отдельной функции, которой подается на вход массив строк, выделенных в динамической памяти и его длина.
 * На выходе функция возвращает указатель на NULL-терминированный массив с найденными строками (последним элементом массива добавлен NULL для обозначения, что данных больше нет).
 *
 * Программа должна уметь обрабатывать возникающие ошибки (например, ошибки выделения памяти). В случае возникновения ошибки нужно вывести в поток стандартного вывода сообщение "[error]" и завершить выполнение программы.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 4096
#define MAX_CPY_SIZE 1024
#define COUNT_OF_KEYS 4
#define ERROR "[error]"

const char* key_words[] = {"Date:", "From:", "To:", "Subject:"};

// input string array
size_t input_strings(char ** strings);

// filter array
char * array_filter(char * inp[], size_t lines, char ** out, size_t *out_size);

void free_array(char ** result, size_t size);

int main(){
    char ** p_string = calloc(MAX_SIZE, sizeof(char* ));
    if (p_string == NULL) {
        printf(ERROR);
        return 0;
    }

    size_t size = input_strings(p_string);

    if (size == 0) {
        free(p_string);
        return 0;
    }

    char ** p = NULL;

    char ** result = calloc(size, sizeof(char* ));
    if (result == NULL) {
        free_array(p_string, size);
        printf(ERROR);
        return 0;
    }

    // process
    size_t out_size = 0;
    char * error = array_filter(p_string, size, result, &out_size);

    if (error != NULL) {
        printf(ERROR);
        return 0;
    }

    // Output
    p = result;
    while (*p) {
        if (*p != NULL) {
            printf("%s\n", *p);
            p++;
        }
    }

    free_array(p_string, size);
    free_array(result, out_size);
    return 0;
}

char* array_filter(char *inp[], size_t lines, char ** out, size_t *out_size){

    char* line = calloc(MAX_SIZE, sizeof(char));
    if (line == NULL) {
        return ERROR;
    }

    size_t j = 0;
    size_t key_size = 0;
    _Bool is_ok = 0; // 1 - match, 0 - no matches
    size_t current_out_index = 0;

    for (size_t i = 0; i < lines; ++i){
        if (inp[i] != NULL) {
            strncpy(line, inp[i], strlen(inp[i]) * sizeof(char*));
            is_ok = 1;
            for (size_t t = 0; t < COUNT_OF_KEYS; ++t) {
                if (line[0] == key_words[t][0]) {
                    j = 1;

                    key_size = strlen(key_words[t]);
                    while (line[j] != '\0' && j < key_size) {
                        if (line[j] != key_words[t][j]) {
                            is_ok = 0;
                            break; // it is not a key
                        }
                        j++ ;
                    }
                    if (is_ok == 1) {
                        out[current_out_index] = (char* )malloc(MAX_CPY_SIZE*sizeof(char));
                        strncpy(out[current_out_index], line, MAX_CPY_SIZE* sizeof(char));
                        current_out_index += 1;
                        break; // one key was found, no more in current char
                    }
                }
            }
        }
    }
    *out_size = current_out_index;
    free(line);
    return NULL;
}

void free_array(char ** result, size_t size) {
    if (!result)
        return;
    for (size_t i = 0; i < size; i++) {
        free(result[i]);
    }
    free(result);
}

size_t input_strings(char **strings) {
    if (!strings) {
        return 0;
    }

    const size_t len_max = 1024;

    size_t n_lines = 0;

    while (1) {
        char *buff = calloc(len_max, sizeof(char));
        if (!buff) {
            return n_lines;
        }
        fgets(buff, len_max, stdin);
        if (strlen(buff) > 0 && buff[strlen(buff) - 1] == '\n') {
            buff[strcspn(buff, "\n")] = '\0';
        }

        if (strlen(buff) == 0) {
            free(buff);
            break;
        }
        strings[n_lines] = buff;
        n_lines++;
    }
    return n_lines;
}