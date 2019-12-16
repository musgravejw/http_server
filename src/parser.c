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

const int LINE_SIZE = 60;

typedef struct {
	char *method;
	char *document;
	char *protocol;
	char *host;
	char *accept;
	char *language;
	char *encoding;
	char *user_agent;
} Request;


typedef struct {
	char *protocol;
	char *status;
	char *date;
	char *server;
	char *last_modified;
	char *etag;
	char *accept_ranges;
	char *content_length;
	char *connection;
	char *content_type;
	char *body;
} Response;


typedef struct {
	char *token_class;
	char *lexeme;
} Token;


void new_request(Request *request) {
	request->method = malloc(sizeof(LINE_SIZE));
	request->document = malloc(sizeof(LINE_SIZE));
	request->protocol = malloc(sizeof(LINE_SIZE));
	request->host = malloc(sizeof(LINE_SIZE));
	request->accept = malloc(sizeof(LINE_SIZE));
	request->language = malloc(sizeof(LINE_SIZE));
	request->encoding = malloc(sizeof(LINE_SIZE));
	request->user_agent = malloc(sizeof(LINE_SIZE));
}


void new_response(Response *response) {
	response->protocol = malloc(sizeof(LINE_SIZE));
	response->status = malloc(sizeof(LINE_SIZE));
	response->date = malloc(sizeof(LINE_SIZE));
	response->server = malloc(sizeof(LINE_SIZE));
	response->last_modified = malloc(sizeof(LINE_SIZE));
	response->etag = malloc(sizeof(LINE_SIZE));
	response->accept_ranges = malloc(sizeof(LINE_SIZE));
	response->content_length = malloc(sizeof(LINE_SIZE));
	response->connection = malloc(sizeof(LINE_SIZE));
	response->content_type = malloc(sizeof(LINE_SIZE));
	response->body = malloc(512);
}


void new_token(Token *token) {
	token->token_class = malloc(sizeof(LINE_SIZE));
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
	// buffer += strlen(request->host) + 1;

	// accept
	// next_token(buffer)
	// request->accept = (next_token(buffer))->lexeme;

	// // language
	// next_token(buffer)
	// request->language = (next_token(buffer))->lexeme;

	// // encoding
	// next_token(buffer)
	// request->encoding = (next_token(buffer))->lexeme;

	// // user agent
	// next_token(buffer)
	// request->user_agent = (next_token(buffer))->lexeme;

	// // blank line
	// next_token(buffer)
	// next_token(buffer)
}


void build_response_body(Request *request, 
						 Response *response) {
	FILE *fp;
	char *filename = malloc(sizeof(LINE_SIZE));
	char c;
	int i = 0;
	int j = 0;

	filename[i++] = '.';
	filename[i++] = '/';
	filename[i++] = 'w';
	filename[i++] = 'w';
	filename[i++] = 'w';
	filename[i++] = '/';

	while (request->document[j] != '\0')
		filename[i++] = request->document[j++];

	i = 0;
	fp = fopen(filename, "r");

	while (c != EOF) {
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
	response->date = "Date: Sat, 14 Dec 2019 10:56:53 EST";
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

	sprintf(res, "\n\n%s %s\nLast-Modified: %s\nETag: \"%s\"\nAccept-Ranges: %s\nContent-Length: %s\nConnection: %s\nContent-Type: %s\n%s", 
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
