/*-
 * Copyright (c) 1998 Robert Nordier
 * All rights reserved.
 * Copyright (c) 2001 Robert Drehmel
 * All rights reserved.
 * Copyright (c) 2014 Nathan Whitehorn
 * All rights reserved.
 * Copyright (c) 2015 Eric McCorkle
 * All rights reverved.
 *
 * Redistribution and use in source and binary forms are freely
 * permitted provided that the above copyright notice and this
 * paragraph and the following disclaimer are duplicated in all
 * such forms.
 *
 * This software is provided "AS IS" and without any express or
 * implied warranties, including, without limitation, the implied
 * warranties of merchantability and fitness for a particular
 * purpose.
 */
#include <stdarg.h>
#include <stdbool.h>

#include <sys/cdefs.h>
#include <sys/param.h>

#include <efi.h>

#include "boot_module.h"

static EFI_HANDLE image;
static EFI_SYSTEM_TABLE* systab;
static EFI_BOOT_SERVICES *bootsrv;
static dev_info_t devinfo;
static EFI_GUID LoadedImageGUID = LOADED_IMAGE_PROTOCOL;

static int
dskread(void *buf, u_int64_t lba, int nblk)
{
	EFI_STATUS status;
	int size;

	lba = lba / (devinfo.dev->Media->BlockSize / DEV_BSIZE);
	size = nblk * DEV_BSIZE;
	status = devinfo.dev->ReadBlocks(devinfo.dev,
                                         devinfo.dev->Media->MediaId, lba,
	    size, buf);

	if (EFI_ERROR(status))
		return (-1);

	return (0);
}

#include "ufsread.c"

static ssize_t
fsstat(ufs_ino_t inode)
{
#ifndef UFS2_ONLY
	static struct ufs1_dinode dp1;
	ufs1_daddr_t addr1;
#endif
#ifndef UFS1_ONLY
	static struct ufs2_dinode dp2;
#endif
	static struct fs fs;
	static ufs_ino_t inomap;
	char *blkbuf;
	void *indbuf;
	size_t n, nb, size, off, vboff;
	ufs_lbn_t lbn;
	ufs2_daddr_t addr2, vbaddr;
	static ufs2_daddr_t blkmap, indmap;
	u_int u;

	blkbuf = dmadat->blkbuf;
	indbuf = dmadat->indbuf;
	if (!dsk_meta) {
		inomap = 0;
		for (n = 0; sblock_try[n] != -1; n++) {
			if (dskread(dmadat->sbbuf, sblock_try[n] / DEV_BSIZE,
			    SBLOCKSIZE / DEV_BSIZE))
				return -1;
			memcpy(&fs, dmadat->sbbuf, sizeof(struct fs));
			if ((
#if defined(UFS1_ONLY)
			    fs.fs_magic == FS_UFS1_MAGIC
#elif defined(UFS2_ONLY)
			    (fs.fs_magic == FS_UFS2_MAGIC &&
			    fs.fs_sblockloc == sblock_try[n])
#else
			    fs.fs_magic == FS_UFS1_MAGIC ||
			    (fs.fs_magic == FS_UFS2_MAGIC &&
			    fs.fs_sblockloc == sblock_try[n])
#endif
			    ) &&
			    fs.fs_bsize <= MAXBSIZE &&
			    fs.fs_bsize >= sizeof(struct fs))
				break;
		}
		if (sblock_try[n] == -1) {
			return -1;
		}
		dsk_meta++;
	} else
		memcpy(&fs, dmadat->sbbuf, sizeof(struct fs));
	if (!inode)
		return 0;
	if (inomap != inode) {
		n = IPERVBLK(&fs);
		if (dskread(blkbuf, INO_TO_VBA(&fs, n, inode), DBPERVBLK))
			return -1;
		n = INO_TO_VBO(n, inode);
#if defined(UFS1_ONLY)
		memcpy(&dp1, (struct ufs1_dinode *)blkbuf + n,
		    sizeof(struct ufs1_dinode));
#elif defined(UFS2_ONLY)
		memcpy(&dp2, (struct ufs2_dinode *)blkbuf + n,
		    sizeof(struct ufs2_dinode));
#else
		if (fs.fs_magic == FS_UFS1_MAGIC)
			memcpy(&dp1, (struct ufs1_dinode *)blkbuf + n,
			    sizeof(struct ufs1_dinode));
		else
			memcpy(&dp2, (struct ufs2_dinode *)blkbuf + n,
			    sizeof(struct ufs2_dinode));
#endif
		inomap = inode;
		fs_off = 0;
		blkmap = indmap = 0;
	}
	size = DIP(di_size);
	n = size - fs_off;
	return (n);
}

static struct dmadat __dmadat;

static bool
probe(dev_info_t* const dev)
{
        devinfo = *dev;
	dmadat = &__dmadat;
	if (fsread(0, NULL, 0)) {
		return 0;
	}
	return 1;
}

static void*
try_load(const dev_info_t dev,
         const char* const loader_path,
         size_t* const bufsizeref)
{
	ufs_ino_t ino;
	EFI_STATUS status;
	void *buffer;
	size_t bufsize;

        devinfo = dev;
	if ((ino = lookup(loader_path)) == 0) {
		printf("File %s not found\n", loader_path);
		return NULL;
	}

	bufsize = fsstat(ino);
        *bufsizeref = bufsize;
	status = systab->BootServices->AllocatePool(EfiLoaderData,
	    bufsize, &buffer);
	fsread(ino, buffer, bufsize);
        return buffer;
}

static void*
load(const dev_info_t devs[],
     const size_t ndevs,
     const char* const loader_path,
     int* const idxref,
     size_t* const bufsizeref)
{
        for(int i = 0; i < ndevs; i++)
        {
               void* const out = try_load(devs[i], loader_path, bufsizeref);
               if (out != NULL)
               {
                       *idxref = i;
                       return out;
               }
        }
        return NULL;
}


static void init(EFI_HANDLE xImage,
                 EFI_SYSTEM_TABLE* xSystab,
                 EFI_BOOT_SERVICES * xBootsrv)
{
        image = xImage;
        systab = xSystab;
        bootsrv = xBootsrv;
}

const boot_module_t ufs_module =
{
        .name = "UFS",
        .init = init,
        .probe = probe,
        .load = load
};
