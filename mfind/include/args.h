#ifndef ARGS_H
#define ARGS_H

struct config_t {
char **start_dirs;
int num_start_dirs;
const char *name_pattern;
char type; // 'f', 'd' oder 0 = egal
int min_depth;
int max_depth;
int parallel_threads;	
};

// Funktionsprototypen
void config_init(struct config_t *cfg);
    int parse_arguments(int argc, char **argv, struct config_t *cfg);
#endif