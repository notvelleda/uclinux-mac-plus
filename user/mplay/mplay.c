#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <getopt.h>

/*static int copyfd(int from, int to, char *buf, int size) {
	int n, w;
	char *buf2;

	while ((n = read(from, buf, size)) > 0) {
		buf2 = buf;
		//for (w = 0; w >= 0 && n > 0; n -= (w = write(to, readbuf, n)));
		while (n > 0) {
			w = write(to, buf2, n);
			n -= w;
			buf2 += w;
			if (w < 0) {
				return 1;
			}
		}
	}

	return n < 0 ? 1 : 0;
}*/

int play_sound(char *filename, int low_sample_rate, int volume) {
	int device, to_play;
	long buffer_size;
	int underruns;
	char *readbuf;
	char *buf;
	struct stat st;

	if ((to_play = open(filename, O_RDONLY)) < 0) {
		perror("couldn't open file");
		return 1;
	}

	stat(filename, &st);
	buffer_size = st.st_size;

	if (buffer_size <= 0) {
		//close(device);
		close(to_play);
		fprintf(stderr, "invalid buffer size %d\n", buffer_size);
		//perror("invalid buffer size");
		return 1;
	}

	//buffer_size *= 10;

	printf("using buffer size %d\n", buffer_size);

	if ((readbuf = (char *) malloc(buffer_size)) == 0) {
		//close(device);
		close(to_play);
		fprintf(stderr, "couldn't allocate buffer\n");
		return 1;
	}

	if (read(to_play, readbuf, buffer_size) != buffer_size) {
		perror("failed to read file");
		close(to_play);
		free(readbuf);
		return 1;
	}

	close(to_play);

	if ((device = open("/dev/sound", O_WRONLY)) < 0) {
		perror("couldn't open sound device");
		//close(to_play);
		free(readbuf);
		return 1;
	}

	if (ioctl(device, 1, low_sample_rate) != 0) {
		fprintf(stderr, "ioctl() SNDIO_SETLOWRT failed\n");
	}

	if (volume >= 0 && ioctl(device, 4, volume) != 0) {
		fprintf(stderr, "ioctl() SNDIO_SETVOL failed\n");
	}

	//buffer_size = ioctl(device, 0, NULL); // gets buffer size from driver

	/*fseek(to_play, 0, SEEK_END);
	buffer_size = ftell(to_play);
	fseek(to_play, 0, SEEK_SET);*/

	// buffer_size and malloc previously here

	//res = copyfd(to_play, device, readbuf, buffer_size);

	//for (w = 0; w >= 0 && n > 0; n -= (w = write(to, readbuf, n)));
	buf = readbuf;
	while (buffer_size > 0) {
		int w = write(device, buf, buffer_size);
		buffer_size -= w;
		buf += w;
		if (w < 0) {
			close(device);
			free(readbuf);
			perror("failed to write to device");
			return 1;
		}
	}

	underruns = ioctl(device, 3, NULL);

	close(device);
	free(readbuf);

	if (underruns >= 0) {
		printf("%d buffer underruns\n", underruns); // gets number of buffer underruns from driver
	} else {
		fprintf(stderr, "ioctl() SNDIO_GETRUNS failed\n");
	}

	return 0;
}

int main(int argc, char **argv) {
#if 1
	char *end;
	int opt;

	// options
	int low_sample_rate = 0;
	int volume = -1;

	// parse command line arguments
	while ((opt = getopt(argc, argv, ":hlv:")) != -1)  {
		switch (opt) {
			case 'h':
				printf("mplay: plays audio files\n" \
				       "usage: %s [-hl] [-v volume] FILE\n\n" \
					   "audio files must be 22255.5 Hz (or 11127.75 Hz) 8-bit unsigned PCM raw data\n\n" \
				       "options:\n" \
				       "\t-h\tdisplays this help information\n" \
				       "\t-l\thalves the sample rate to 11127.75 Hz\n" \
				       "\t-v\tsets the volume to the given value, on a scale from 0 to 7\n" \
					   , argv[0]);
				return 0;
			case 'l':
				low_sample_rate = 1;
				break;
			case 'v':
				volume = (int) strtol(optarg, &end, 10);
				if (end == optarg) {
					fprintf(stderr, "expected number for volume\ntry \"%s -h\" for help\n", argv[0]);
					return 1;
				}
				break;
			case '?':
				fprintf(stderr, "unknown option: %c\ntry \"%s -h\" for help\n", optopt, argv[0]);
				return 1;
			case ':':
				fprintf(stderr, "missing arg for %c\ntry \"%s -h\" for help\n", optopt, argv[0]);
				return 1;
		}
	}

	// do we have any audio to play?
	if (optind >= argc) {
		fprintf(stderr, "expected an audio file\ntry \"%s -h\" for help\n", argv[0]);
		return 1;
	} else {
		return play_sound(argv[optind], low_sample_rate, volume);
	}
#else
	int device, to_play;
	long buffer_size;
	int res;
	char *readbuf;
	char *buf;
	struct stat st;

	if (argc < 2 || argc > 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		return 1;
	}

	if ((to_play = open(argv[1], O_RDONLY)) < 0) {
		perror("couldn't open file");
		return 1;
	}

	if ((device = open("/dev/sound", O_WRONLY)) < 0) {
		perror("couldn't open sound device");
		close(to_play);
		return 1;
	}

	buffer_size = ioctl(device, 0, NULL);

	if (buffer_size <= 0) {
		close(device);
		close(to_play);
		fprintf(stderr, "invalid buffer size %d\n", buffer_size);
		return 1;
	}

	//buffer_size *= 10;

	printf("using buffer size %d\n", buffer_size);

	if ((readbuf = malloc(buffer_size)) == 0) {
		close(device);
		close(to_play);
		fprintf(stderr, "couldn't allocate buffer\n");
		return 1;
	}

	res = copyfd(to_play, device, readbuf, buffer_size);

	close(to_play);
	close(device);
	free(readbuf);

	return res;
#endif
}
