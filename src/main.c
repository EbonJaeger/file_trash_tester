#include <stdio.h>
#include "trash.h"

int main(void)
{
    char *path = (char *)trash_get_path();
    printf("Trash directory: %s\n", path);

    GError *err = 0;
    GSList *files = trash_get_items(path, &err);
    g_free(path);

    if (!files || err)
    {
        if (err)
        {
            printf("Error listing trash items: %d: %s\n", err->code, err->message);
            return err->code;
        }
        else
        {
            printf("Unknown error while getting trash files\n");
            return 1;
        }
    }

    guint length = g_slist_length(files);
    printf("Files (%d total):\n\n", length);
    for (int i = 0; i < length; i++)
    {
        TrashItem *item = (TrashItem *)g_slist_nth_data(files, i);
        printf("%d - %s %s\n\tTrashed Path: %s\n\tRestore Path: %s\n\n", i + 1, item->directory ? "D" : "F", item->name, item->path, item->restore_path);
    }

    g_slist_free_full(g_steal_pointer(&files), (GDestroyNotify)trash_item_free);

    return 0;
}
