/* HTTP Server
 *
 * Copyright (c) 2019, John Musgrave <musgrajw at uc dot mail dot edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of this project nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include <stdlib.h>

const int TOKEN_SIZE = 60;
const int LINE_SIZE = 255;

typedef struct {
  char *accept;
  char *cache_control;
  char *connection;
  char *document;
  char *encoding;
  char *host;
  char *language;
  char *method;
  char *protocol;
  char *upgrade_insecure;
  char *user_agent;
} Request;


typedef struct {
  char *accept_ranges;
  char *body;
  char *content_length;
  char *content_type;
  char *connection;
  char *date;
  char *etag;
  char *last_modified;
  char *protocol;
  char *server;
  char *status;
} Response;


typedef struct {
	char *lexeme;
  char *token_class;
} Token;


void new_request(Request *request) {
  request->method = malloc(sizeof(TOKEN_SIZE));
  request->document = malloc(sizeof(LINE_SIZE));
  request->protocol = malloc(sizeof(TOKEN_SIZE));
  request->host = malloc(sizeof(TOKEN_SIZE));
  request->accept = malloc(sizeof(LINE_SIZE));
  request->language = malloc(sizeof(TOKEN_SIZE));
  request->encoding = malloc(sizeof(TOKEN_SIZE));
  request->user_agent = malloc(sizeof(LINE_SIZE));
  request->connection = malloc(sizeof(TOKEN_SIZE));
  request->cache_control = malloc(sizeof(TOKEN_SIZE));
  request->upgrade_insecure = malloc(sizeof(TOKEN_SIZE));
}


void new_response(Response *response) {
  response->protocol = malloc(sizeof(TOKEN_SIZE));
  response->status = malloc(sizeof(TOKEN_SIZE));
  response->date = malloc(sizeof(TOKEN_SIZE));
  response->server = malloc(sizeof(TOKEN_SIZE));
  response->last_modified = malloc(sizeof(TOKEN_SIZE));
  response->etag = malloc(sizeof(TOKEN_SIZE));
  response->accept_ranges = malloc(sizeof(TOKEN_SIZE));
  response->content_length = malloc(sizeof(TOKEN_SIZE));
  response->connection = malloc(sizeof(TOKEN_SIZE));
  response->content_type = malloc(sizeof(TOKEN_SIZE));
  response->body = malloc(512);
}


void new_token(Token *token) {
  token->token_class = malloc(sizeof(TOKEN_SIZE));
  token->lexeme = malloc(sizeof(LINE_SIZE));
}


void log_request(Request *request) {
  printf("\nRequest:\n%s", request->method);
  printf(" %s", request->document);
  printf(" %s", request->protocol);
  printf("\nHost: %s", request->host);
  printf("\nAccept: %s", request->accept);
  printf("\nAccept-Language: %s", request->language);
  printf("\nAccept-Encoding: %s", request->encoding);
  printf("\nUser-Agent: %s", request->user_agent);
  printf("\n\n");
}


void log_response(Response *response) {
  printf("\nResponse:\n%s %s", response->protocol, response->status);
  printf("\nLast-Modified: %s", response->last_modified);
  printf("\nETag: \"%s\"", response->etag);
  printf("\nAccept-Ranges: %s", response->accept_ranges);
  printf("\nContent-Length: %s", response->content_length);
  printf("\nConnection: %s", response->connection);
  printf("\nContent-Type: %s", response->content_type);
  printf("\n%s", response->body);
}


Token* next_token(char *buffer) {
  Token *token = malloc(sizeof(Token));
  char *str = malloc(sizeof(LINE_SIZE));
  int i = 0;

  new_token(token);

  while(1) {
    if (buffer[i] == '\0') return token;

    if (buffer[i] == ' ' || buffer[i] == '\n') {
      str[i] = '\0';
      token->lexeme = str;

      return token;
    }

    str[i++] = buffer[i];
  }
}


Token* next_line(char *buffer) {
  Token *token = malloc(sizeof(Token));
  char *str = malloc(sizeof(LINE_SIZE));
  int i = 0;

  new_token(token);

  while(1) {
    if (buffer[i] == '\0') return token;

    if (buffer[i] == '\n') {
      str[i] = '\0';
      token->lexeme = str;

      return token;
    }

    str[i++] = buffer[i];
  }
}


void parse_request(Request *request, 
				   char *buffer) {
  request->method = (next_token(buffer))->lexeme;
  buffer += strlen(request->method) + 1;

  request->document = (next_token(buffer))->lexeme;
  buffer += strlen(request->document) + 1;
  request->protocol = (next_token(buffer))->lexeme;
  buffer += strlen("HTTP/1.1 ") + 1;

  // host
  buffer += strlen("Host: ");
  request->host = (next_token(buffer))->lexeme;
  buffer += strlen(request->host) + 1;

  // connection
  buffer += strlen("Connection: ");
  request->connection = (next_token(buffer))->lexeme;
  buffer += strlen(request->connection) + 1;

  // cache control
  buffer += strlen("Cache-Control: ");
  request->cache_control = (next_token(buffer))->lexeme;
  buffer += strlen(request->cache_control) + 1;

  // Upgrade Insecure
  buffer += strlen("Upgrade-Insecure-Requests: ");
  request->upgrade_insecure = (next_token(buffer))->lexeme;
  buffer += strlen(request->upgrade_insecure) + 1;

  // user agent
  buffer += strlen("User-Agent: ");
  request->user_agent = (next_line(buffer))->lexeme;
  buffer += strlen(request->user_agent) + 1;

  // accept
  buffer += strlen("Accept: ");
  request->accept = (next_line(buffer))->lexeme;
  buffer += strlen(request->accept) + 1;

  // Set-Fetch-Site
  buffer += strlen("Set-Fetch-Site: ");
  request->upgrade_insecure = (next_token(buffer))->lexeme;
  buffer += strlen(request->upgrade_insecure) + 1;

  // Set-Fetch-Mode
  buffer += strlen("Set-Fetch-Mode: ");
  request->upgrade_insecure = (next_token(buffer))->lexeme;
  buffer += strlen(request->upgrade_insecure) + 1;

  // Set-Fetch-User
  buffer += strlen("Set-Fetch-User: ");
  request->upgrade_insecure = (next_token(buffer))->lexeme;
  buffer += strlen(request->upgrade_insecure) + 1;

  // Set-Fetch-Dest
  buffer += strlen("Set-Fetch-Dest: ");
  request->upgrade_insecure = (next_token(buffer))->lexeme;
  buffer += strlen(request->upgrade_insecure) + 1;

  // encoding
  buffer += strlen("Accept-Encoding: ");
  request->encoding = (next_line(buffer))->lexeme;
  buffer += strlen(request->encoding) + 1;

  // language
  buffer += strlen("Accept-Language: ");
  request->language = (next_line(buffer))->lexeme;
  buffer += strlen(request->language) + 1;

  // blank line
  next_token(buffer);
  next_token(buffer);
}


void build_response_body(Request *request, 
						 Response *response) {
  FILE *fp;
  char *filename = malloc(sizeof(TOKEN_SIZE));
  char c;
  int i = 0;
  int j = 0;

  filename[i++] = '.';
  filename[i++] = '/';
  filename[i++] = 'w';
  filename[i++] = 'w';
  filename[i++] = 'w';
  filename[i++] = '/';

  if (strlen(request->document) < 2) 
    request->document = "index.html";

  while(request->document[j] != '\0') filename[i++] = request->document[j++];

  printf("%s\n", request->document);

  i = 0;
  fp = fopen(filename, "r");

  while(c != EOF) {
    c = fgetc(fp);
    response->body[i++] = c;
  }

  fclose(fp);
  response->body[i] = '\0';
}


void build_response(Request *request,
				    Response *response) {
  // placeholder header values
  response->protocol = "HTTP/1.1";
  response->status = "200 OK";
  response->date = "Date: Mon, 3 Aug 2020 10:56:53 EST";
  response->server = "Server: Apache/2.2.14 (Win32)";
  response->last_modified = "Sat, 20 Nov 2019 07:16:26 GMT";
  response->etag = "10000000565a5-2c-3e94b66c2e680";
  response->accept_ranges = "bytes";
  response->connection = "close";
  response->content_type = "text/html";

  build_response_body(request, response);

  sprintf(response->content_length, "%d", sizeof(response->body) * 8);
}


char *response_to_string(Response *response) {
  char *res = malloc(1028);

  sprintf(res, "\n\n%s %s \n\
Last-Modified: %s \n\
ETag: \"%s\" \n\
Accept-Ranges: %s \n\
Content-Length: %s \n\
Connection: %s \n\
Content-Type: %s \n\
\n\
%s \n", 
    response->protocol, 
    response->status, 
    response->last_modified, 
    response->etag, 
    response->accept_ranges, 
    response->content_length, 
    response->connection, 
    response->content_type, 
    response->body);

  return res;
}


void free_request(Request *request) {
  free(request->method);
  free(request->host);
  free(request->accept);
  free(request->language);
  free(request->encoding);
  free(request->connection);
  free(request->cache_control);
  free(request->upgrade_insecure);
  free(request->user_agent);
}


void free_response(Response *response) {
  free(response->protocol);
  free(response->status);
  free(response->date);
  free(response->server);
  free(response->last_modified);
  free(response->etag);
  free(response->accept_ranges);
  free(response->content_length);
  free(response->connection);
  free(response->content_type);
  free(response->body);
}
