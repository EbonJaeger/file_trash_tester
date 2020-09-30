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
#define TRASH_INFO_PATH_OFFSET 18

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
 * Get the restore path for a given file in the trash bin.
 * 
 * The trash info file will be read to find the path. This
 * will block until it is done.
 * 
 * The result of this should be freed with `free()`.
 */
char *trash_get_restore_path(const char *name, GError **err);

#endif
