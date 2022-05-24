//
// General purpose helper functions
//

#ifndef CROSS_FOX_UTILS_H
#define CROSS_FOX_UTILS_H

#include <string.h>
#include <stdbool.h>

typedef enum RabbitContentType{
    RBT_CT_NOT_DEFINED,

    // Text
    RBT_CT_TXT_HTML,
    RBT_CT_TXT_CSS,
    RBT_CT_TXT_CSV,
    RBT_CT_TXT_PLAIN,
    RBT_CT_TXT_XML,

}RabbitContentType;

bool rbt_str_equals(char* str1, char* str2);
unsigned int rbt_get_num_of_chars(char* string, char find);
char* rbt_get_file_content_type(char* filepath);
void rbt_str_replace(char *target, const char *needle, const char *replacement);

#endif //CROSS_FOX_UTILS_H
