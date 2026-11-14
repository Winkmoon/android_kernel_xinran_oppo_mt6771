/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Filesystem context for the fsopen()/fsconfig()/fsmount() API.
 *
 * This is a backport of the Linux 5.2 mount API to 4.14.  Options are
 * collected as a legacy ",key=value" string and handed to the filesystem's
 * existing mount method when fsmount() is called, so no filesystem needs to
 * be converted.
 */
#ifndef _LINUX_FS_CONTEXT_H
#define _LINUX_FS_CONTEXT_H

#include <linux/types.h>
#include <linux/fs.h>

/* fsopen() flags */
#define FSOPEN_CLOEXEC		0x00000001

/* fsconfig() commands */
#define FSCONFIG_SET_FLAG	0
#define FSCONFIG_SET_STRING	1
#define FSCONFIG_SET_BINARY	2
#define FSCONFIG_SET_PATH	3
#define FSCONFIG_SET_PATH_EMPTY	4
#define FSCONFIG_SET_FD		5
#define FSCONFIG_CMD_CREATE	6
#define FSCONFIG_CMD_RECONFIGURE 7

/* fsmount() flags */
#define FSMOUNT_CLOEXEC		0x00000001

/* open_tree() flags */
#define OPEN_TREE_CLONE		1
#define OPEN_TREE_CLOEXEC	O_CLOEXEC

/* move_mount() flags */
#define MOVE_MOUNT_F_SYMLINKS		0x00000001
#define MOVE_MOUNT_F_AUTOMOUNTS		0x00000002
#define MOVE_MOUNT_F_EMPTY_PATH		0x00000004
#define MOVE_MOUNT_T_SYMLINKS		0x00000010
#define MOVE_MOUNT_T_AUTOMOUNTS		0x00000020
#define MOVE_MOUNT_T_EMPTY_PATH		0x00000040
#define MOVE_MOUNT__MASK		0x00000077

struct fs_context {
	struct file_system_type	*fs_type;
	unsigned int		sb_flags;
	unsigned int		sb_flags_mask;
	void			*s_fs_info;
	char			*source;
	char			*subtype;
	char			*options;	/* ",opt=val,opt" */
	unsigned int		options_len;
	unsigned int		options_alloc;
	bool			configured;
};

#endif /* _LINUX_FS_CONTEXT_H */
