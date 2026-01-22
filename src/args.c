#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include "args.h"

static int add_start_dir(struct config_t *cfg, const char *dir)
{
    char **new_start_dirs = realloc(cfg->start_dirs, (cfg->num_start_dirs + 1) * sizeof(char *));
    cfg->start_dirs = new_start_dirs;
    cfg->start_dirs[cfg->num_start_dirs] = NULL; // safety

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

static int parse_size_arg(const char *s, long *size_out, char *sign_out, long *unit_out)
{
    if (!s || !*s) return 1;

    // 1) sign
    char sign = '=';              // "=" bedeutet exakt
    if (*s == '+' || *s == '-') {
        sign = *s;
        s++;
        if (!*s) return 1;        // nur "+" ist ungültig
    }

    // 2) number
    errno = 0;
    char *end = NULL;
    long num = strtol(s, &end, 10);

    if (errno != 0) return 1;
    if (end == s) return 1;       // keine Zahl gelesen
    if (num < 0) return 1;        // wir erlauben negative NICHT, sign regelt das

    // 3) optional suffix
    long unit = 512;              // default wie find: 512-byte blocks
    if (*end != '\0') {
        if (end[1] != '\0') return 1; // nur 1 Zeichen suffix erlaubt
        switch (*end) {
            case 'c': unit = 1; break;            // bytes
            case 'k': unit = 1024; break;         // kib
            case 'M': unit = 1024 * 1024; break;  // mib
            default: return 1;
        }
    }

    // 4) overflow-safe multiply in long
    if (num > 0 && unit > LONG_MAX / num) return 1;

    *size_out = num * unit;   // in bytes
    *sign_out = sign;
    *unit_out = unit;
    return 0;
}

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
    cfg->parallel_threads = 1;

    cfg->size_set = 0;
    cfg->size = 0; 
    cfg->sign = '=';
    cfg->unit = 512;
    
    cfg ->empty_flag = 0;
}

void config_free(struct config_t* cfg)
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
int parse_arguments(int argc, char** argv, struct config_t* cfg)
{
    for (int i = 1; i < argc; i++) {
        
        // -empty 
        if(strcmp(argv[i], "-empty") == 0){
            cfg->empty_flag = 1;
            continue;
        }

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

        // -j <n>
        if (strcmp(argv[i], "-j") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "mfind: missing argument after -j\n");
                return 1;
            }
            if (parse_nonnegative_int(argv[i + 1], &cfg->parallel_threads) != 0 || cfg->parallel_threads <= 0) {
                fprintf(stderr, "mfind: invalid value for -j: %s\n", argv[i + 1]);
                return 1;
            }
            i++;
            continue;
        }

        // -size <[+|-]N[c|k|M]> ]
        if (strcmp(argv[i], "-size") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "mfind: missing argument after -size\n");
                return 1;
            }

            cfg->size_set = 1;

            long tmp_size = 0;
            long tmp_unit = 512;
            char tmp_sign = '=';

            if (parse_size_arg(argv[i + 1], &tmp_size, &tmp_sign, &tmp_unit) != 0) {
                fprintf(stderr, "mfind: invalid argument for -size: %s\n", argv[i + 1]);
                return 1;
            }

            cfg->size = tmp_size;
            cfg->sign = tmp_sign;
            cfg->unit = tmp_unit;

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
