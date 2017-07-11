#include <stdio.h>
#include <string.h>

#include "bat.h"

int main(int argc, char **argv)
{
	struct opts opts = {
		.path = "/sys/class/power_supply/BAT1"
	};
	struct bat_info info = {
		.max = 0,
		.cur = 0,
		.status = CHARGED
	};

	get_opt_info(argc, argv, &opts);
	get_bat_info(&info, &opts);

	print(&info);
}

void bat_icon(const struct bat_info *bat, const unsigned short len)
{
	char buf[len+4];
	if (bat->status == CHARGING || bat->status == CHARGED)
		buf[0] = '+';
	else
		buf[0] = '-';
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

	fputs(buf, stdout);
}

void print(const struct bat_info *bat)
{
	int pct = (int)((100.0 * bat->cur) / bat->max);
	bat_icon(bat, 8);
	printf(" %d%%", pct);
}

int get_opt_info(const int argc, char **argv, struct opts *opts)
{
	if (argc == 1) {
		return 0;
	} else if (argc == 3 && strcmp(argv[1], "-p") == 0) {
		opts->path = argv[2];
		return 0;
	} else {
		fprintf(stderr, "Unknown option %s\n", argv[1]);
		return -1;
	}
}

void get_bat_info(struct bat_info *info, struct opts *opts)
{
	int pathlen = strlen(opts->path) + 13;
	char path[pathlen];
	snprintf(path, pathlen, "%s/%s", opts->path, "energy_full");
	FILE *maxFile = fopen(path, "r");
	snprintf(path, pathlen, "%s/%s", opts->path, "energy_now");
	FILE *curFile = fopen(path, "r");

	char status[13];
	snprintf(path, pathlen, "%s/%s", opts->path, "status");
	FILE *statusFile = fopen(path, "r");
	fread(status, 1, 13, statusFile);
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
