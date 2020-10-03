#include "trash.h"
#include <stdio.h>

GSList *trash_get_items(const char *path, GError *err)
{
    // Open our trash directory
    GFile *trash_dir = g_file_new_for_path(path);
    GFileEnumerator *enumerator = g_file_enumerate_children(trash_dir, G_FILE_ATTRIBUTE_STANDARD_NAME, G_FILE_QUERY_INFO_NONE, NULL, &err);
    if G_UNLIKELY (!enumerator)
    {
        // There was a problem getting the enumerator; return early
        g_object_unref(trash_dir);
        return NULL;
    }

    // Iterate over the directory's children and append each file name to a list
    GSList *files = NULL;
    GFileInfo *current_file;
    while ((current_file = g_file_enumerator_next_file(enumerator, NULL, &err)))
    {
        const char *file_name = g_file_info_get_name(current_file);
        char *trashed_path = (char *)g_build_path("/", path, file_name, NULL);

        // Parse the trashinfo file for this item
        char *trash_info_contents = trash_read_trash_info(file_name, &err);
        if G_UNLIKELY (!trash_info_contents)
        {
            break;
        }

        char *restore_path = trash_get_restore_path(trash_info_contents);
        GDateTime *deletion_date = trash_get_deletion_date(trash_info_contents);
        TrashInfo *trash_info = trash_info_new(restore_path, deletion_date);

        TrashItem *trash_item = trash_item_new_with_info(file_name, trashed_path, trash_info);
        g_warn_if_fail(trash_item != NULL);

        free(trash_info_contents);
        files = g_slist_append(files, (gpointer)trash_item);
    }

    // Free resources
    g_file_enumerator_close(enumerator, NULL, NULL);
    g_object_unref(enumerator);
    g_object_unref(trash_dir);

    return files;
}

gchar *trash_get_path(void)
{
    return g_build_path("/", g_get_user_data_dir(), "Trash", "files", NULL);
}

gchar *trash_get_info_file_path(const char *name)
{
    // Build the trashinfo file name
    char *file_ext = ".trashinfo";
    gchar *info_file_name = (gchar *)malloc(1 + strlen(name) + strlen(file_ext));
    strcpy(info_file_name, name);
    strcat(info_file_name, file_ext);

    // Build the path to the trashinfo file
    char *path = g_build_path("/", g_get_user_data_dir(), "Trash", "info", info_file_name, NULL);
    g_free(info_file_name);

    return path;
}

GDateTime *trash_get_deletion_date(char *data)
{
    int substr_start = (int)(strchr(data, '\n') - data + TRASH_INFO_DELETION_DATE_PREFIX_OFFSET);
    int length = strlen(data) - substr_start - 1;

    char *deletion_date_str = (char *)malloc(length + 1);

    deletion_date_str = substring(data, deletion_date_str, substr_start, length);
    deletion_date_str[length] = '\0';

    GDateTime *deletion_date = g_date_time_new_from_iso8601((const gchar *)deletion_date_str, g_time_zone_new_local());
    free(deletion_date_str);

    return deletion_date;
}

char *trash_get_restore_path(char *data)
{
    int end_of_line = (int)(strchr(data, '\n') - data);
    int length = end_of_line - TRASH_INFO_PATH_PREFIX_OFFSET;

    char *restore_path = (char *)malloc(length + 1);

    restore_path = substring(data, restore_path, TRASH_INFO_PATH_PREFIX_OFFSET, length);
    restore_path[length] = '\0';

    return restore_path;
}

char *trash_read_trash_info(const char *file_name, GError **err)
{
    // Get the path to the trashinfo file
    gchar *info_file_path = trash_get_info_file_path(file_name);

    // Open the file
    GFile *info_file = g_file_new_for_path(info_file_path);
    GFileInputStream *input_stream = g_file_read(info_file, NULL, err);
    if (!input_stream)
    {
        g_object_unref(info_file);
        g_free(info_file_path);
        return NULL;
    }

    // Seek to the Path line
    g_seekable_seek((GSeekable *)input_stream, TRASH_INFO_PATH_OFFSET, G_SEEK_SET, NULL, err);

    // Read the file contents and extract the line containing the restore path
    char *buffer = (char *)malloc(1024 * sizeof(char));
    gssize read;
    while ((read = g_input_stream_read((GInputStream *)input_stream, buffer, 1024, NULL, err)))
    {
        buffer[read] = '\0';
    }

    // Free some resources
    g_input_stream_close((GInputStream *)input_stream, NULL, NULL);
    g_object_unref(input_stream);
    g_object_unref(info_file);
    g_free(info_file_path);

    return buffer;
}
