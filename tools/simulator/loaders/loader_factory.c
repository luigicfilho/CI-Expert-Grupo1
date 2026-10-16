#include "loader_factory.h"
#include "stim_loader.h"
#include "test_loader.h"
#include "mem_loader.h"
#include <string.h>

stimulus_source_t* loader_factory_open(const char *filepath) {

    const char *ext = strrchr(filepath, '.');

    if (!ext) return NULL;

    if (strcmp(ext, ".stim") == 0) {
        return stim_loader_open(filepath);
    } 
    else if (strcmp(ext, ".test") == 0) {
        return test_loader_open(filepath);
    }
    else if (strcmp(ext, ".hex") == 0 || strcmp(ext, ".mem") == 0) {
        return mem_loader_open(filepath);
    }

    return NULL;
}