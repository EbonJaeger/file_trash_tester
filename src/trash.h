#ifndef _TESTER_TRASH_H
#define _TESTER_TRASH_H

#include <glib.h>
#include <gio/gio.h>
#include "trash_item.h"
#include "utils.h"

/**
 * The offset to the beginning of the line containing the
 * restore path.
 */
#define TRASH_INFO_PATH_OFFSET 13

/**
 * The offset from the beginning of a line to the start of
 * the restore path in a trash info file.
 */
#define TRASH_INFO_PATH_PREFIX_OFFSET 5

/**
 * The offset to the beginning of the date from the start
 * of a line.
 */
#define TRASH_INFO_DELETION_DATE_PREFIX_OFFSET 14

/**
 * Get a list of all files in the current user's trash bin.
 * 
 * If there was an error reading the trash directory, `NULL` will
 * be returned, and `err` will be set.
 */
GSList *trash_get_items(const char *path, GError **err);

/**
 * Gets the user's current Trash directory.
 * For most people, this is "~/.local/share/Trash/items"
 * 
 * The result of this must be freed with g_free().
 */
gchar *trash_get_path(void);

/**
 * Get the path to the trash info file for a given file
 * in the trash bin.
 * 
 * The result of this must be freed with `g_free()`.
 */
gchar *trash_get_info_file_path(const char *name);

/**
 * Parse the contents of a trashinfo file to get the time
 * that a file was deleted.
 * 
 * The result of this should be unreffed with `g_date_time_unref()`.
 */
GDateTime *trash_get_deletion_date(char *data);

/**
 * Parse the contents of a trashinfo file to get the path
 * that a trashed file would be restored to.
 * 
 * The result of this should be freed with `free()`.
 */
char *trash_get_restore_path(char *data);

/**
 * Read the contents of a trashinfo file into memory.
 * 
 * The result of this should be freed with `free()`.
 */
char *trash_read_trash_info(const char *file_name, GError **err);

#endif
