#include <string.h>

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


void print_request(Request *request) {
	printf("\n\n%s", request->method);
	printf(" %s", request->document);
	printf(" %s", request->protocol);
	printf("\nHost: %s", request->host);
	printf("\nAccept: %s", request->accept);
	printf("\nAccept-Language: %s", request->language);
	printf("\nAccept-Encoding: %s", request->encoding);
	printf("\nUser-Agent: %s", request->user_agent);
}


void new_response(Response *response) {
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

	print_request(request);

	// n = getline(&(request->host), &len, stdin);
	// request->host[n - 1] = 0;

	// n = getline(&(request->accept), &len, stdin);
	// request->accept[n - 1] = 0;

	// n = getline(&(request->language), &len, stdin);
	// request->language[n - 1] = 0;

	// n = getline(&(request->encoding), &len, stdin);
	// request->encoding[n - 1] = 0;

	// n = getline(&(request->user_agent), &len, stdin);
	// request->user_agent[n - 1] = 0;
}


void build_response(Response *response) {
	response->status = "HTTP/1.1 200 OK";
	response->date = "Date: Sun, 18 Oct 2009 08:56:53 GMT";
	response->server = "Server: Apache/2.2.14 (Win32)";
	response->last_modified = "Last-Modified: Sat, 20 Nov 2019 07:16:26 GMT";
	response->etag = "ETag: \"10000000565a5-2c-3e94b66c2e680\"";
	response->accept_ranges = "Accept-Ranges: bytes";
	response->content_length = "Content-Length: 44";
	response->connection = "Connection: close";
	response->content_type = "Content-Type: text/html";
	response->body = "<html><body><h1>It works!</h1></body></html>";
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

