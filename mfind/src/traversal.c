#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "traversal.h"

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
        (void)cfg; // Unused parameter

        // 1) print current path
        printf("%s\n", path);

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