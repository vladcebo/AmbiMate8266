#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_

#include "c_types.h"
#include "esp_common.h"
#include "espconn.h"

class HttpClient {
    
public:
    static void ICACHE_FLASH_ATTR init(void);
    static void ICACHE_FLASH_ATTR post(const char* host, int port, const char* path, const char* headers, const char* payload);

};


#endif