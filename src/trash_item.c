#include "trash_item.h"
#include <stdio.h>

TrashItem *trash_item_new(const char *name, const char *path, char *restore_path)
{
    struct TrashItem *item = (struct TrashItem *)malloc(sizeof(struct TrashItem));
    if (!item)
    {
        return NULL;
    }

    item->name = name;
    item->path = path;
    item->restore_path = restore_path;
    item->directory = g_file_test(item->name, G_FILE_TEST_IS_DIR);

    return item;
}

void trash_item_free(TrashItem *item)
{
    if (!item)
    {
        return;
    }

    free((char *)item->name);
    free((char *)item->path);
    free(item->restore_path);
    free(item);
}
