#include <stdio.h>
#include <stdlib.h>

#define TBD()	\
	printf("%s(%d)\n", __func__, __LINE__);

static void f_object(void);
static void f_object_all(void);
static void f_show(void);

struct c {
	const char *name;
	void (*f)(void);
};

struct c object = { "object", f_object };
struct c object_all = { "object_all", f_object_all };
struct c show = { "show", f_show };
struct c *cs[] = {
	&object,
	&object_all,
	&show
};
#define ARRAY_SIZE(a)	(sizeof((a))/sizeof((a)[0]))

static void
f_object(void)
{

	TBD();
}

static void
f_object_all(void)
{

	TBD();
}

static void
f_show(void)
{

	TBD();
}

int
main(int argc, char **argv)
{
	int i, eq;

	if (argc != 2)
		exit(EXIT_FAILURE);
	eq = 0;
	for (i = 0; i < ARRAY_SIZE(cs); i++) {
		eq = (strcmp(cs[i]->name, argv[1]) == 0);
		if (eq) {
			cs[i]->f();
			break;
		}
	}
	if (eq == 0)
		fprintf(stderr, "no command '%s' found\n", argv[1]);
	exit(EXIT_SUCCESS);
}
