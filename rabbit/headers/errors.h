//
// Error implementation
//

#ifndef RABBIT_ERRORS_H
#define RABBIT_ERRORS_H

#include <limits.h>
#include <stdio.h>
#include <time.h>

/**
 * Rabbit error representation <br>
 * <b>Note:</b> RBT_ERR_NO_ERROR is defined as 0, and every other error is a negative number
 */
typedef enum RabbitError{
  RBT_ERR_NO_ERROR = 0,
  RBT_ERR_NULL_POINTER = INT_MIN,
  RBT_ERR_SOCKET_ERROR,
  RBT_ERR_KEY_USED
} RabbitError;

#define rbt_log(error_message, destination) _rbt_log(error_message, destination, __FILE__, __LINE__)
int _rbt_log(char* message, char* destination, char* file, size_t line);

#endif //RABBIT_ERRORS_H
