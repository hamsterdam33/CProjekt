#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <math.h>
#include "filters.h"

#include "traversal.h"

static int checkDepth(const struct config_t* cfg, int* depth) {
    if ((cfg->max_depth >= *depth || cfg->max_depth < 0) &&
        (cfg->min_depth <= *depth || cfg->min_depth < 0)) {
        return 0;
    }
    else {
        return 1;
    }
}

static int checkFilesize(const char* path, const struct config_t* cfg) {
    struct stat st;

    if (stat(path, &st) != 0) {
        return 1;
    }

    if ((st.st_size < cfg->size && cfg->sign == '-') ||
        (st.st_size > cfg->size && cfg->sign == '+') ||
        ((st.st_size + cfg->unit - 1) / cfg->unit == (cfg->size + cfg->unit - 1) / cfg->unit) ||
        (cfg->size == 0 && cfg->sign == ' ')) {
        return 0;
    }
    else {
        return 1;
    }
}

static int is_directory(const char *path)
{
        struct stat st;
        if (lstat(path, &st) != 0) {
                return 0; // Could not stat, treat as non-directory
        }
        return S_ISDIR(st.st_mode);
}

void traverse(const char *path, int depth, const struct config_t *cfg)
{
     // fprintf(stderr, "TRAVERSE %s\n", path);

        // Check filters and print if matched
        if (matches_filters(path, cfg)) {
           // fprintf(stderr, "CHECK %s\n", path);
            printf("%s\n", path);
        }

        // 1) print current path
        //if (checkDepth(cfg, &depth) == 0 &&
        //   (checkFilesize(path, cfg) == 0)) {
        //        printf("%s\n", path);
        //}


        // 2) check if path is a directory
        if (!is_directory(path)) {
                return; // Not a directory, nothing more to do
        }

        // 3) Open directory
        DIR *dir = opendir(path);
        if (!dir) {
                 fprintf(stderr, "mfind: cannot open directory '%s': %s\n", path, strerror(errno));
                return; // Not a directory or cannot open
        }

        // 4) Read entries
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
                const char *name = entry->d_name;

                // Skip "." and ".." to avoid endless loops
                if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
                        continue;                 
                }

                // 5) Construct child path
                char child_path[4096]; // Assuming PATH_MAX is 4096
                int n = snprintf(child_path, sizeof(child_path), "%s/%s", path, name);
                if (n < 0 || n >= (int)sizeof(child_path)) {
                        fprintf(stderr, "mfind: path too long: '%s/%s'\n", path, name);
                        continue;
                }   
                
                // 6) Recursive call
                traverse(child_path, depth + 1, cfg);
        }

        closedir(dir);

}