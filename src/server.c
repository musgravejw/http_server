#include <stdio.h>

// rename request buffer
typedef struct {
	char *buffer;
	size_t buffer_length;
	ssize_t input_length;
} RequestBuffer;


typedef struct {
	char *buffer;
	size_t buffer_length;
	ssize_t input_length;
} ResponseBuffer;


/*
Request

GET /docs/index.html HTTP/1.1
Host: www.nowhere123.com
Accept: image/gif, image/jpeg, *\/*
Accept-Language: en-us
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)
(blank line)
*/


/*
Response

HTTP/1.1 200 OK
Date: Sun, 18 Oct 2009 08:56:53 GMT
Server: Apache/2.2.14 (Win32)
Last-Modified: Sat, 20 Nov 2004 07:16:26 GMT
ETag: "10000000565a5-2c-3e94b66c2e680"
Accept-Ranges: bytes
Content-Length: 44
Connection: close
Content-Type: text/html
X-Pad: avoid browser bug
  
<html><body><h1>It works!</h1></body></html>
*/


int main() {
	printf("Starting server...\n\n");

	RequestBuffer *request = malloc(sizeof(RequestBuffer));
	ResponseBuffer *response = malloc(sizeof(ResponseBuffer));

	request->buffer = NULL;
	request->buffer_length = 0;
	request->input_length = 0;

	response->buffer = NULL;
	response->buffer_length = 0;
	response->input_length = 0;

	while(1) {
		ssize_t n = getline(&(request->buffer), &(request->buffer_length), stdin);

		if (n <= 0) {
			printf("Error reading request\n");
			return -1;
		}

		request->input_length = n - 1;
		request->buffer[n - 1] = 0;

		if(strcmp(request->buffer, "GET") == 0 
			|| strcmp(request->buffer, "HEAD") == 0
			|| strcmp(request->buffer, "POST") == 0
			|| strcmp(request->buffer, "PUT") == 0
			|| strcmp(request->buffer, "DELETE") == 0
			|| strcmp(request->buffer, "TRACE") == 0
			|| strcmp(request->buffer, "OPTIONS") == 0
			|| strcmp(request->buffer, "PATCH") == 0) {
			
			printf("\nrequest: %s\n\n", request->buffer);
			// printf("\nresponse: %s\n\n". response->buffer);
		} else {
			free(request->buffer);
			free(request);
			free(response->buffer);
			free(response);

			return 0;
		}

		// do stuff with request

		// return resposne
	}

	return 0;
}