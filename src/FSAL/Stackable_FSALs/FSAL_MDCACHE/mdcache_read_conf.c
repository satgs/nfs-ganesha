/*
 * vim:noexpandtab:shiftwidth=8:tabstop=8:
 *
 * Copyright CEA/DAM/DIF  (2008)
 * contributeur : Philippe DENIEL   philippe.deniel@cea.fr
 *                Thomas LEIBOVICI  thomas.leibovici@cea.fr
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * ---------------------------------------
 */

/**
 * @addtogroup cache_inode
 * @{
 */

/**
 * @file cache_inode_read_conf.c
 * @brief Cache inode configuration parameter tables.
 */
#include "config.h"
#include "log.h"
#include "hashtable.h"
#include "fsal.h"
#include "mdcache_int.h"
#include "config_parsing.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/param.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

/** File cache configuration, settable in the CacheInode
    stanza. */

struct mdcache_parameter mdcache_param;

static struct config_item mdcache_params[] = {
	CONF_ITEM_UI32("NParts", 1, 32633, 7,
		       mdcache_parameter, nparts),
	CONF_ITEM_UI32("Cache_Size", 1, UINT32_MAX, 32633,
		       mdcache_parameter, cache_size),
	CONF_ITEM_BOOL("Use_Getattr_Directory_Invalidation", false,
		       mdcache_parameter, getattr_dir_invalidation),
	CONF_ITEM_UI32("Dir_Max_Deleted", 1, UINT32_MAX, 65536,
		       mdcache_parameter, dir.avl_max_deleted),
	CONF_ITEM_UI32("Dir_Max", 1, UINT32_MAX, 65536,
		       mdcache_parameter, dir.avl_max),
	CONF_ITEM_UI32("Dir_Chunk", 0, UINT32_MAX, 128,
		       mdcache_parameter, dir.avl_chunk),
	CONF_ITEM_UI32("Entries_HWMark", 1, UINT32_MAX, 100000,
		       mdcache_parameter, entries_hwmark),
	CONF_ITEM_UI32("LRU_Run_Interval", 1, 24 * 3600, 90,
		       mdcache_parameter, lru_run_interval),
	CONF_ITEM_BOOL("Cache_FDs", true,
		       mdcache_parameter, use_fd_cache),
	CONF_ITEM_UI32("FD_Limit_Percent", 0, 100, 99,
		       mdcache_parameter, fd_limit_percent),
	CONF_ITEM_UI32("FD_HWMark_Percent", 0, 100, 90,
		       mdcache_parameter, fd_hwmark_percent),
	CONF_ITEM_UI32("FD_LWMark_Percent", 0, 100, 50,
		       mdcache_parameter, fd_lwmark_percent),
	CONF_ITEM_UI32("Reaper_Work", 1, 2000, 1000,
		       mdcache_parameter, reaper_work),
	CONF_ITEM_UI32("Biggest_Window", 1, 100, 40,
		       mdcache_parameter, biggest_window),
	CONF_ITEM_UI32("Required_Progress", 1, 50, 5,
		       mdcache_parameter, required_progress),
	CONF_ITEM_UI32("Futility_Count", 1, 50, 8,
		       mdcache_parameter, futility_count),
	CONF_ITEM_BOOL("Retry_Readdir", false,
		       mdcache_parameter, retry_readdir),
	CONFIG_EOL
};

static void *mdcache_param_init(void *link_mem, void *self_struct)
{
	if (self_struct == NULL)
		return &mdcache_param;
	else
		return NULL;
}

struct config_block mdcache_param_blk = {
	.dbus_interface_name = "org.ganesha.nfsd.config.cache_inode",
	.blk_desc.name = "CacheInode",
	.blk_desc.type = CONFIG_BLOCK,
	.blk_desc.u.blk.init = mdcache_param_init,
	.blk_desc.u.blk.params = mdcache_params,
	.blk_desc.u.blk.commit = noop_conf_commit
};

int mdcache_set_param_from_conf(config_file_t parse_tree,
				struct config_error_type *err_type)
{
	(void) load_config_from_parse(parse_tree,
				      &mdcache_param_blk,
				      NULL,
				      true,
				      err_type);
	if (!config_error_is_harmless(err_type)) {
		LogCrit(COMPONENT_INIT,
			"Error while parsing MDCACHE specific configuration");
		return -1;
	}

	return 0;
}

/** @} */