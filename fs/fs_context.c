// SPDX-License-Identifier: GPL-2.0
/*
 * fsopen()/fsconfig()/fsmount()/move_mount()/open_tree() for 4.14.
 *
 * A backport of the Linux 5.2 mount API.  It deliberately reuses the old
 * mount path: fsconfig() only collects a legacy option string and fsmount()
 * hands it to the filesystem's existing mount method through
 * vfs_kern_mount(), so no filesystem has to be converted.
 */

#include <linux/syscalls.h>
#include <linux/fs_context.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/anon_inodes.h>
#include <linux/security.h>
#include <linux/kernel.h>

/* fs/pnode.h is private to fs/ */
#define CL_COPY_ALL	(0x04 | 0x80)

/* MOUNT_ATTR_* passed to fsmount() */
#define MOUNT_ATTR_RDONLY	0x00000001
#define MOUNT_ATTR_NOSUID	0x00000002
#define MOUNT_ATTR_NODEV	0x00000004
#define MOUNT_ATTR_NOEXEC	0x00000008
#define MOUNT_ATTR__ATIME	0x00000070
#define MOUNT_ATTR_RELATIME	0x00000000
#define MOUNT_ATTR_NOATIME	0x00000010
#define MOUNT_ATTR_STRICTATIME	0x00000020
#define MOUNT_ATTR_NODIRATIME	0x00000080

/*
 * A file returned by fsmount() or open_tree().  "attached" records whether
 * the mount has been grafted into the tree, which decides whether closing
 * the fd drops a reference or tears the mount down.
 */
struct mount_fd {
	struct vfsmount	*mnt;
	bool		attached;
};

static int fscontext_release(struct inode *inode, struct file *file)
{
	struct fs_context *fc = file->private_data;

	if (fc) {
		kfree(fc->options);
		kfree(fc->source);
		put_filesystem(fc->fs_type);
		kfree(fc);
	}
	return 0;
}

static const struct file_operations fscontext_fops = {
	.release	= fscontext_release,
	.llseek		= no_llseek,
};

static int mount_fd_release(struct inode *inode, struct file *file)
{
	struct mount_fd *mf = file->private_data;

	if (mf) {
		/*
		 * mnt is NULL once move_mount() handed the mount to the tree:
		 * do_add_mount() consumed that reference on success.
		 */
		if (mf->mnt) {
			if (mf->attached)
				mntput(mf->mnt);
			else
				kern_unmount(mf->mnt);
		}
		kfree(mf);
	}
	return 0;
}

static const struct file_operations mount_fd_fops = {
	.release	= mount_fd_release,
	.llseek		= no_llseek,
};

/*
 * Append one option to the legacy option string.  Keys and values containing
 * a comma are refused because the legacy parsers split on it.
 */
static int fs_context_add_option(struct fs_context *fc, const char *key,
				 const char *val, bool has_val)
{
	unsigned int need, len;
	char *new;

	if (!key || !*key || strchr(key, ','))
		return -EINVAL;
	if (has_val && (!val || strchr(val, ',')))
		return -EINVAL;

	need = strlen(key) + 1;
	if (has_val)
		need += strlen(val) + 1;
	/* leading comma for everything after the first option */
	need += fc->options_len ? 1 : 0;

	if (fc->options_len + need + 1 > fc->options_alloc) {
		unsigned int n = max(fc->options_alloc * 2,
				     fc->options_len + need + 64);

		new = krealloc(fc->options, n, GFP_KERNEL);
		if (!new)
			return -ENOMEM;
		fc->options = new;
		fc->options_alloc = n;
	}

	len = fc->options_len;
	if (len)
		fc->options[len++] = ',';
	len += sprintf(fc->options + len, "%s", key);
	if (has_val) {
		fc->options[len++] = '=';
		len += sprintf(fc->options + len, "%s", val);
	}
	fc->options[len] = 0;
	fc->options_len = len;
	return 0;
}

SYSCALL_DEFINE2(fsopen, const char __user *, _fs_name, unsigned int, flags)
{
	struct file_system_type *fs_type;
	struct fs_context *fc;
	char *fs_name;
	int fd;

	if (flags & ~FSOPEN_CLOEXEC)
		return -EINVAL;

	fs_name = strndup_user(_fs_name, 256);
	if (IS_ERR(fs_name))
		return PTR_ERR(fs_name);

	fs_type = get_fs_type(fs_name);
	kfree(fs_name);
	if (!fs_type)
		return -ENODEV;

	fc = kzalloc(sizeof(*fc), GFP_KERNEL);
	if (!fc) {
		put_filesystem(fs_type);
		return -ENOMEM;
	}
	fc->fs_type = fs_type;

	fd = anon_inode_getfd("fscontext", &fscontext_fops, fc,
			      O_RDWR | (flags & FSOPEN_CLOEXEC ? O_CLOEXEC : 0));
	if (fd < 0) {
		put_filesystem(fs_type);
		kfree(fc);
	}
	return fd;
}

SYSCALL_DEFINE5(fsconfig, int, fd, unsigned int, cmd, const char __user *, _key,
		const void __user *, _value, int, aux)
{
	struct fs_context *fc;
	struct fd f;
	char *key = NULL;
	char *val = NULL;
	int ret;

	f = fdget(fd);
	if (!f.file)
		return -EBADF;
	if (f.file->f_op != &fscontext_fops) {
		fdput(f);
		return -EINVAL;
	}
	fc = f.file->private_data;
	if (!fc) {
		ret = -EBADF;
		goto out;
	}
	if (fc->configured && cmd != FSCONFIG_CMD_CREATE) {
		ret = -EBUSY;
		goto out;
	}

	if (_key) {
		key = strndup_user(_key, 256);
		if (IS_ERR(key)) {
			ret = PTR_ERR(key);
			key = NULL;
			goto out;
		}
	}
	if (_value && cmd != FSCONFIG_SET_FLAG) {
		val = strndup_user(_value, 4096);
		if (IS_ERR(val)) {
			ret = PTR_ERR(val);
			val = NULL;
			goto out;
		}
	}

	switch (cmd) {
	case FSCONFIG_SET_FLAG:
		ret = fs_context_add_option(fc, key, NULL, false);
		break;
	case FSCONFIG_SET_STRING:
	case FSCONFIG_SET_PATH:
		ret = fs_context_add_option(fc, key, val, true);
		break;
	case FSCONFIG_SET_PATH_EMPTY:
		ret = fs_context_add_option(fc, key, NULL, false);
		break;
	case FSCONFIG_CMD_CREATE:
		if (fc->configured) {
			ret = -EBUSY;
			break;
		}
		fc->configured = true;
		ret = 0;
		break;
	case FSCONFIG_SET_BINARY:
	case FSCONFIG_SET_FD:
	case FSCONFIG_CMD_RECONFIGURE:
	default:
		ret = -EOPNOTSUPP;
		break;
	}

out:
	kfree(key);
	kfree(val);
	fdput(f);
	return ret;
}

SYSCALL_DEFINE3(fsmount, int, fs_fd, unsigned int, flags, unsigned int, ms_flags)
{
	struct mount_fd *mf;
	struct fs_context *fc;
	struct vfsmount *mnt;
	unsigned int sb_flags = 0;
	struct fd f;
	int fd;

	if (flags & ~FSMOUNT_CLOEXEC)
		return -EINVAL;
	if (ms_flags & ~(MOUNT_ATTR_RDONLY | MOUNT_ATTR_NOSUID |
			 MOUNT_ATTR_NODEV | MOUNT_ATTR_NOEXEC |
			 MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME))
		return -EINVAL;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	f = fdget(fs_fd);
	if (!f.file)
		return -EBADF;
	if (f.file->f_op != &fscontext_fops) {
		fdput(f);
		return -EINVAL;
	}
	fc = f.file->private_data;
	if (!fc || !fc->fs_type) {
		fdput(f);
		return -EBADF;
	}
	if (!fc->configured) {
		fdput(f);
		return -EINVAL;
	}

	if (ms_flags & MOUNT_ATTR_RDONLY)
		sb_flags |= SB_RDONLY;
	if (ms_flags & MOUNT_ATTR_NOSUID)
		sb_flags |= SB_NOSUID;
	if (ms_flags & MOUNT_ATTR_NODEV)
		sb_flags |= SB_NODEV;
	if (ms_flags & MOUNT_ATTR_NOEXEC)
		sb_flags |= SB_NOEXEC;
	if (ms_flags & MOUNT_ATTR_NOATIME)
		sb_flags |= SB_NOATIME;
	if (ms_flags & MOUNT_ATTR_NODIRATIME)
		sb_flags |= SB_NODIRATIME;

	mnt = vfs_kern_mount(fc->fs_type, sb_flags, fc->source, fc->options);
	fdput(f);
	if (IS_ERR(mnt))
		return PTR_ERR(mnt);

	mf = kzalloc(sizeof(*mf), GFP_KERNEL);
	if (!mf) {
		kern_unmount(mnt);
		return -ENOMEM;
	}
	mf->mnt = mnt;
	mf->attached = false;

	fd = anon_inode_getfd("mount", &mount_fd_fops, mf,
			      O_RDWR | (flags & FSMOUNT_CLOEXEC ? O_CLOEXEC : 0));
	if (fd < 0) {
		kern_unmount(mnt);
		kfree(mf);
	}
	return fd;
}

/* fetch the mount fd backing @fd, or an error */
static struct mount_fd *mount_fd_get(int fd, struct fd *f)
{
	struct mount_fd *mf;

	*f = fdget(fd);
	if (!f->file)
		return ERR_PTR(-EBADF);
	if (f->file->f_op != &mount_fd_fops) {
		fdput(*f);
		return ERR_PTR(-EINVAL);
	}
	mf = f->file->private_data;
	if (!mf || !mf->mnt) {
		fdput(*f);
		return ERR_PTR(-EBADF);
	}
	return mf;
}

SYSCALL_DEFINE5(move_mount, int, from_dfd, const char __user *, from_pathname,
		int, to_dfd, const char __user *, to_pathname, unsigned int, flags)
{
	struct mount_fd *mf;
	struct fd f;
	struct path to;
	int ret;

	if (flags & ~MOVE_MOUNT__MASK)
		return -EINVAL;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	/* only the "from a mount fd" form is supported */
	mf = mount_fd_get(from_dfd, &f);
	if (IS_ERR(mf))
		return PTR_ERR(mf);
	if (from_pathname) {
		char c;

		if (get_user(c, from_pathname) || c) {
			ret = -EINVAL;
			goto out_fd;
		}
	}
	if (mf->attached) {
		ret = -EBUSY;
		goto out_fd;
	}

	ret = user_path_at(to_dfd, to_pathname, LOOKUP_FOLLOW, &to);
	if (ret)
		goto out_fd;

	ret = ksys_add_mount(mf->mnt, &to, 0);
	if (!ret) {
		/* the mount tree owns the reference now */
		mf->mnt = NULL;
		mf->attached = true;
	}
	path_put(&to);

out_fd:
	fdput(f);
	return ret;
}

SYSCALL_DEFINE3(open_tree, int, dfd, const char __user *, filename,
		unsigned, flags)
{
	struct mount_fd *mf;
	struct vfsmount *mnt;
	struct path path;
	int ret, fd;

	if (flags & ~(OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC))
		return -EINVAL;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	ret = user_path_at(dfd, filename, LOOKUP_FOLLOW, &path);
	if (ret)
		return ret;

	if (flags & OPEN_TREE_CLONE)
		mnt = ksys_clone_mnt(path.mnt, path.dentry, CL_COPY_ALL);
	else
		mnt = mntget(path.mnt);
	path_put(&path);
	if (IS_ERR(mnt))
		return PTR_ERR(mnt);

	mf = kzalloc(sizeof(*mf), GFP_KERNEL);
	if (!mf) {
		kern_unmount(mnt);
		return -ENOMEM;
	}
	mf->mnt = mnt;
	/* a clone is detached; a plain reference points at an attached mount */
	mf->attached = !(flags & OPEN_TREE_CLONE);

	fd = anon_inode_getfd("mount", &mount_fd_fops, mf,
			      O_RDWR | (flags & OPEN_TREE_CLOEXEC ? O_CLOEXEC : 0));
	if (fd < 0) {
		if (mf->attached)
			mntput(mnt);
		else
			kern_unmount(mnt);
		kfree(mf);
	}
	return fd;
}

/*
 * Reconfiguration of an existing mount is not implemented; reporting that
 * makes userspace fall back to the legacy remount path.
 */
SYSCALL_DEFINE3(fspick, int, dfd, const char __user *, path, unsigned int, flags)
{
	return -EOPNOTSUPP;
}
