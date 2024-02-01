#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BUFF_SIZE 1024

int main(int argc, char* argv[]) {
	if (argc < 2)
	{
		fprintf(stderr, "Error: please enter messages\n");
		return -1;
	}

	int c_fd;
	struct sockaddr_un s_addr;
	char buff[BUFF_SIZE];

	strcpy(buff, argv[1]);
	c_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (c_fd == -1)
	{
		fprintf(stderr, "Error: cannot create socket\n");
		return -1;
	}

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sun_family = AF_UNIX;
	strcpy(s_addr.sun_path, "./test");

	int connecting;

	connecting = connect(c_fd, (struct sockaddr*)&s_addr, sizeof(s_addr));
	if (connecting == -1)
	{
		fprintf(stderr, "Error: cannot connect\n");
		close(c_fd);
		return -1;
	}

	int sending;

	sending = sendto(c_fd, buff, BUFF_SIZE, 0, (struct sockaddr*)&s_addr, sizeof(s_addr));
	if (sending == -1)
	{
		fprintf(stderr, "Error: cannot send messages\n");
		close(c_fd);
		return -1;
	}
	printf("Send messages: %s\n", argv[1]);
}
