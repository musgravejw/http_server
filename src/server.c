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

#include <stdio.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "parser.c"

int main() {
	printf("Starting server...\n\n");

	struct sockaddr_in server_address;
	struct protoent *protocol = getprotobyname("tcp");
	int sock = socket(AF_INET, SOCK_STREAM, protocol->p_proto);

	if (protocol == -1 || sock == -1) return -1;

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) return -1;

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(80);

	if (bind(sock, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) return -1;

	if (listen(sock, 5) == -1) return -1;

	printf("Listening on 127.0.0.1:80 ...\n\n");

	while(1) {
		Request *request = malloc(sizeof(Request));
		Response *response = malloc(sizeof(Response));

		new_request(request);
		new_response(response);

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
