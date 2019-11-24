#include "HttpClient.hpp"

typedef struct http_req_s {
    char* host;
    char* path;
    int   port;
    char* payload;
    char* headers;
} http_req_t;

static char* ICACHE_FLASH_ATTR strdup(const char* str) {
    char* new_str = (char*)malloc(strlen(str) + 1);
    if (new_str == NULL) {
        os_printf("strdup malloc error\n");
        return NULL;
    }
    return strcpy(new_str, str);
}

static void ICACHE_FLASH_ATTR receive_cb(void * arg, char * buf, unsigned short len)
{
    /* Do nothing on receive */
}

static void ICACHE_FLASH_ATTR sent_cb(void * arg)
{
    struct espconn * conn = (struct espconn *)arg;
    http_req_t * req      = (http_req_t *)conn->reserve;

    if (req->payload == NULL) {
        os_printf("Message sent successufully\n");
    }
    else {
        // Send the payload
        os_printf("Sending request body\n");
        os_printf("%s\n", req->payload);
        espconn_sent(conn, (uint8_t *)req->payload, strlen(req->payload));
        free(req->payload);
        req->payload = NULL;
    }
}

static void ICACHE_FLASH_ATTR connect_cb(void* arg) {

    os_printf("TCP Connection successful\n");

    struct espconn* conn = (struct espconn*) arg;
    http_req_t*     req  = (http_req_t*) conn->reserve;

    espconn_regist_recvcb(conn, receive_cb);
    espconn_regist_sentcb(conn, sent_cb);

    char post_headers[32] = "";

    if (req->payload != NULL) {
        sprintf(post_headers, "Content-Length: %d\r\n", strlen(req->payload));
    }

    /* Prepare POST request */
    char buf[73 + strlen(req->path) + strlen(req->host) +
             strlen(req->headers) + strlen(post_headers)];
    int len = sprintf(buf,
                         "POST %s HTTP/1.1\r\n"
                         "Host: %s:%d\r\n"
                         "Connection: close\r\n"
                         "User-Agent: ESP8266\r\n"
                         "%s"
                         "%s"
                         "\r\n",
                        req->path, req->host, req->port, req->headers, post_headers);

    os_printf("Sending request header:\n%s\n", buf);

    espconn_sent(conn, (uint8_t *)buf, len);
    // we need them no more
    os_free(req->headers);
    req->headers = NULL;
}

static void ICACHE_FLASH_ATTR disconnect_cb(void* arg) {

    os_printf("TCP Disconnected. \n");

    struct espconn* conn = (struct espconn*) arg;
    if (conn == NULL) {
        return;
    }
    http_req_t*     req  = (http_req_t*) conn->reserve;
    if (req != NULL) {
        free(req->host);
        free(req->path);
        free(req);
    }
    espconn_delete(conn);
    if(conn->proto.tcp != NULL) {
        os_free(conn->proto.tcp);
    }
    os_free(conn);
}

static void error_cb(void* arg, sint8 err) {
    disconnect_cb(arg);
}


static void ICACHE_FLASH_ATTR dns_cb(const char *name, ip_addr_t *ipaddr, void *callback_arg) {

    http_req_t *req = (http_req_t*) callback_arg;

    if (ipaddr != NULL) {
        os_printf("DNS Successful for %s : " IPSTR "\n", name, IP2STR(ipaddr));

        /* Create a TCP connection */
        struct espconn * conn        = (struct espconn * )malloc(sizeof(struct espconn));

        conn->type                   = ESPCONN_TCP;
        conn->state                  = ESPCONN_NONE;
        conn->proto.tcp              = (esp_tcp*)  malloc(sizeof(esp_tcp));
        conn->proto.tcp->local_port  = espconn_port();
        conn->proto.tcp->remote_port = req->port;
        conn->reserve                = req;

        os_printf("Initializing TCP connection\n");
        memcpy(conn->proto.tcp->remote_ip, ipaddr, 4);

        int err = espconn_regist_connectcb(conn, connect_cb);

        espconn_regist_disconcb(conn,  disconnect_cb);
        espconn_regist_reconcb(conn,   error_cb);
        err = espconn_connect(conn);
        if (err == ESPCONN_ISCONN) {
            /* Already connected, so execute callback ourselves */
            connect_cb(conn);
        }

    } else {
        os_printf("DNS Failed for %s\n", name);

        free(req->host);
        free(req->path);
        free(req->headers);
        free(req->payload);
        free(req);
    }

}

static void ICACHE_FLASH_ATTR request(const char* host, int port, const char* path, const char* headers, const char* payload) {

    http_req_t *req = (http_req_t* ) malloc(sizeof(http_req_t));
    req->host    = strdup(host);
    req->path    = strdup(path);
    req->port    = port;
    req->headers = strdup(headers);
    req->payload = strdup(payload);

    

    ip_addr_t addr;
    /* Get IP of the host using DNS */
    err_t error = espconn_gethostbyname((struct espconn *)req, host, &addr, dns_cb);

    if (error == ESPCONN_INPROGRESS) {
        os_printf("DNS request in progress for %s\n", host);
    }
    else if (error == ESPCONN_OK) {
        dns_cb(host, &addr, req);
    }
    else {
        dns_cb(host, NULL, req);
    }

}


void HttpClient::post(const char* host, int port, const char* path, const char* headers, const char* payload) {
    request(host, port, path, headers, payload);
}

void HttpClient::init(void) {
    espconn_init();
}