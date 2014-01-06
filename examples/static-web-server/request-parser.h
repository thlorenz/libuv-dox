#ifndef SWS_REQUEST_PARSER_H
#define SWS_REQUEST_PARSER_H

#include "../libuv/include/uv.h"
#include "../http-parser/http_parser.h"

typedef struct sws_req_s sws_req_t;
typedef struct sws_parse_result_s sws_parse_result_t;
typedef struct sws_parse_req_s sws_parse_req_t;
typedef void (*sws_parse_complete_cb)(sws_parse_req_t*);

void sws_req_parser_init(sws_parse_req_t* parse_req, sws_parse_complete_cb);
int sws_req_parser_execute(sws_parse_req_t* parse_req, char* buf, ssize_t nread);
void sws_cleanup_parse_req(sws_parse_req_t* parse_req);
char* sws_req_parser_result_str(sws_parse_result_t* result);

#define MAX_HEADER_VALUE 500

/* In order to decouple this parser from libuv, we could remove the handle and write req.
 * In that case the user would have to attach these, i.e. to a void* data.
 */
#define SWS_REQ_FIELDS        \
  uv_tcp_t handle;            \
  uv_write_t write_req;       \
  int id;

struct sws_header_line_s {
  char *field;
  size_t field_len;
  char *value;
  size_t value_len;
};

#define SWS_PARSE_RESULT_FIELDS           \
  char *url;                              \
  char user_agent[MAX_HEADER_VALUE];      \
  char host[MAX_HEADER_VALUE];            \
  char accept[MAX_HEADER_VALUE];          \
  char accept_encoding[MAX_HEADER_VALUE]; \
  char accept_language[MAX_HEADER_VALUE]; \
  enum http_method method;                \
  /* private */                           \
  struct sws_header_line_s header_line;   \

// TODO: make url be allocated and ensure that header_line is freed for every value
//       or implement cleanup method
struct sws_req_s {
  SWS_REQ_FIELDS
};

/* extends sws_req_s */
struct sws_parse_result_s {
  SWS_REQ_FIELDS
  SWS_PARSE_RESULT_FIELDS
};


/* extends sws_parse_result_s */
struct sws_parse_req_s {
  SWS_REQ_FIELDS
  SWS_PARSE_RESULT_FIELDS
  http_parser parser;
  sws_parse_complete_cb on_parse_complete;
};
#endif
