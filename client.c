#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#define BUFF_SIZE 1024
#define FILE_PATH "./test"
#define Error() perror("Error: ");\
		return -1;

void* read_msg(void* c_fd) {
	char s_buff[BUFF_SIZE];

	while (1)
	{
		read(*(int*)c_fd, s_buff, BUFF_SIZE);
		printf("\nServer said: %s", s_buff);
	}
}

int main(void) {
	int c_fd;
	struct sockaddr_un s_addr;
	char c_buff[BUFF_SIZE];
	pthread_t read_thread;

	c_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (c_fd == -1)
	{
		Error();
	}

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sun_family = AF_UNIX;
	strcpy(s_addr.sun_path, FILE_PATH);
	if (connect(c_fd, (struct sockaddr*)&s_addr, sizeof(s_addr)) == -1)
	{
		close(c_fd);
		Error();
	}
	puts("Connected!");
	
	pthread_create(&read_thread, NULL, read_msg, (void*)&c_fd);
	while (1)
	{
		printf("You: ");
		fgets(c_buff, BUFF_SIZE, stdin);
		if (!write(c_fd, c_buff, BUFF_SIZE))
		{
			perror("Error: send message");
		}
		
		if (!strcmp(c_buff, "bye\n")) break;
	}
	close(c_fd);
}
