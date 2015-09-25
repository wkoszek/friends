#include <sys/param.h>
#include <sys/types.h>

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static int	g_num = 10;
static int	g_debug = 0;
#define	dbg if (g_debug) printf

int
main(int argc, char **argv)
{
	int	c2p[2];
	int	p2c[2];
	int	rv, fdi, fdo, len_to_wr, i, o;
	char	buf[1000];

	while ((o = getopt(argc, argv, "dn:")) != -1) {
		switch (o) {
		case 'd':
			g_debug = 1;
			break;
		case 'n':
			g_num = atoi(optarg);
			break;
		default:
			abort();
		}
	}

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	rv = pipe(c2p);
	assert(rv == 0 && "c2p failed");
	rv = pipe(p2c);
	assert(rv == 0 && "p2c failed");

	dbg("%d %d %d %d\n", c2p[0], c2p[1], p2c[0], p2c[1]);

	rv = fork();
	if (rv == 0) {	// child
		dbg("c>hild\n");
		fdo = c2p[1];
		fdi = p2c[0];
		for (i = 0; i < g_num; i++) {
			(void)snprintf(buf, sizeof(buf) - 1, "writing %d", i);
			len_to_wr = strlen(buf);
			dbg("c>len_to_wr=%d fdo=%d, buf='%s'\n", len_to_wr,
						fdo, buf);
			rv = write(fdo, buf, len_to_wr);
			dbg("c>rv = %d, errno=%d\n", rv, errno);
			rv = read(fdi, buf, sizeof(buf));
			dbg("c>rv = %d, got: '%s'\n", rv, buf);
		}
	} else {	// parent
		dbg("p>arent %d\n", rv);
		fdo = p2c[1];
		fdi = c2p[0];
		for (i = 0; i < g_num; i++) {
			memset(buf, 0, sizeof(buf));
			rv = read(fdi, buf, sizeof(buf));
			dbg("p>read> '%s' rv=%d\n", buf, rv);
			snprintf(buf, sizeof(buf) - 1, "received %d", rv);
			len_to_wr = strlen(buf);
			rv = write(fdo, buf, len_to_wr);
			dbg("p>wrote> '%s', rv=%d\n", buf, rv);
			assert(rv == len_to_wr);
		}
	}
}
