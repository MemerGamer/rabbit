//
// General purpose helper functions
//

#include "../headers/utils.h"

bool rbt_str_equals(char* str1, char* str2){
    return strcmp(str1, str2) == 0;
}

unsigned int rbt_get_num_of_chars(char* string, char find){
    unsigned int count = 0;
    for (int i = 0; string[i]; i++){
        count += (string[i] == '.');
    }
    return count;
}

char* rbt_get_file_content_type(char* filepath){
    char ext[10];
    strcpy(ext, strrchr(filepath, '.'));

    if (rbt_str_equals(ext, ".html")){
        return "text/html";
    }
    else if (rbt_str_equals(ext, ".txt")){
        return "text/plain";
    }
}