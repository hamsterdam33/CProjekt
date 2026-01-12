#include <stdio.h>
#include "args.h"
#include "traversal.h"

//Function to print the configuration for debugging

/*  
    call debug through wsl 
    make clean
    make CFLAGS="-Wall -Wextra -pedantic -g -Iinclude -DDEBUG"
    // then run ./mfind -name test -type f || d 

    check for memory leaks with 
    valgrind --leak-check=full --show-leak-kinds=all ./mfind src
*/

static void print_config(const struct config_t *cfg) {
    printf("=== config_init() test ===\n");
    printf("num_start_dirs: %d\n", cfg->num_start_dirs);
    for (int i = 0; i < cfg->num_start_dirs; i++) {
    printf("start_dirs[%d]: %s\n", i, cfg->start_dirs[i]);
    }
    printf("name_pattern: %s\n", cfg->name_pattern ? cfg->name_pattern : "(NULL)");
    printf("type: %c\n", cfg->type ? cfg->type : '0');
    printf("min_depth: %d\n", cfg->min_depth);
    printf("max_depth: %d\n", cfg->max_depth);
    printf("parallel_threads: %d\n", cfg->parallel_threads);
    printf("===========================\n");
}


int main(int argc, char **argv) {
  
    // fprintf(stderr, "MAIN START\n");

    struct config_t cfg;
    // 1) Initialize configuration
    config_init(&cfg);     
    parse_arguments(argc, argv, &cfg);

    // fprintf(stderr, "START_DIRS = %d\n", cfg.num_start_dirs);

    for (int i = 0; i < cfg.num_start_dirs; i++) {
        traverse(cfg.start_dirs[i], 0, &cfg);
    }
 
    

    
#ifdef DEBUG
     print_config(&cfg);
#endif
    return 0;

    // Later: Tracersal here ...

    config_free(&cfg);
    return 0;

}



// ᨐฅ