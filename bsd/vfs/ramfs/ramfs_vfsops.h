#ifndef _RAMFS_H_
#define _RAMFS_H_

#include <stdint.h>
#include <sys/mount.h>
#include <sys/malloc.h>
#include <sys/kernel.h>
#include <sys/vnode.h>
#include <sys/vnode_if.h>
#include <sys/param.h>
#include <sys/errno.h>

#define VV_ROOT 0x000001

struct ramfs_mount {
    struct vnode *root_vnode;
};

int ramfs_init_root (struct mount *m);
int ramfs_mount (struct mount *mp);

extern struct vfsops ramfs;

#endif