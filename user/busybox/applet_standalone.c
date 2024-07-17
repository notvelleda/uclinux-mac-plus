#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "usage.h"

#define STRINGIFY(x) ""#x
#define STRINGIFY_VALUE_OF(x) STRINGIFY(x)

#define TRIVIAL_USAGE(n) n##_trivial_usage
#define TRIVIAL_USAGE_OF(n) TRIVIAL_USAGE(n)

#define FULL_USAGE(n) n##_full_usage
#define FULL_USAGE_OF(n) FULL_USAGE(n)

const char *applet_name = STRINGIFY_VALUE_OF(APPLET_NAME);

extern void show_usage(void)
{
	const char *format_string;

	fprintf(stderr, "Usage: " STRINGIFY_VALUE_OF(APPLET_NAME) " " TRIVIAL_USAGE_OF(APPLET_NAME) "\n\n" FULL_USAGE_OF(APPLET_NAME) "\n");
	exit(EXIT_FAILURE);
}
