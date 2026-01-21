#include <string.h> // strrchr, strcmp
#include <fnmatch.h> // fnmatch
#include <dirent.h>  // DIR, opendir, readdir, closedir, struct dirent
#include <sys/stat.h> // lstat, struct stat, S_ISREG, S_ISDIR
#include <errno.h>
#include <stdio.h>

#include "filters.h"


// forward declarations
static const char *basename_simple(const char *path);
static int is_empty_dir(const char *path);

static const char *basename_simple(const char *path){
    const char *slash = strrchr(path, '/');
    return slash ? slash + 1 : path;
}

static int is_empty_dir(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        return 0; // best-effort: treat errors as non-empty
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;
        if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0) {
            closedir(dir);
            return 0; // Found an entry other than . or ..
        }
    }

    closedir(dir);
    return 1; // Directory is empty
}

static int depth_allows_print(int depth, const struct config_t *cfg) {
    if (depth < cfg->min_depth) {
        return 0;
    }
    if (cfg->max_depth >= 0 && depth > cfg->max_depth) {
        return 0;
    }
    return 1;
}

int matches_filters(const char *path, int depth, const struct config_t *cfg) {

    struct stat st;
    if (lstat(path, &st) != 0) {
        perror("lstat");
        return 0; // On error, do not match
    }

    

    // depth gate (print)
    if (!depth_allows_print(depth, cfg)) {
        return 0;
    }

    // -type 
    if (cfg->type == 'f' && !S_ISREG(st.st_mode)) {
        return 0;
    }
    if (cfg->type == 'd' && !S_ISDIR(st.st_mode)) {
        return 0;
    }

    // -size
    if (cfg->size_set) {

        if (!S_ISREG(st.st_mode)) {
            return 0; // size filter only applies to regular files
        }

        if (cfg->sign == '+' && st.st_size <= cfg->size) return 0;
        if (cfg->sign == '-' && st.st_size >= cfg->size) return 0;
        if (cfg->sign == '=' && st.st_size != cfg->size) return 0;
    
    }

    // -name
    if (cfg->name_pattern) {
        const char *base = basename_simple(path);
        if (fnmatch(cfg->name_pattern, base, 0) != 0) {
            return 0;
        }
    }

    // -empty
     if (cfg->empty_flag) {
        if (S_ISREG(st.st_mode)) {
            if (st.st_size != 0) return 0;
        } else if (S_ISDIR(st.st_mode)) {
            if (!is_empty_dir(path)) return 0;
        } else {
            return 0; // other types are not considered empty
        }
    }

    return 1; // all filters passed
}



// ᨐฅ