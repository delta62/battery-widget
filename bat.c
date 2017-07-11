#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "bat.h"

int main(int argc, char **argv)
{
	struct opts *opts = mk_opts();
	get_opt_info(argc, argv, opts);

	struct bat_info *info = mk_bat_info();
	get_bat_info(info, opts);

	print(info);

	destroy_opts(opts);
	destroy_bat_info(info);
}

struct opts* mk_opts()
{
	struct opts *ret = (struct opts *)malloc(sizeof(struct opts));
	ret->path = "/sys/class/power_supply/BAT1";
	return ret;
}

char *bat_icon(const struct bat_info *bat, const unsigned short len)
{
	char *buf = (char *)malloc(len + 4);
	switch (bat->status) {
		case CHARGING:
		case CHARGED:
			buf[0] = '+';
			break;
		default:
			buf[0] = '-';
			break;
	}
	buf[1] = '[';
	buf[len + 2] = ']';
	buf[len + 3] = '\0';

	float pct = (100.0 * bat->cur) / bat->max;
	float step = 100.0 / len;

	for (unsigned short i = 0; i < len; i++) {
		char c = ' ';
		if (step * (len - i) <= pct) c = '|';
		buf[i + 2] = c;
	}

	return buf;
}

void print(const struct bat_info *bat)
{
	int pct = (int)((100.0 * bat->cur) / bat->max);
	char * icon = bat_icon(bat, 8);
	printf("%s (%d%%)\n", icon, pct);
	free(icon);
}

int get_opt_info(const int argc, char **argv, struct opts *opts)
{
	int opt;
	while ((opt = getopt(argc, argv, "p:")) != -1) {
		switch (opt) {
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
	ret->status = CHARGED;
	return ret;
}

void destroy_bat_info(struct bat_info *info)
{
	if (info != NULL) free(info);
}

void get_bat_info(struct bat_info *info, struct opts *opts)
{
	int pathlen = strlen(opts->path) + 13;
	char path[pathlen];
	snprintf(path, pathlen, "%s/%s", opts->path, "energy_full");
	FILE *maxFile = fopen(path, "r");
	snprintf(path, pathlen, "%s/%s", opts->path, "energy_now");
	FILE *curFile = fopen(path, "r");

	char status[56];
	snprintf(path, pathlen, "%s/%s", opts->path, "status");
	FILE *statusFile = fopen(path, "r");
	fread(status, 1, 55, statusFile);
	if (strcmp(status, "Charging\n") == 0) {
		info->status = CHARGING;
	} else if (strcmp(status, "Discharging\n") == 0) {
		info->status = DISCHARGING;
	} else {
		info->status = CHARGED;
	}

	fscanf(maxFile, "%u", &info->max);
	fscanf(curFile, "%u", &info->cur);

	fclose(maxFile);
	fclose(curFile);
	fclose(statusFile);
}
