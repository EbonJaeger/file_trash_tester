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

struct TrashStore;
typedef struct TrashStore
{
    const char *trashed_file_path;
    const char *trashed_info_path;
    GSList *trashed_items;
} TrashStore;

TrashStore *trash_store_new(const char *trashed_file_path, const char *trashed_info_path);

void trash_store_free(TrashStore *trash_store);

/**
 * Get a list of all files in the current user's trash bin.
 * 
 * If there was an error reading the trash directory, `NULL` will
 * be returned, and `err` will be set.
 */
void trash_load_items(TrashStore *trash_store, GError *err);

gboolean trash_restore_item(TrashStore *trash_store,
                            TrashItem *trash_item,
                            GFileProgressCallback progress_callback,
                            gpointer progress_data,
                            GError **err);

#endif
