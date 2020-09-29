#include "trash.h"
#include <stdio.h>

GSList *trash_get_items(const char *path, GError **err)
{
    // Open our trash directory
    GFile *trash_dir = g_file_new_for_path(path);
    GFileEnumerator *enumerator = g_file_enumerate_children(trash_dir, G_FILE_ATTRIBUTE_STANDARD_NAME, G_FILE_QUERY_INFO_NONE, NULL, err);
    if (!enumerator)
    {
        // There was a problem getting the enumerator; return early
        g_object_unref(trash_dir);
        return NULL;
    }

    // Iterate over the directory's children and append each file name to a list
    GSList *files = NULL;
    GFileInfo *current_file;
    while ((current_file = g_file_enumerator_next_file(enumerator, NULL, err)))
    {
        files = g_slist_append(files, (gpointer)g_file_info_get_name(current_file));
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

gchar *trash_get_info_file_path(char *name)
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

char *trash_get_restore_path(char *name, GError **err)
{
    // Get the path to the trashinfo file
    gchar *info_file_path = trash_get_info_file_path(name);

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
    char buffer[1024];
    gssize read;
    while ((read = g_input_stream_read((GInputStream *)input_stream, buffer, 1024, NULL, err)))
    {
        buffer[read] = '\0';
    }

    // Free some resources
    g_input_stream_close((GInputStream *)input_stream, NULL, NULL);
    g_object_unref(info_file);
    g_free(info_file_path);

    // Get just the path itself
    int end_of_line = (int)(strchr(buffer, '\n') - buffer);
    char *restore_path = (char *)malloc(strlen(buffer) - end_of_line + 1);
    restore_path = substring(buffer, restore_path, 0, end_of_line);
    restore_path[end_of_line] = '\0';

    return restore_path;
}
