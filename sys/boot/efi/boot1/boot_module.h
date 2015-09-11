#ifndef _BOOT_MODULE_H_
#define _BOOT_MODULE_H_

#include <stdbool.h>

#include <efi.h>
#include <efilib.h>
#include <eficonsctl.h>

#define UFS_EFI_BOOT 1
#define ZFS_EFI_BOOT 1

// EFI device info
typedef struct dev_info_t
{
      EFI_BLOCK_IO *dev;
      EFI_DEVICE_PATH *devpath;
      EFI_HANDLE *devhandle;
      void *devdata;
} dev_info_t;

// A boot loader module.  This is a standard interface for filesystem
// modules in the EFI system.
typedef struct boot_module_t
{
        const char* const name;

        // Initialize the module.
        void (* const init)(EFI_HANDLE image,
                            EFI_SYSTEM_TABLE* systab,
                            EFI_BOOT_SERVICES *bootsrv);

        // Check to see if curr_dev is a device that this module can handle.
        bool (* const probe)(dev_info_t* dev);

        // Select the best out of a set of devices that probe indicated were
        // loadable, and load it.
        void* (* const load)(const dev_info_t devs[],
                             size_t ndevs,
                             const char* loader_path,
                             int* idxref,
                             size_t* bufsizeref);
} boot_module_t;

// Standard boot modules
#ifdef UFS_EFI_BOOT
extern const boot_module_t ufs_module;
#endif
#ifdef ZFS_EFI_BOOT
extern const boot_module_t zfs_module;
#endif

// Functions available to modules
extern int strcmp(const char *s1, const char *s2);
extern void bcopy(const void *src, void *dst, size_t len);
extern void panic(const char *fmt, ...) __dead2;
extern int printf(const char *fmt, ...);
extern int vsnprintf(char *str, size_t sz, const char *fmt, va_list ap);

#endif
