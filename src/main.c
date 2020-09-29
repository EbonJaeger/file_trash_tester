#include <stdio.h>
#include "trash.h"

int main(void)
{
    char *path = (char *)trash_get_path();
    printf("Trash directory in use: %s\n", path);

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
    printf("Files (%d total):\n", length);
    for (int i = 0; i < length; i++)
    {
        printf("%d - %s\n", i + 1, (char *)g_slist_nth_data(files, i));
    }

    g_slist_free_full(g_steal_pointer(&files), g_free);

    printf("\n\n");

    char *restore_path = trash_get_restore_path("eclipse", &err);
    if (!restore_path)
    {
        if (err)
        {
            printf("Error getting restore path: %d: %s\n", err->code, err->message);
            return err->code;
        }
        else
        {
            printf("Unknown error while getting trash restore path\n");
            return 1;
        }
    }

    printf("Restore Path: %s\n", restore_path);
    free(restore_path);

    return 0;
}
