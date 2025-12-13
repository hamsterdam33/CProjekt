struct config_t {
char **start_dirs;
int num_start_dirs;
const char *name_pattern;
char type; // 'f', 'd' oder 0 = egal
int min_depth;
int max_depth;
int parallel_threads;	
};