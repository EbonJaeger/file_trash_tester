#include "trash_item.h"

struct TrashItem *trash_item_new(char *name, char *path, char *restore_path)
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

void trash_item_free(struct TrashItem *item)
{
    if (!item)
    {
        return;
    }

    free((char *)item->name);
    free(item->path);
    free(item->restore_path);
    free(item);
}
