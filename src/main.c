#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "args.h"
#include "traversal.h"


struct thread_arg {
    const struct config_t *cfg;
    int start_index;   
    int step;          
};

static void *worker(void *p)
{
    struct thread_arg *a = (struct thread_arg *)p;

    for (int i = a->start_index; i < a->cfg->num_start_dirs; i += a->step) {
        traverse(a->cfg->start_dirs[i], 0, a->cfg);
    }
    return NULL;
}
//Function to print the configuration for debugging

/*  
    call debug through wsl 
    make clean
    make CFLAGS="-Wall -Wextra -pedantic -g -Iinclude -DDEBUG"
    // then run ./mfind -name test -type f || d 

    check for memory leaks with 
    valgrind --leak-check=full --show-leak-kinds=all ./mfind src
*/
#ifdef DEBUG
   static void print_config(const struct config_t *cfg) {
    printf("=== config_init() test ===\n");
    printf("num_start_dirs: %d\n", cfg->num_start_dirs);
    for (int i = 0; i < cfg->num_start_dirs; i++) {
    printf("start_dirs[%d]: %s\n", i, cfg->start_dirs[i]);
    }
    printf("name_pattern: %s\n", cfg->name_pattern ? cfg->name_pattern : "(NULL)");
    printf("type: %c\n", cfg->type ? cfg->type : '0');
    printf("empty_flag: %d\n", cfg->empty_flag);
    printf("size_set: %d\n", cfg->size_set);
    printf("size: %ld\n", cfg->size);
    printf("min_depth: %d\n", cfg->min_depth);
    printf("max_depth: %d\n", cfg->max_depth);
    printf("parallel_threads: %d\n", cfg->parallel_threads);
    printf("===========================\n");
}
#endif



int main(int argc, char **argv) {
    struct config_t cfg;
    int rc = 0;

    config_init(&cfg);

    if (parse_arguments(argc, argv, &cfg) != 0) {
        rc = 1;           // usage error
        goto cleanup;
    }

    // Easy mode parallel: nur Startdirs parallelisieren
    if (cfg.parallel_threads <= 1 || cfg.num_start_dirs <= 1) {
        for (int i = 0; i < cfg.num_start_dirs; i++) {
            traverse(cfg.start_dirs[i], 0, &cfg);
        }
    } else {
        int nthreads = cfg.parallel_threads;
        if (nthreads > cfg.num_start_dirs) nthreads = cfg.num_start_dirs;

        pthread_t *threads = calloc((size_t)nthreads, sizeof(*threads));
        struct thread_arg *args = calloc((size_t)nthreads, sizeof(*args));
        if (!threads || !args) {
            fprintf(stderr, "mfind: out of memory (threads)\n");
            free(threads);
            free(args);
            rc = 2;        // runtime error
            goto cleanup;
        }

        for (int t = 0; t < nthreads; t++) {
            args[t].cfg = &cfg;
            args[t].start_index = t;
            args[t].step = nthreads;

            if (pthread_create(&threads[t], NULL, worker, &args[t]) != 0) {
                fprintf(stderr, "mfind: pthread_create failed\n");
                rc = 2;
                // join was schon läuft
                for (int j = 0; j < t; j++) pthread_join(threads[j], NULL);
                free(threads);
                free(args);
                goto cleanup;
            }
        }

        for (int t = 0; t < nthreads; t++) {
            pthread_join(threads[t], NULL);
        }

        free(threads);
        free(args);
    }

    #ifdef DEBUG
    print_config(&cfg);
    #endif

cleanup:
    config_free(&cfg);
    return rc;
}


// ᨐฅ