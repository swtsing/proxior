#ifndef _HTTP_H_
#define _HTTP_H_

#include "common.h"
#include "logging.h"
#include "config.h"

extern conf *config;

typedef struct conn
{
  struct bufferevent *be_client, *be_server;
  char url[MAX_URL_LEN];
  struct parsed_url *purl;
  char method[10];
  char version[10];

  // type of service
  enum {
    DIRECT = 0x01,
    HTTP_PROXY = 0x02,
    SOCKS_PROXY = 0x03,
    CONFIG = 0x08
  } tos;

  /* proxy to use, NULL for direct */
  struct proxy_t *proxy;

  /* response headline */
  short headline;

  // For CONNECT method
  short handshaked;

  /* When connected to a new host via socks5, if there is data not sent yet, send it */
  short not_sent_yet;

  /*  track current http state */
  struct state *state; 
} conn_t;


struct state {
  /* for content section */
  int length, read; 
  int is_cont;
  
  /* end of request header */
  int eor;

  struct evbuffer *header;
  struct evbuffer *header_b; // for trying on failure
  struct evbuffer *cont;
};

void
http_ready_cb(int (*callback)(void *ctx), void *ctx);

void
read_server(struct bufferevent *bev, void *ctx);

void 
server_event(struct bufferevent *bev, short e, void *ptr);

void start();

#endif /* _HTTP_H_ */
