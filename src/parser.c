const int LINE_SIZE = 60;

// rename request buffer
typedef struct {
	char *method;
	char *host;
	char *accept;
	char *language;
	char *encoding;
	char *user_agent;
} RequestBuffer;


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
} ResponseBuffer;


void new_request(RequestBuffer *request) {
	request->method = malloc(sizeof(LINE_SIZE));
	request->host = malloc(sizeof(LINE_SIZE));
	request->accept = malloc(sizeof(LINE_SIZE));
	request->language = malloc(sizeof(LINE_SIZE));
	request->encoding = malloc(sizeof(LINE_SIZE));
	request->user_agent = malloc(sizeof(LINE_SIZE));
}


void new_response(ResponseBuffer *response) {
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


void parse_request(RequestBuffer *request) {
	ssize_t len;
	ssize_t n = getline(&(request->method), &len, stdin);
	request->method[n - 1] = 0;

	n = getline(&(request->host), &len, stdin);
	request->host[n - 1] = 0;

	n = getline(&(request->accept), &len, stdin);
	request->accept[n - 1] = 0;

	n = getline(&(request->language), &len, stdin);
	request->language[n - 1] = 0;

	n = getline(&(request->encoding), &len, stdin);
	request->encoding[n - 1] = 0;

	n = getline(&(request->user_agent), &len, stdin);
	request->user_agent[n - 1] = 0;
}


void build_response(ResponseBuffer *response) {
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
