#include "trash_item.h"

TrashItem *trash_item_new(const char *name, const char *path)
{
    struct TrashItem *item = (struct TrashItem *)malloc(sizeof(struct TrashItem));
    if (!item)
    {
        return NULL;
    }

    item->name = name;
    item->path = path;
    item->directory = g_file_test(item->path, G_FILE_TEST_IS_DIR);

    return item;
}

TrashInfo *trash_info_new(char *restore_path, GDateTime *deletion_date)
{
    TrashInfo *trash_info = (TrashInfo *)malloc(sizeof(TrashInfo));
    if (!trash_info)
    {
        return NULL;
    }

    trash_info->restore_path = restore_path;
    trash_info->deletion_date = deletion_date;

    return trash_info;
}

void trash_info_free(TrashInfo *trash_info)
{
    if (!trash_info)
    {
        return;
    }

    free(trash_info->restore_path);
    g_date_time_unref(trash_info->deletion_date);
    free(trash_info);
}

void trash_item_free(TrashItem *item)
{
    if (!item)
    {
        return;
    }

    free((char *)item->name);
    free((char *)item->path);
    trash_info_free(item->trash_info);
    free(item);
}
