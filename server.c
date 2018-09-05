/*
 *  This code is pretty much the threads example from http://www.martinbroadhurst.com/server-examples.html
 *  with the barest minimumest of changes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>

#define PORT    "8080" /* Port to listen on */
#define BACKLOG     10  /* Passed to listen() */

#define RESPONSE "HTTP/1.1 200 OK\ncontent-type: text/plain; charset=utf-8\nserver: golo\nDate: Wed, 05 Sep 2018 11:03:48 GMT\nContent-Length: 12\n\nHello World!"

void *handle(int *pnewsock)
{
  if (*pnewsock < 0) {
    perror("ERROR on accept");

    return NULL;
  }

  char buffer[256];
  bzero(buffer, 256);

  if (read(*pnewsock, buffer, 255) < 0)
    perror("ERROR reading from socket");

  free(buffer);

  if (write(*pnewsock, RESPONSE, strlen(RESPONSE)) < 0)
    perror("ERROR writing to socket");

  if (close(*pnewsock) < 0)
    perror("close")

  free(pnewsock);

  return NULL;
}

int main(void)
{
    int sock;
    pthread_t thread;
    struct addrinfo hints, *res;
    int reuseaddr = 1; /* True */

    /* Get the address info */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_addr = INADDR_ANY;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    /* Create the socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    /* Enable the socket to reuse the address */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        return 1;
    }

    /* Bind to the address */
    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        return 0;
    }

    char *s = NULL;
    switch(res->ai_family) {
    case AF_INET: {
      struct sockaddr_in *addr_in = (struct sockaddr_in *)res;
      s = malloc(INET_ADDRSTRLEN);
      inet_ntop(AF_INET, &(addr_in->sin_addr), s, INET_ADDRSTRLEN);
      break;
    }
    case AF_INET6: {
      struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)res;
      s = malloc(INET6_ADDRSTRLEN);
      inet_ntop(AF_INET6, &(addr_in6->sin6_addr), s, INET6_ADDRSTRLEN);
      break;
    }
    default:
      break;
    }
    printf("Listening on: %s\n", s);
    free(s);

    freeaddrinfo(res);

    /* Listen */
    if (listen(sock, BACKLOG) == -1) {
        perror("listen");
        return 0;
    }

    /* Main loop */
    while (1) {
        socklen_t size = sizeof(struct sockaddr_in);
        struct sockaddr_in their_addr;
        int newsock = accept(sock, (struct sockaddr*)&their_addr, &size);
        if (newsock == -1) {
            perror("accept");
        }
        else {
            /* Make a safe copy of newsock */
            int *safesock = malloc(sizeof(int));
            if (safesock) {
                *safesock = newsock;
                if (pthread_create(&thread, NULL, handle, safesock) != 0) {
                    fprintf(stderr, "Failed to create thread\n");
                }
            }
            else {
                perror("malloc");
            }
        }
    }

    close(sock);

    return 0;
}
