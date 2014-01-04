#include <stdio.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/fcntl.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main( int argc, char **argv ){
	int length, i;
	int fd;
	int wd;
	char buffer[BUF_LEN];

	fd = inotify_init();
	if (fd < 0)
		puts("error: inotify_init()");
	if(fcntl(fd, F_SETFL, O_NONBLOCK))
		puts("error: fcntl()");

	wd = inotify_add_watch( fd, "/tmp/foo", IN_MODIFY | IN_CLOSE_WRITE);

	int cont = 23;
	while(cont){
		length = read(fd, buffer, BUF_LEN);

		i = 0;
		while (i < length){
			struct inotify_event *event = (struct inotify_event*) &buffer[i];
			printf("i: %i; length: %i; mask: %i; IN_MODIFY: %i, mask & flag: %i\n", i, length, event->mask, IN_MODIFY, event->mask & IN_MODIFY);
			if (event->mask & IN_MODIFY){
				puts("The file was modified.");
//				cont = 0;
			}
			else if (event->mask & IN_CLOSE_WRITE){
				puts("the file was close-written");
//				cont = 0;
			}

			i += EVENT_SIZE + event->len;
		}

		usleep(0);
	}

	(void) inotify_rm_watch(fd, wd);
	(void) close(fd);

	return 0;
}
