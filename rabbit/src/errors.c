//
// Error implementation
//

#include "../headers/errors.h"

int _rbt_log(char *message, char *destination, char* file, size_t line) {
    freopen(destination, "w", stdout);
    printf("\n");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int flag = 0;
    
    if(printf("%d-%02d-%02d %02d:%02d:%02d \n\t%s at: %s:%zu\n",
              tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, message, file, line)){
        flag = 1;
    }

    freopen("CON", "w", stdout);
    return flag;
}
