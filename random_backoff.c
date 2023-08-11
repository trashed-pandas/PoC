#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


int main(int argc, char **argv){

	char *program_invocation_short_name = argv[0];
	int controller = 1;

	int retval;
	int tmp_fd;

  unsigned int seed;

	unsigned long tmp_ulong;
	long backoff_ceiling;
	long backoff_floor;
	unsigned int backoff;

	struct timespec timeout;


	if((tmp_fd = open("/dev/random", O_RDONLY)) == -1){
		fprintf(stderr, "\n%s: %d: open(\"/dev/random\", O_RDONLY): %s\n", \
				program_invocation_short_name, controller, \
				strerror(errno));
		return(-1);
	}

	retval = read(tmp_fd, &seed, sizeof(seed));

	if(retval != sizeof(seed)){
		fprintf(stderr, "\n%s: %d: read(%d, %lx, %d): %s\n", \
				program_invocation_short_name, controller, \
				tmp_fd, (unsigned long) &seed, (int) sizeof(seed), \
				strerror(errno));
		return(-1);
	}

	close(tmp_fd);

	srandom(seed);


	backoff = 5;

	while(1){

		if(backoff == 20){
			return(0);
		}
		backoff_ceiling = 1<<backoff;
		backoff_floor = backoff * backoff;

		tmp_ulong = random();
		timeout.tv_nsec = backoff_floor + (tmp_ulong % (backoff_ceiling - backoff_floor));
		printf("%02d\t%ld\t%lu\t%ld\n", backoff, backoff_floor, timeout.tv_nsec, backoff_ceiling);

		backoff++;
	}

	return(0);
}
