#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include "filters.h"

#include "traversal.h"

#if 0
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
#endif

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
    // 1) Print if this path matches all filters
    if (matches_filters(path, depth, cfg)) {
        char line[4096 + 2];
        int len = snprintf(line, sizeof(line), "%s\n", path);
        if (len > 0 && len < (int)sizeof(line)) {
            (void)write(STDOUT_FILENO, line, (size_t)len);
        }
    }

    // 2) Stop traversing deeper if maxdepth reached
    if (cfg->max_depth >= 0 && depth >= cfg->max_depth) {
        return;
    }

    // 3) Only directories can be traversed
    if (!is_directory(path)) {
        return;
    }

    DIR *dir = opendir(path);
    if (!dir) {
        fprintf(stderr, "mfind: cannot open directory '%s': %s\n", path, strerror(errno));
        return; // best-effort: keep going elsewhere
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            continue;
        }

        char child[4096];
        int n = snprintf(child, sizeof(child), "%s/%s", path, name);
        if (n < 0 || n >= (int)sizeof(child)) {
            fprintf(stderr, "mfind: path too long: '%s/%s'\n", path, name);
            continue;
        }

        traverse(child, depth + 1, cfg);
    }

    closedir(dir);
}