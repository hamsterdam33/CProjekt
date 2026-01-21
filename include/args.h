#ifndef ARGS_H
#define ARGS_H

struct config_t {
char **start_dirs;
int num_start_dirs;

const char *name_pattern;
char type; // 'f', 'd' oder 0 = egal
int empty_flag; // 1 if -empty is set, 0 otherwise	

int min_depth;
int max_depth;

int parallel_threads;

int size_set; // 1 if size filter is set, 0 otherwise
long size;
char sign;
long unit;


};

void config_free(struct config_t *cfg);
// Pointer to make dynamic array of start directories

// Funktionsprototypen
void config_init(struct config_t *cfg);
    int parse_arguments(int argc, char **argv, struct config_t *cfg);
#endif

void config_free(struct config_t *cfg);    
    