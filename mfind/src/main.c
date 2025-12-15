#include <stdio.h>
#include "args.h"

int main(void) {
    struct config_t cfg;

    // 1) Initialize configuration 
    config_init(&cfg);

    // 2) Print default configuration values
    printf("=== config_init() test ===\n");
    printf("start_dirs: %p\n", (void*)cfg.start_dirs);
    printf("num_start_dirs: %d\n", cfg.num_start_dirs);
    printf("name_pattern: %p\n", (void*)cfg.name_pattern);
    printf("type: %d\n", cfg.type);
    printf("min_depth: %d\n", cfg.min_depth);
    printf("max_depth: %d\n", cfg.max_depth);
    printf("parallel_threads: %d\n", cfg.parallel_threads);

    return 0;
}

