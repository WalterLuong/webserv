#include <algorithm>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#define PORT 8080
#define MAX_EVENTS 32

/**
 * It sets up the server socket and binds it to the port
 *
 * @param server_fd This is the file descriptor of the socket created by the socket() function.
 * @param address This is the address of the server.
 */
void setupserv(int &server_fd, struct sockaddr_in &address)
{

	int opt = 1;

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	};
};

/**
 * It adds a file descriptor to the epoll instance
 *
 * @param epfd the epoll file descriptor
 * @param fd The file descriptor to add to the epoll instance.
 * @param events The events we want to monitor for the file descriptor fd.
 */
void epoll_ctl_add(int epfd, int fd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
	{
		perror("epoll_ctl()\n");
		exit(1);
	}
}

/**
 * It sets the socket to non-blocking mode
 *
 * @param sockfd The socket file descriptor to set to non-blocking.
 */
void nonblockingfd(int sockfd)
{
	int flags;

	flags = fcntl(sockfd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(sockfd, F_SETFL, flags);
}
/**
 * It reads data from the socket, prints it to the screen, and then writes it back to the socket
 *
 * @param event the event that triggered the callback
 */

void readdata(struct epoll_event &event)
{
	char buf[1024];

	while (true)
	{
		memset(buf, 0, sizeof(buf));
		int n = read(event.data.fd, buf, sizeof(buf));
		if (n <= 0)
			break;
		else
		{
			printf("[+] data: %s\n", buf);
			write(event.data.fd, buf, strlen(buf));
		}
	};
};

/**
 * It accepts a new connection, sets the socket to non-blocking, and adds it to the epoll instance
 *
 * @param server_fd the server socket file descriptor
 * @param cli_addr the client's address
 * @param epfd the epoll file descriptor
 * @param socklen the length of the client address
 */
void addconnexion(int server_fd, struct sockaddr_in cli_addr, int epfd, unsigned int socklen)
{
	char buf[1024];

	int conn_sock = accept(server_fd, (struct sockaddr *)&cli_addr, &socklen);
	inet_ntop(AF_INET, (char *)&(cli_addr.sin_addr), buf, sizeof(cli_addr));
	printf("[+] connected with %s:%d\n", buf, ntohs(cli_addr.sin_port));
	nonblockingfd(conn_sock);
	epoll_ctl_add(epfd, conn_sock, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
};

/**
 * It closes the connection and removes the file descriptor from the epoll instance
 *
 * @param epfd the epoll file descriptor
 * @param event the event that triggered the callback
 */
void closeconnexion(int epfd, struct epoll_event &event)
{
	epoll_ctl(epfd, EPOLL_CTL_DEL, event.data.fd, NULL);
	printf("[+] connection closed\n");
	close(event.data.fd);
}

/**
 * It waits for events on the epoll file descriptor, and when it receives one, it checks if it's a
 * new connection, if it's data to read, or if it's a closed connection
 *
 * @param epfd the epoll file descriptor
 * @param server_fd the server's socket file descriptor
 */
void serv_running(int epfd, int server_fd)
{
	struct epoll_event events[MAX_EVENTS];
	struct sockaddr_in cli_addr;
	unsigned int socklen = sizeof(cli_addr);

	while (true)
	{
		int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		for (int i = 0; i < nfds; i++)
		{
			if (events[i].data.fd == server_fd)
				addconnexion(server_fd, cli_addr, epfd, socklen);
			else if (events[i].events & EPOLLIN)
				readdata(events[i]);
			else
				printf("[+] unexpected\n");
			if (events[i].events & (EPOLLRDHUP | EPOLLHUP))
				closeconnexion(epfd, events[i]);
		}
	}
}
int main(int argc, char const *argv[])
{
	struct sockaddr_in srv_addr;
	int server_fd;
	struct sockaddr_in address;
	int epfd;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	setupserv(server_fd, address);
	epfd = epoll_create(1);
	epoll_ctl_add(epfd, server_fd, EPOLLIN | EPOLLOUT | EPOLLET);

	serv_running(epfd, server_fd);

	close(server_fd);
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}