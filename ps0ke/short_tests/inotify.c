#include <stdio.h>
#include <unistd.h>
#include <sys/inotify.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main( int argc, char **argv ){
	int length, i;
	int fd;
	int wd;
	char buffer[BUF_LEN];

	fd = inotify_init();
	if (fd < 0)
		puts("error: inotify_init");

	wd = inotify_add_watch( fd, "/tmp/foo", IN_MODIFY);

	while(23){
		length = read(fd, buffer, BUF_LEN);
		if (length < 0)
			puts("error: read");

		i = 0;
		while (i < length){
			struct inotify_event *event = (struct inotify_event*) &buffer[i];
			if (event->mask & IN_MODIFY)
				puts("The file was modified.");

			i += EVENT_SIZE + event->len;
		}

		usleep(0);
	}

	(void) inotify_rm_watch(fd, wd);
	(void) close(fd);

	return 0;
}
