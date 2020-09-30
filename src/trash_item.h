#ifndef _TRASH_ITEM_H
#define _TRASH_ITEM_H

#include <gio/gio.h>

struct TrashItem;

/**
 * Represents an item in the trash bin.
 */
typedef struct TrashItem
{
    const char *name;
    const char *path;
    char *restore_path;
    int directory;
} TrashItem;

/**
 * Creates and allocates a new TrashItem.
 * 
 * The returned pointer should be freed with `trash_item_free()`.
 */
TrashItem *trash_item_new(const char *name, const char *path, char *restore_path);

/**
 * Frees all resources for a TrashItem.
 */
void trash_item_free(struct TrashItem *item);

#endif