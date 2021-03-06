#ifndef __BAT_H__
#define __BAT_H__

enum bat_status {
  CHARGING,
  CHARGED,
  DISCHARGING
};

struct bat_info {
  unsigned int max;
  unsigned int cur;
  enum bat_status status;
};

struct opts {
  char *path;
};

int get_bat_info(struct bat_info *info, struct opts *opts);

int get_opt_info(const int argc, char **argv, struct opts *opts);

void print(const struct bat_info *bat);

void bat_icon(const struct bat_info *bat, const unsigned short len);

#endif
