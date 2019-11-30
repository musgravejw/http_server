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
	response->body = malloc(sizeof(LINE_SIZE));
}


void new_token(Token *token) {
	token->token_class = malloc(sizeof(LINE_SIZE));
	token->lexeme = malloc(sizeof(LINE_SIZE));
}


void print_request(Request *request) {
	printf("\n\n%s", request->method);
	printf(" %s", request->document);
	printf(" %s", request->protocol);
	printf("\nHost: %s", request->host);
	printf("\nAccept: %s", request->accept);
	printf("\nAccept-Language: %s", request->language);
	printf("\nAccept-Encoding: %s", request->encoding);
	printf("\nUser-Agent: %s", request->user_agent);
	printf("\n");
}


void print_response(Response *response) {
	printf("\n\n%s %s", response->protocol, response->status);
	printf("\nLast-Modified: %s", response->last_modified);
	printf("\nETag: \"%s\"", response->etag);
	printf("\nAccept-Ranges: %s", response->accept_ranges);
	printf("\nContent-Length: %s", response->content_length);
	printf("\nConnection: %s", response->connection);
	printf("\nContent-Type: %s", response->content_type);
	printf("\n%s", response->body);
}


Token* next_token() {
	Token *token = malloc(sizeof(Token));
	char *str = malloc(sizeof(LINE_SIZE));
	char c;
	int i = 0;

	new_token(token);

	while(1) {
		c = getchar();
		
		if (c == ' ' || c == '\n') {
			str[i] = '\0';
			token->lexeme = str;

			return token;
		}

		str[i++] = c;
	}
}


// GET /docs/index.html HTTP/1.1
void parse_request(Request *request) {
	request->method = (next_token())->lexeme;
	request->document = (next_token())->lexeme;
	request->protocol = (next_token())->lexeme;

	// host
	(next_token())->lexeme;
	request->host = (next_token())->lexeme;

	// accept
	(next_token())->lexeme;
	request->accept = (next_token())->lexeme;

	// language
	(next_token())->lexeme;
	request->language = (next_token())->lexeme;

	// encoding
	(next_token())->lexeme;
	request->encoding = (next_token())->lexeme;

	// user agent
	(next_token())->lexeme;
	request->user_agent = (next_token())->lexeme;

	// blank line
	(next_token())->lexeme;
	(next_token())->lexeme;
}


void build_response_body(Request *request, 
						 Response *response) {
	FILE *fp;
	char *filename = malloc(sizeof(LINE_SIZE)); // Memory Leak
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
	response->date = "Date: Sun, 18 Oct 2009 08:56:53 GMT";
	response->server = "Server: Apache/2.2.14 (Win32)";
	response->last_modified = "Sat, 20 Nov 2019 07:16:26 GMT";
	response->etag = "10000000565a5-2c-3e94b66c2e680";
	response->accept_ranges = "bytes";
	response->content_length = "44";
	response->connection = "close";
	response->content_type = "text/html";

	build_response_body(request, response);
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

