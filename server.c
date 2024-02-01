#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

#define QUEUE_DEFAULT_NUM 5
#define BUF_MAX_LEN 4096

int main(void) {
	int s_fd;

	s_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (s_fd == -1)
	{
		fprintf(stderr, "Error: cannot create socket\n"); 
		return -1;
	}

	int binding;
	struct sockaddr_un s_addr;
 
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sun_family = AF_UNIX;
	strcpy(s_addr.sun_path, "./test");
	binding = bind(s_fd, (const struct sockaddr*)&s_addr, sizeof(s_addr));
	if (binding == -1)
	{
		fprintf(stderr, "Error: cannot bind socket\n");
		close(s_fd);
		return -1;
	}	

	int listening;

	listening = listen(s_fd, QUEUE_DEFAULT_NUM);
	if (listening == -1)
	{
		fprintf(stderr, "Error: cannont listen\n");
		close(s_fd);
		return -1;
	}

	int c_fd;
	int c_addr_size;
	struct sockaddr_un c_addr;

	c_addr_size = sizeof(c_addr);
	c_fd = accept(s_fd, (struct sockaddr*)&c_addr, &c_addr_size);
	if (c_fd == -1)
	{
		fprintf(stderr, "Error: cannot accept\n");
		close(s_fd);
		return -1;
	}

	char buf[BUF_MAX_LEN];
	int written = 0;
	int ret = 1;
	int size = sizeof(buf);

	while (ret) {
		ret = recv(c_fd, (char*)buf + written, size - written, 0);
		if (ret == -1)
		{
			fprintf(stderr, "Error : cannot receive\n");
			close(c_fd);
			close(s_fd);
			return ret;
		}
		written += ret;
	}

	printf("Client said : %s\n", buf);
}
