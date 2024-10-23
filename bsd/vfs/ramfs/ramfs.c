#include <stdint.h>
#include <sys\mount.h>
#include <sys\malloc.h>
#include <sys\kernel.h>
#include <sys\vnode.h>
#include <sys\vnode_if.h>
#include <sys\param.h>
#include <sys\errno.h>

#define VV_ROOT 0x000001

struct ramfs_mount {
    struct vnode *root_vnode;
};

static int ramfs_init_root (struct mount *mp) {

	struct ramfs_mount *rm = (struct ramfs_mount *)mp->mnt_data;
    struct vnode *root_vnode;

    //def global error
    int error;

    uint32_t flavor = 0;

    error = vnode_create(VNCREATE_FLAVOR, VCREATESIZE, NULL, &root_vnode);
    if(error != 0)
    {
    	return error;
    }

    root_vnode->v_type = VDIR;
    root_vnode->v_flag |= VROOT;
    root_vnode->v_mntvnodes = mp;
    
    rm->root_vnode = root_vnode;

    return 0;
}

static int ramfs_mount (struct mount *mp) {
    struct  ramfs_mount *rm;

    rm = malloc(sizeof(struct ramfs_mount), M_TEMP, M_WAITOK | M_ZERO);
    if(!rm){
        return ENOMEM;
    }

    mp->mnt_data = rm;

    int err = ramfs_init_root(mp);
    if(err != 0){
        free(mp, rm);
        return err;
    }

    vfs_setfsprivate(mp, rm);
    vfs_getnewfsid(mp);

    return 0;
}

static struct vfsops ramfs_vfsops = {
    .vfs_mount = ramfs_mount
}

VFS_SET(ramfs_vfsops, ramfs, VFCF_SYNTHETIC);