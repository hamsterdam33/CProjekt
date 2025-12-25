#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include "args.h"

static int add_start_dir(struct config_t *cfg, const char *dir)
{
    char **new_start_dirs = realloc(cfg->start_dirs, (cfg->num_start_dirs + 1) * sizeof(char *));
    
    if (!new_start_dirs) {
        fprintf(stderr, "mfind: out of memory err 01\n");
        return 1; // Memory allocation failure
    }

    cfg->start_dirs = new_start_dirs;


    cfg->start_dirs[cfg->num_start_dirs] = strdup(dir);
    if (!cfg->start_dirs[cfg->num_start_dirs]) {
        fprintf(stderr, "mfind: out of memory err 02\n");
        return 1; // Memory allocation failure
    }
    cfg->num_start_dirs++;
    return 0; // Success
}

/*
                           ⬆   
- Changes size of the array start_dirs and adds dir to the array.
- Returns 0 on success, 1 on memory allocation failure. 
- counts num_start_dirs up.
*/

static int parse_nonnegative_int(const char *s, int *out){
    char *end;
    errno = 0;

    long val = strtol(s, &end, 10);
    if (*end != '\0' || end == s || *end != '\0') {
        return 1; // Invalid integer
    }
    if(val < 0 || val > INT_MAX) {
        return 1; // Out of range
    }
    *out = (int)val;
    return 0; // Success
}

/*
-                         ⬆       
checks if s is a valid non-negative integer and converts it to int.
Returns 0 on success, 1 on failure.
*/

// Initializes the config_t structure with default values.
void config_init(struct config_t *cfg)
{
    cfg->start_dirs = NULL;
    cfg->num_start_dirs = 0;
    cfg->name_pattern = NULL;
    cfg->type = 0;
    cfg->min_depth = 0;
    cfg->max_depth = -1;
    cfg->parallel_threads = 0;
}

void config_free(struct config_t *cfg)
{
    if (!cfg) return;

    // Free each start directory string
    for (int i = 0; i < cfg->num_start_dirs; i++) {
        free(cfg->start_dirs[i]);
    }

    // Free the array of start directory pointers
    free(cfg->start_dirs);

    // Reset fields to default values
    cfg->start_dirs = NULL;
    cfg->num_start_dirs = 0;
}

// Parses command-line arguments and fills the config_t structure.
int parse_arguments(int argc, char **argv, struct config_t *cfg)
{
    for (int i = 1; i < argc; i++) {

        // -name <pattern> 
        if (strcmp(argv[i], "-name") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "mfind: missing argument after -name\n");
                return 1;
            }
            cfg->name_pattern = argv[i + 1];
            i++;
            continue;
        }

        // -type <f|d>
        if (strcmp(argv[i], "-type") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "mfind: missing argument after -type\n");
                return 1;
            }
            char type_char = argv[i + 1][0];
            if (type_char != 'f' && type_char != 'd') {
                fprintf(stderr, "mfind: invalid argument for -type: %s\n", argv[i + 1]);
                return 1;
            }
            cfg->type = type_char;
            i++;
            continue;
        }

        // -mindepth <n>
        if (strcmp(argv[i], "-mindepth") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "mfind: missing argument after -mindepth\n");
                return 1;
            }
            if (parse_nonnegative_int(argv[i + 1], &cfg->min_depth) != 0) {
                fprintf(stderr, "mfind: invalid value for -mindepth: %s\n", argv[i + 1]);
                return 1;
            }
            i++;
            continue;
        }
        // -maxdepth <n>
        if (strcmp(argv[i], "-maxdepth") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "mfind: missing argument after -maxdepth\n");
                return 1;
            }
            if (parse_nonnegative_int(argv[i + 1], &cfg->max_depth) != 0) {
                fprintf(stderr, "mfind: invalid value for -maxdepth: %s\n", argv[i + 1]);
                return 1;
            }
            i++;
            continue;
        }

        // Startdir
        if (argv[i][0] != '-') {
            if (add_start_dir(cfg, argv[i]) != 0) {
                return 1;
            }
            continue;
        }

        // Unknown option
        fprintf(stderr, "mfind: unknown option: %s\n", argv[i]);
        return 1;
    }

    // Default Startdir
    if (cfg->num_start_dirs == 0) {
        if (add_start_dir(cfg, ".") != 0) {
            return 1;
        }
    }

    // mindepth/maxdepth consistency
    if (cfg->max_depth >= 0 && cfg->min_depth > cfg->max_depth) {
        fprintf(stderr,
                "mfind: mindepth (%d) greater than maxdepth (%d)\n",
                cfg->min_depth, cfg->max_depth);
        return 1;
    }

    return 0;
}


// ᨐฅ