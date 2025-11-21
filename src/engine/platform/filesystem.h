#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "engine/core/define.h" // IWYU pragma: keep
#include "engine/core/paths.h"
#include "engine/core/memory/arena.h"

typedef struct {
    void *handle;
    b8 is_valid;
} file_t;

typedef enum {
    READ_TEXT = 0x01,
    READ_BINARY = 0x02,
    WRITE_TEXT = 0x04,
    WRITE_BINARY = 0x08
} filemode_t;

typedef struct {
    arena_alloc_t *arena;
    path_t base_path;
    b8 is_available;
} file_system_t;

// initialization
file_system_t *file_system_init(arena_alloc_t *arena);

void file_system_kill(file_system_t *fs);

// utilities
b8 file_exist(const char *path);

b8 file_open(const char *path, filemode_t mode, file_t *handle);

void file_close(file_t *handle);

b8 file_size(file_t *handle, u64 *size);

b8 file_read_all_text(file_t *handle, char *text, u64 *out_read);

b8 file_read_all_binary(file_t *handle, u8 *out_byte, u64 *out_read);

#endif // FILESYSTEM_H
