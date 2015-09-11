/* Copyright (c) 2015 Eric McCorkle. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the author nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/queue.h>

#include <efi.h>

#include "boot_module.h"

#include "libzfs.h"
#include "zfsimpl.c"

#define PATH_CONFIG "/boot/config"
#define PATH_DOTCONFIG "/boot/.config"

static EFI_HANDLE image;
static EFI_SYSTEM_TABLE* systab;
static EFI_BOOT_SERVICES *bootsrv;

static int
vdev_read(vdev_t * const vdev,
          void * const priv,
          const off_t off,
          void * const buf,
          const size_t bytes)
{
        const dev_info_t* const devinfo = (const dev_info_t*) priv;
        const off_t lba = off / devinfo->dev->Media->BlockSize;
	const EFI_STATUS status =
          devinfo->dev->ReadBlocks(devinfo->dev,
                                   devinfo->dev->Media->MediaId,
                                   lba, bytes, buf);
	if (EFI_ERROR(status))
		return (-1);

	return (0);
}

static bool probe(dev_info_t* const dev)
{
        spa_t* spa;
        int result = vdev_probe(vdev_read, dev, &spa);
        dev->devdata = spa;

        return result == 0;
}

static void* try_load(const dev_info_t devinfo,
                      const char* const loader_path,
                      size_t* const bufsizeref)
{
        spa_t *spa = devinfo.devdata;
        struct zfsmount zfsmount;
        dnode_phys_t dn;
        bool autoboot = true;

        if (zfs_spa_init(spa) != 0) {
                // Mount failed.  Don't report this loudly
                return NULL;
        }

        // First, try mounting the ZFS volume
        if (zfs_mount(spa, 0, &zfsmount) != 0) {
                // Mount failed.  Don't report this loudly
                return NULL;
        }

        //vdev_t * const primary_vdev = spa_get_primary_vdev(spa);

        if (zfs_lookup(&zfsmount, loader_path, &dn) != 0) {
                return NULL;
        }

        struct stat st;
        if (zfs_dnode_stat(spa, &dn, &st)) {
                return NULL;
        }

        const size_t bufsize = st.st_size;
        void* buffer;
        EFI_STATUS status;

        *bufsizeref = bufsize;

        if (systab->BootServices->AllocatePool(EfiLoaderData,
                                               bufsize, &buffer) !=
            EFI_SUCCESS) {
                return NULL;
        }

        if (dnode_read(spa, &dn, 0, buffer, bufsize) < 0) {
                return NULL;
        }

        return buffer;
}

static int zfs_mount_ds(const char * const dsname,
                        struct zfsmount * const zfsmount,
                        spa_t ** const spa)
{
        uint64_t newroot;
        spa_t *newspa;
        char *q;

        q = strchr(dsname, '/');
        if (q)
	        *q++ = '\0';
        newspa = spa_find_by_name(dsname);
        if (newspa == NULL) {
	        printf("\nCan't find ZFS pool %s\n", dsname);
	        return -1;
        }

        if (zfs_spa_init(newspa))
                return -1;

        newroot = 0;
        if (q) {
                if (zfs_lookup_dataset(newspa, q, &newroot)) {
                        printf("\nCan't find dataset %s in ZFS pool %s\n",
                               q, newspa->spa_name);
                        return -1;
                }
        }
        if (zfs_mount(newspa, newroot, zfsmount)) {
                printf("\nCan't mount ZFS dataset\n");
                return -1;
        }
        *spa = newspa;
        return (0);
}

static void* load(const dev_info_t devs[],
                  const size_t ndevs,
                  const char* const loader_path,
                  int* const idxref,
                  size_t* const bufsizeref)
{
        for(int i = 0; i < ndevs; i++) {
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
        zfs_init();
}

const boot_module_t zfs_module =
{
        .name = "ZFS",
        .init = init,
        .probe = probe,
        .load = load
};
