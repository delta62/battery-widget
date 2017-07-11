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

struct bat_info *mk_bat_info();
void destroy_bat_info(struct bat_info *info);
void get_bat_info(struct bat_info *info, struct opts *opts);

struct opts *mk_opts();
int get_opt_info(const int argc, char **argv, struct opts *opts);
void destroy_opts(struct opts *opts);

void print(const struct bat_info *bat);
char *bat_icon(const struct bat_info *bat, const unsigned short len);

#endif
