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

char *rbt_get_file_content_str(RabbitContentType contentType) {
    switch (contentType) {
        // Application
        case RBT_CT_APP_EDI_X_12: return "application/EDI-X12";
        case RBT_CT_APP_EDIFACT: return "application/EDIFACT";
        case RBT_CT_APP_JAVASCRIPT: return "application/javascript";
        case RBT_CT_APP_OCTET_STREAM: return "application/octet-stream";
        case RBT_CT_APP_OGG: return "application/ogg";
        case RBT_CT_APP_PDF: return "application/pdf";
        case RBT_CT_APP_XHTML_HTML: return "application/xhtml+xml";
        case RBT_CT_APP_X_SHOCKWAVE_FLASH: return "application/x-shockwave-flash";
        case RBT_CT_APP_JSON: return "application/json";
        case RBT_CT_APP_LD_JSON: return "application/ld+json";
        case RBT_CT_APP_XML: return "application/xml";
        case RBT_CT_APP_ZIP: return "application/zip";
        case RBT_CT_APP_X_WWW_FORM_URLENCODED: return "application/x-www-form-urlencoded";

        // Audio
        case RBT_CT_AUDIO_MPEG: return "audio/mpeg";
        case RBT_CT_AUDIO_X_MS_WMA: return "audio/x-ms-wma";
        case RBT_CT_AUDIO_VND_RN_REALAUDIO: return "audio/vnd.rn-realaudio";
        case RBT_CT_AUDIO_X_WAV: return "audio/x-wav";


        default: return "text/html";    //default text/html
    }
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
    else if (rbt_str_equals(ext, ".css")){
        return "text/css";
    }
    else if (rbt_str_equals(ext, ".png")){
        return "image/png";
    }
    else if (rbt_str_equals(ext, ".js")){
        return "application/javascript";
    }
}

void rbt_str_replace(char *target, const char *needle, const char *replacement){
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

    while (1) {
        const char *p = strstr(tmp, needle);

        // walked past last occurrence of needle; copy remaining part
        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }

        // copy part before needle
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        // copy replacement string
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        // adjust pointers, move on
        tmp = p + needle_len;
    }

    // write altered string back to target
    strcpy(target, buffer);
}