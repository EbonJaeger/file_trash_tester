#include <stdio.h>
#include "trash.h"

enum Command
{
    DELETE,
    LIST,
    RESTORE,
    UNKNOWN
};

enum Command parse_input_char(char *input)
{
    if (strlen(input) != 1)
    {
        return UNKNOWN;
    }

    switch (input[0])
    {
    case 'd':
    case 'D':
        return DELETE;
    case 'l':
    case 'L':
        return LIST;
    case 'r':
    case 'R':
        return RESTORE;
    default:
        return UNKNOWN;
    }
}

void print_help_text(void)
{
    printf("Trash Tester\n\n");
    printf("USAGE:\n\n\tfile_trash_tester [OPTIONS]\n\n");
    printf("OPTIONS:\n\n");
    printf("\td <file name>\tDelete an item in the trash\n");
    printf("\tl\t\tList all items in the trash\n");
    printf("\tr <file name>\tRestore an item from the trash\n\n");
}

int do_list_trash(GError *err)
{
    char *path = (char *)trash_get_path();
    printf("Trash directory: %s\n", path);

    GSList *files = trash_get_items(path, err);
    g_free(path);

    if (!files || err)
    {
        if (err)
        {
            return err->code;
        }
        else
        {
            return 1;
        }
    }

    guint length = g_slist_length(files);
    printf("Files (%d total):\n\n", length);
    for (int i = 0; i < length; i++)
    {
        TrashItem *item = (TrashItem *)g_slist_nth_data(files, i);
        printf("%d - %s - %s\n", i + 1, item->directory ? "D" : "F", item->name);
        printf("\tTrashed Path: %s\n", item->path);
        printf("\tRestore Path: %s\n", item->trash_info->restore_path);
        gchar *formatted_deletion_date = g_date_time_format(item->trash_info->deletion_date, "%B %e, %Y %I:%M %p");
        printf("\tDeletion Time: %s\n\n", formatted_deletion_date);
        g_free(formatted_deletion_date);
    }

    g_slist_free_full(g_steal_pointer(&files), (GDestroyNotify)trash_item_free);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc > 3)
    {
        printf("Invalid usage! Run the program with no options to view the help.\n");
        return 1;
    }
    else if (argc == 1)
    {
        print_help_text();
        return 0;
    }

    char *cmd_char = argv[1];
    enum Command cmd = parse_input_char(cmd_char);
    GError *err = 0;

    int result = 0;
    switch (cmd)
    {
    case DELETE:
        printf("Not implemented yet!\n");
        break;
    case LIST:
        printf("\n");
        result = do_list_trash(err);
        break;
    case RESTORE:
        printf("Not implemented yet!\n");
        break;
    case UNKNOWN:
        printf("Unknown command! Run without options to view the help.\n");
        break;
    }

    if (result != 0)
    {
        if (err)
        {
            printf("Error while performing command: %d: %s\n", err->code, err->message);
            g_error_free(err);
        }
        else
        {
            printf("Unknown error while performing command\n");
        }
    }

    return result;
}
