#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dgb.h"

#include "sws-request-parser.h"
#include "../libuv/include/uv.h"
#include "../http-parser/http_parser.h"

static char* strslice(const char* s, size_t len);

static int on_message_begin    ( http_parser* parser                                 );
static int on_url              ( http_parser* parser, const char* hdr, size_t length );
static int on_header_field     ( http_parser* parser, const char* hdr, size_t length );
static int on_header_value     ( http_parser* parser, const char* hdr, size_t length );
static int on_headers_complete ( http_parser* parser                                 );
static int on_message_complete ( http_parser* parser                                 );

// parser settings shared for each req
static http_parser_settings parser_settings = {
    .on_message_begin    =  on_message_begin
  , .on_url              =  on_url
  , .on_header_field     =  on_header_field
  , .on_header_value     =  on_header_value
  , .on_headers_complete =  on_headers_complete
  , .on_message_complete =  on_message_complete
};

/* Initializes *only* the parser of the passed request.
 *
 * The following pointers are initialized:
 *  - parse_req->parser.data == parse_req
 */
void sws_req_parser_init(sws_parse_req_t* parse_req, sws_parse_complete_cb on_parse_complete) {
  http_parser_init(&parse_req->parser, HTTP_REQUEST);
  parse_req->parser.data = parse_req;
  parse_req->on_parse_complete = on_parse_complete;
}

int sws_req_parser_execute(sws_parse_req_t * parse_req, char* buf, ssize_t nread) {
  return http_parser_execute(&parse_req->parser, &parser_settings, buf, nread);
}

static char* strslice(const char* s, size_t len) {
  char *slice = (char*) malloc(sizeof(char) * (len + 1));
  strncpy(slice, s, len);
  slice[len] = '\0';
  return slice;
}

static int on_message_begin(http_parser* parser) {
  sws_req_t *req = (sws_req_t*) parser->data;
  dbg("[ %3d ] * message begin\n", req->id);
  return 0;
}

static int on_url(http_parser* parser, const char* hdr, size_t length) {
  sws_parse_req_t *req = (sws_parse_req_t*) parser->data;
  req->url = strslice(hdr, length);
  dbg("[ %3d ] url: %s\n", req->id, req->url);
  return 0;
}

static int on_header_field(http_parser* parser, const char* hdr, size_t length) {
  sws_parse_req_t *req = (sws_parse_req_t*) parser->data;

  req->header_line.field = strslice(hdr, length);
  req->header_line.field_len = length;

  dbg("[ %3d ] %s: ", req->id, req->header_line.field);
  return 0;
}

static int on_header_value(http_parser* parser, const char* hdr, size_t length) {
  sws_parse_req_t *req = (sws_parse_req_t*) parser->data;

  assert(req->header_line.field_len > 0);
  assert(req->header_line.field);

  req->header_line.value = strslice(hdr, length);
  req->header_line.value_len = length;

  // TODO: process headerline
  dbg("%s\n", req->header_line.value);

  free(req->header_line.field);
  req->header_line.field = NULL;

  req->header_line.field_len = 0;

  return 0;
}

static int on_headers_complete(http_parser* parser) {
  // parser->data was pointed to the req struct in on_connect
  sws_req_t *req = (sws_req_t*) parser->data;
  dbg("[ %3d ] * headers complete\n", req->id);

  // signal that there won't be a body by returning 1
  // we don't support anything but HEAD and GET since we are just a static webserver
  return 1;
}

static int on_message_complete(http_parser* parser) {
  sws_parse_req_t *req = (sws_parse_req_t*) parser->data;
  dbg("[ %3d ] * message complete\n", req->id);

  if (req->on_parse_complete) {
    req->on_parse_complete((sws_parse_result_t*)req);
  }

  return 0;
}
