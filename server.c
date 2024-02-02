#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <pthread.h>
#include <signal.h>

#define BUFF_SIZE 1024
#define FILE_PATH "./test"
#define Error() perror("Error: ");\
		return -1;

void* send_msg(void* c_fd) {
	char s_buff[BUFF_SIZE];

	while (1)
	{
		printf("You: ");
		fgets(s_buff, BUFF_SIZE, stdin);
		if (!write(*(int*)c_fd, s_buff, BUFF_SIZE))
		{
			perror("Error: send message");
		}
	}
}

int main(void) {
	int s_fd, c_fd, c_addr_size;
	struct sockaddr_un s_addr, c_addr;
	char c_buff[BUFF_SIZE];
	sigset_t sigset;
	pthread_t send_thread;

	if (!access(FILE_PATH, F_OK))
		unlink(FILE_PATH);
	
	s_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (s_fd == -1)
	{
		Error();
	}

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sun_family = AF_UNIX;
	strcpy(s_addr.sun_path, FILE_PATH);
	if (bind(s_fd, (struct sockaddr*)&s_addr, sizeof(s_addr)) == -1)
	{
		close(s_fd);
		Error();
	}	

	if (listen(s_fd, 5) == -1)
	{
		close(s_fd);
		Error();
	}
	printf("Listening...\n");

	c_addr_size = sizeof(c_addr);
	c_fd = accept(s_fd, (struct sockaddr*)&c_addr, &c_addr_size);
	if (c_fd == -1)
	{
		close(s_fd);
		Error();
	}
	printf("Accept!\n");
	
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);
	sigprocmask(SIG_BLOCK, &sigset, (sigset_t*)NULL);
	pthread_create(&send_thread, NULL, send_msg, (void*)&c_fd);
	while (read(c_fd, c_buff, BUFF_SIZE))
	{
		printf("\nClient said : %s", c_buff);

		if (!strcmp(c_buff, "bye\n")) 
			break;
	}
	if (shutdown(c_fd, SHUT_RDWR) == -1)
	{
		Error();
	}
	close(c_fd);
	close(s_fd);
}
