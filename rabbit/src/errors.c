//
// Error implementation
//

#include "../headers/errors.h"
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <stdio.h>
    #include <time.h>
#endif
#pragma execution_character_set("utf-8")

int _rbt_log(char *message, char *destination, char* file, size_t line) {
#ifdef _WIN32
    freopen(destination, "w", stdout);
    SetConsoleOutputCP( 65001 );
#else
    FILE* fp = freopen(destination, "w", stdout);
#endif
    printf("\n");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char time_text[20];
    sprintf(time_text, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    char* file_text = (char*)calloc(strlen(file)+4, sizeof(char));
    sprintf(file_text, "%s:%zu", file, line);
    int box_width = strlen(file_text) > strlen(message) ? strlen(file_text) : strlen(message);

    if (strlen(message) > 115){
        message[115] = '\0';
        message[114] = '.';
        message[113] = '.';
        message[112] = '.';
        box_width = 115;
    }
    if (strlen(file_text) > 115){
        file_text[115] = '\0';
        file_text[114] = '.';
        file_text[113] = '.';
        file_text[112] = '.';
        box_width = 115;
    }

    printf("%c%c%c", '\xE2', '\x95', '\xAD');
    for (int i = 0; i < box_width/2 - 10 + 2; ++i) {
        printf("%c%c%c", '\xE2', '\x94', '\x80');
    }

    printf("%s", time_text);

    for (int i = 0; i < box_width/2 - 10 + 2 - ((!(box_width%2))?1:0); ++i) {
        printf("%c%c%c", '\xE2', '\x94', '\x80');
    }
    printf("%c%c%c\n", '\xE2', '\x95', '\xAE');

    printf("%c%c%c", '\xE2', '\x94', '\x82');
    printf(" %-*s ", box_width, message);
    printf("%c%c%c\n", '\xE2', '\x94', '\x82');

    printf("%c%c%c", '\xE2', '\x94', '\x82');
    for (int i = 0; i < box_width+2; ++i) {
        printf("%c%c%c", '\xE2', '\x94', '\x80');
    }
    printf("%c%c%c\n", '\xE2', '\x94', '\x82');

    printf("%c%c%c", '\xE2', '\x94', '\x82');
    printf(" %-*s ",box_width, file_text);
    printf("%c%c%c\n", '\xE2', '\x94', '\x82');

    printf("%c%c%c", '\xE2', '\x95', '\xB0');
    for (int i = 0; i < box_width+2; ++i) {
        printf("%c%c%c", '\xE2', '\x94', '\x80');
    }
    printf("%c%c%c\n", '\xE2', '\x95', '\xAF');

//    free(file_text);  

    freopen("CON", "w", stdout);
    return 1;
}
