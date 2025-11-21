#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include "engine/core/define.h" // IWYU pragma: keep
#include "engine/core/memory/memory.h"
#include "engine/platform/filesystem.h"
#include "deps/stb_image/stb_image.h"

INL void *read_file_binary(const char *path, u64 *out_size)
{
    file_t file;
    if (!file_open(path, READ_BINARY, &file))
    {
        LOG_ERROR("Failed to open binary file: %s", path);
        return NULL;
    }

    u64 size = 0;
    if (!file_size(&file, &size) || size == 0)
    {
        LOG_ERROR("Binary file is empty: %s", path);
        file_close(&file);
        return NULL;
    }

    u8 *data = ALLOC(size, MEM_RESOURCE);
    u64 read_size = 0;
    if (!file_read_all_binary(&file, data, &read_size) || read_size != size)
    {
        LOG_ERROR("Failed to read binary file: %s", path);
        file_close(&file);
        FREE(data, size, MEM_RESOURCE);
        return NULL;
    }

    file_close(&file);
    *out_size = size;
    return data;
}

INL void *read_file_text(const char *path, u64 *out_size)
{
    file_t file;
    if (!file_open(path, READ_TEXT, &file))
    {
        LOG_ERROR("Failed to open text file: %s", path);
        return NULL;
    }

    u64 size = 0;
    if (!file_size(&file, &size) || size == 0)
    {
        LOG_ERROR("Text file is empty: %s", path);
        file_close(&file);
        return NULL;
    }

    char *data = ALLOC(size, MEM_RESOURCE);
    u64 read_size = 0;
    if (!file_read_all_text(&file, data, &read_size) || read_size != size)
    {
        LOG_ERROR("Failed to read text file: %s", path);
        file_close(&file);
        FREE(data, size, MEM_RESOURCE);
        return NULL;
    }

    data[size] = '\0';
    file_close(&file);
    *out_size = size;
    return data;
}

INL void *read_image_file(const char *path, i32 *width, i32 *height,
                          i32 *channels)
{
    file_t file;
    if (!file_open(path, READ_BINARY, &file))
    {
        LOG_ERROR("Failed to open image file: %s", path);
        return NULL;
    }

    u64 size = 0;
    if (!file_size(&file, &size))
    {
        LOG_ERROR("Image file is empty: %s", path);
        file_close(&file);
        return NULL;
    }

    u8 *data = ALLOC(size, MEM_RESOURCE);
    u64 read_size = 0;
    if (!file_read_all_binary(&file, data, &read_size) || read_size != size)
    {
        LOG_ERROR("Failed to read image file: %s", path);
        file_close(&file);
        FREE(data, size, MEM_RESOURCE);
        return NULL;
    }
    file_close(&file);

    stbi_set_flip_vertically_on_load(true);
    u8 *pixels = stbi_load_from_memory(data, (i32)size, width, height,
                                       channels, STBI_rgb_alpha);
    if (pixels) *channels = 4;

    FREE(data, size, MEM_RESOURCE);

    if (!pixels) return NULL;

    return pixels;
}

#endif // RESOURCE_LOADER_H
