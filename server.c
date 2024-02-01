#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

#define BUFF_SIZE 1024

int main(void) {
	int s_fd, c_fd, c_addr_size, ret;
	struct sockaddr_un s_addr, c_addr;
	char buf[BUFF_SIZE];

	s_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (s_fd == -1)
	{
		fprintf(stderr, "Error: cannot create socket\n"); 
		return -1;
	}

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sun_family = AF_UNIX;
	strcpy(s_addr.sun_path, "./test");
	if (bind(s_fd, (struct sockaddr*)&s_addr, sizeof(s_addr)) == -1)
	{
		fprintf(stderr, "Error: cannot bind socket\n");
		close(s_fd);
		return -1;
	}	

	if (listen(s_fd, 5) == -1)
	{
		fprintf(stderr, "Error: cannont listen\n");
		close(s_fd);
		return -1;
	}
	printf("Listening...\n");

	c_addr_size = sizeof(c_addr);
	c_fd = accept(s_fd, (struct sockaddr*)&c_addr, &c_addr_size);
	if (c_fd == -1)
	{
		fprintf(stderr, "Error: cannot accept\n");
		close(s_fd);
		return -1;
	}
	printf("Accept!\n");

	ret = recvfrom(s_fd, buf, BUFF_SIZE, 0, (struct sockaddr*)&c_addr, &c_addr_size);
	if (ret == -1)
	{
		fprintf(stderr, "Error : cannot receive\n");
		close(c_fd);
		close(s_fd);
		return ret;
	}
	printf("Client said : %s\n", buf);
}
