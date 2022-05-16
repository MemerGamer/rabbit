//
// Defaults to use during the lifespan of the server
// These values can be changed
//

#ifndef RABBIT_DEFAULTS_H
#define RABBIT_DEFAULTS_H

#include <stdbool.h>

// region General settings
#define RBT_LOG_ERRORS true
#define RBT_ERROR_LOG_FILE "CON"
// endregion

// region Server defaults
#define RBT_LOCALHOST "127.0.0.1"

#define RBT_MIN_RESPONSE_BUFFER_SIZE 5000
#define RBT_MED_RESPONSE_BUFFER_SIZE 10000
#define RBT_MAX_RESPONSE_BUFFER_SIZE 20000

#define RBT_MAX_REQUEST_BUFFER_SIZE 5000

#define RBT_HASH_TABLE_MAX_BUCKETS 30
#define RBT_HASH_TABLE_MAX_ENDPOINTS 10

// endregion
#endif //RABBIT_DEFAULTS_H
