#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "bat.h"

int main(int argc, char **argv)
{
	struct opts *opts = mk_opts();
	get_opt_info(argc, argv, opts);

	struct bat_info *info = mk_bat_info();
	get_bat_info(info);

	print(info, opts->fmt);

	destroy_opts(opts);
	destroy_bat_info(info);
}

struct opts* mk_opts()
{
	struct opts *ret = (struct opts *)malloc(sizeof(struct opts));
	ret->color = 0;
	ret->path = "/sys/class/power_supply/BAT1";
	ret->fmt = "%b %p%%";
	return ret;
}

char *bat_icon(const struct bat_info *bat, const unsigned short len)
{
	char *buf = (char *)malloc(len + 4);
	buf[0] = '+';
	buf[1] = '[';
	buf[len + 2] = ']';
	buf[len + 3] = '\0';

	float pct = (100.0 * bat->cur) / bat->max;
	float step = 100.0 / len;

	for (unsigned short i = 0; i < len; i++) {
		char c = ' ';
		if (pct >= 100 || step * (len - i) < pct) c = '|';
		buf[i + 2] = c;
	}

	return buf;
}

void print(const struct bat_info *bat, const char *fmt)
{
	int pct = (int)((100.0 * bat->cur) / bat->max);
	char * icon = bat_icon(bat, 4);
	printf("%s (%d%%)\n", icon, pct);
	free(icon);
}

int get_opt_info(const int argc, char **argv, struct opts *opts)
{
	int opt;
	while ((opt = getopt(argc, argv, "cf:p:")) != -1) {
		switch (opt) {
			case 'c':
				opts->color = 1;
				break;
			case 'f':
				opts->fmt = optarg;
				break;
			case 'p':
				opts->path = optarg;
				break;
			default:
				printf("Unknown option %c\n", opt);
				return -1;
		}
	}
}

void destroy_opts(struct opts *opts)
{
	if (opts != NULL) free(opts);
}

struct bat_info *mk_bat_info()
{
	struct bat_info *ret = (struct bat_info *)malloc(sizeof(struct bat_info));
	ret->max = 0;
	ret->cur = 0;
	return ret;
}

void destroy_bat_info(struct bat_info *info)
{
	if (info != NULL) free(info);
}

void get_bat_info(struct bat_info *info)
{
	FILE *maxFile = fopen("/sys/class/power_supply/BAT1/energy_full", "r");
	FILE *curFile = fopen("/sys/class/power_supply/BAT1/energy_now", "r");

	fscanf(maxFile, "%u", &info->max);
	fscanf(curFile, "%u", &info->cur);
}
