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
#include <fcntl.h>

#include "parser.c"


#define CLIENT_COUNT 10000

struct client {
  struct sockaddr_in address;
  socklen_t addr_len;
  int socket;
};


int main() {
  struct sockaddr_in server_address;
  struct protoent *protocol;
  int server_socket;
  int enable = 1;
  ssize_t bytes_read;
  char *buffer;
  struct client clients[CLIENT_COUNT] = { 0 };
  size_t client_count = 0;

  printf("\nStarting server...");

  // open socket on host
  protocol = getprotobyname("tcp");
  server_socket = socket(AF_INET, SOCK_STREAM, protocol->p_proto);

  if (protocol == -1 || server_socket == -1) return -1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, 
  	             &enable, sizeof(enable)) < 0) return -1;

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(80);

  if (bind(server_socket, (struct sockaddr*) &server_address, 
  	       sizeof(server_address)) == -1) return -1;
  if (listen(server_socket, 5) == -1) return -1;

  printf("\nListening on port 80...\n");

  buffer = malloc(1028);

  // listen on port 80
  while(1) {
    if (client_count < sizeof clients / sizeof *clients) {
      struct sockaddr_in client_address;
      socklen_t client_socklen = sizeof(client_address);
      int client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_socklen);

      if (client_socket != -1) {
        fcntl(client_socket, F_SETFL, fcntl(client_socket, F_GETFL, 0) | O_NONBLOCK);
        clients[client_count++] = (struct client) { .address = client_address, .addr_len = client_socklen, .socket = client_socket };

        printf("\nNew client connection: %d", client_count);                                                         
      }
    }

    for (int i = 0; i < client_count; i++) {
      // ssize_t bytes_read = read(clients[i].socket, buffer, 1028);
      // printf("\nReceived: %s", buffer);

      Request *request = malloc(sizeof(Request));
      Response *response = malloc(sizeof(Response));

      new_request(request);
      new_response(response);

      // struct sockaddr_in client_address;
      // socklen_t client_socklen = sizeof(client_address);
      // int client_socket = accept(server_socket, (struct sockaddr*) 
      //                          &client_address, &client_socklen);

      // read into buffer
      while(read(clients[i].socket, buffer, 1028) > 0) {
        // printf("\nReceived:\n%s\n", buffer);

        parse_request(request, buffer);
        log_request(request);

        if (strcmp(request->method, "GET") == 0) {
          build_response(request, response);
          log_response(response);

          char *res = response_to_string(response);
          printf("\nSending: %s\n", res);

          // return response
        int n = write(clients[i].socket, res, 1028);

        if (n < 0) {
          printf("Error writing to socket.");
        } else if(strcmp(request->method, "HEAD") == 0
            || strcmp(request->method, "POST") == 0
            || strcmp(request->method, "PUT") == 0
            || strcmp(request->method, "DELETE") == 0
            || strcmp(request->method, "TRACE") == 0
            || strcmp(request->method, "OPTIONS") == 0
            || strcmp(request->method, "PATCH") == 0) {
          printf("\nrequest: %s\n\n", request->method);
        } else {
          // free_request(request);
          // free_response(response);
        }
      }

      if (bytes_read == 0) {
        close(clients[i].socket);
        client_count--;
        memmove(clients + i, clients + i + 1, (client_count - i) * sizeof clients);
        continue;
      }
    }

      sleep(0);
    }
  }

  return -1;
}
