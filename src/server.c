#include <stdio.h>
#include "parser.c"

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


int main() {
	printf("Starting server...\n\n");

	Request *request = malloc(sizeof(Request));
	Response *response = malloc(sizeof(Response));

	new_request(request);
	new_response(response);

	while(1) {
		parse_request(request);

		return 0;

		if(strcmp(request->method, "GET") == 0) {
			// do stuff with request
			printf("\nrequest method: %s\n\n", request->method);

			// return resposne
			build_response(response);

			printf("%s\n", response->body);
		} else if(strcmp(request->method, "HEAD") == 0
			|| strcmp(request->method, "POST") == 0
			|| strcmp(request->method, "PUT") == 0
			|| strcmp(request->method, "DELETE") == 0
			|| strcmp(request->method, "TRACE") == 0
			|| strcmp(request->method, "OPTIONS") == 0
			|| strcmp(request->method, "PATCH") == 0) {
			
			printf("\nrequest: %s\n\n", request->method);
			// printf("\nresponse: %s\n\n". response->buffer);
		} else {
			free_request(request);
			free_response(response);
		}
	}

	return -1;
}