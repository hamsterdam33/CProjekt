#include <stddef.h>
#include "args.h"

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