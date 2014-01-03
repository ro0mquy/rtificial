#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void handler(int signum){
	printf("received signal: %i\n", signum);
}

int main(int argc, const char *argv[]) {

	signal(SIGUSR1, handler);

	while(23)
		usleep(0);

	return 0;
}
