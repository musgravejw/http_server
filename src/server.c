#include <stdio.h>
#include "parser.c"

int main() {
	printf("Starting server...\n\n");

	Request *request = malloc(sizeof(Request));
	Response *response = malloc(sizeof(Response));

	new_request(request);
	new_response(response);

	while(1) {
		parse_request(request);

		if(strcmp(request->method, "GET") == 0) {
			// return response
			build_response(request, response);
			print_response(response);

		} else if(strcmp(request->method, "HEAD") == 0
			|| strcmp(request->method, "POST") == 0
			|| strcmp(request->method, "PUT") == 0
			|| strcmp(request->method, "DELETE") == 0
			|| strcmp(request->method, "TRACE") == 0
			|| strcmp(request->method, "OPTIONS") == 0
			|| strcmp(request->method, "PATCH") == 0) {
			
			printf("\nrequest: %s\n\n", request->method);
		} else {
			free_request(request);
			free_response(response);
		}
	}

	return -1;
}
