#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_

#include "c_types.h"
class HttpClient {
    
public:

    typedef struct http_req_s {
        char* host;
        char* path;
        int   port;
        char* payload;
        char* headers;
    } http_req_t;

    static void ICACHE_FLASH_ATTR post(const char* host);

private:
    static void ICACHE_FLASH_ATTR request(const char* host, int port);
};


#endif