#include <stdio.h>
#include "trash_store.h"

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

TrashStore *load_trash_store(GError *err)
{
    const char *trash_path = (const char *)g_build_path("/", g_get_user_data_dir(), "Trash", "files", NULL);
    const char *info_path = (const char *)g_build_path("/", g_get_user_data_dir(), "Trash", "info", NULL);
    TrashStore *trash_store = trash_store_new(trash_path, info_path);
    if (!trash_store)
    {
        printf("Unable to create a trash store struct\n");
        return NULL;
    }

    // Load the current items in the trash bin
    trash_load_items(trash_store, err);
    if (err)
    {
        printf("Error loading trash items: %d: %s\n", err->code, err->message);
        g_error_free(err);
        trash_store_free(trash_store);
        return NULL;
    }

    return trash_store;
}

void do_list_trash(TrashStore *trash_store)
{
    printf("Trash directory: %s\n", trash_store->trashed_file_path);

    guint length = 0;
    if (trash_store->trashed_items)
    {
        length = g_slist_length(trash_store->trashed_items);
    }

    printf("Files (%d total):\n\n", length);
    for (int i = 0; i < length; i++)
    {
        TrashItem *item = (TrashItem *)g_slist_nth_data(trash_store->trashed_items, i);
        printf("%d - %s - %s\n", i + 1, item->directory ? "D" : "F", item->name);
        printf("\tTrashed Path: %s\n", item->path);
        printf("\tRestore Path: %s\n", item->trash_info->restore_path);
        gchar *formatted_deletion_date = g_date_time_format(item->trash_info->deletion_date, "%B %e, %Y %I:%M %p");
        printf("\tDeletion Time: %s\n\n", formatted_deletion_date);
        g_free(formatted_deletion_date);
    }
}

int do_restore_file(TrashStore *trash_store, const char *file_name, GError *err)
{
    if (!trash_store->trashed_items)
    {
        printf("There are no items in the trash bin!\n");
        return 0;
    }

    // Try to find the correct file
    TrashItem *trash_item;
    guint length = g_slist_length(trash_store->trashed_items);
    for (int i = 0; i < length; i++)
    {
        trash_item = g_slist_nth_data(trash_store->trashed_items, i);
        if (strcmp(trash_item->name, file_name) == 0)
        {
            break;
        }
        trash_item = NULL;
    }

    if (!trash_item)
    {
        printf("That file is not in the trash bin!\n");
        return 0;
    }

    if (!trash_restore_item(trash_store, trash_item, NULL, NULL, &err))
    {
        return err->code;
    }

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

    // Create our default trash store
    GError *err = 0;
    TrashStore *trash_store = load_trash_store(err);
    if (!trash_store)
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

    // Parse the given command
    char *cmd_char = argv[1];
    enum Command cmd = parse_input_char(cmd_char);

    // Perform the command
    int result = 0;
    switch (cmd)
    {
    case DELETE:
        printf("Not implemented yet!\n");
        break;
    case LIST:
        printf("\n");
        do_list_trash(trash_store);
        break;
    case RESTORE:
        if (argc != 3)
        {
            printf("No file name given!\n");
            break;
        }
        result = do_restore_file(trash_store, argv[2], err);
        break;
    case UNKNOWN:
        printf("Unknown command! Run without options to view the help.\n");
        break;
    }

    // Print any messages on error
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

    trash_store_free(trash_store);

    return result;
}
