/*-
 * Copyright (c) 1998 Robert Nordier
 * All rights reserved.
 * Copyright (c) 2001 Robert Drehmel
 * All rights reserved.
 * Copyright (c) 2014 Nathan Whitehorn
 * All rights reserved.
 * Copyright (c) 2014 Eric McCorkle
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

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <machine/elf.h>
#include <machine/stdarg.h>

#include <efi.h>
#include <eficonsctl.h>

#include "boot_module.h"

#define _PATH_LOADER	"/boot/loader.efi"
#define _PATH_KERNEL	"/boot/kernel/kernel"

#define BSIZEMAX	16384

typedef int putc_func_t(char c, void *arg);

struct sp_data {
	char	*sp_buf;
	u_int	sp_len;
	u_int	sp_size;
};

static const boot_module_t* const boot_modules[] =
{
#ifdef ZFS_EFI_BOOT
        &zfs_module,
#endif
#ifdef UFS_EFI_BOOT
        &ufs_module
#endif
};

#define NUM_BOOT_MODULES (sizeof(boot_modules) / sizeof(boot_module_t*))

static const char digits[] = "0123456789abcdef";

static int __printf(const char *fmt, putc_func_t *putc, void *arg, va_list ap);
static int __putc(char c, void *arg);
static int __puts(const char *s, putc_func_t *putc, void *arg);
static int __sputc(char c, void *arg);
static char *__uitoa(char *buf, u_int val, int base);
static char *__ultoa(char *buf, u_long val, int base);

static int domount(EFI_DEVICE_PATH *device, EFI_BLOCK_IO *blkio, int quiet);
static void load(const char *fname);

static EFI_SYSTEM_TABLE *systab;
static EFI_HANDLE *image;


void* Malloc(size_t len, const char* file, int line)
{
        void* out;
        if (systab->BootServices->AllocatePool(EfiLoaderData,
                                               len, &out) !=
            EFI_SUCCESS) {
                printf("Can't allocate memory pool\n");
                return NULL;
        }
        return out;
}

char* strcpy(char* dst, const char* src) {
        for(int i = 0; src[i]; i++)
                dst[i] = src[i];

        return dst;
}

char* strchr(const char* s, int c) {
        for(int i = 0; s[i]; i++)
                if (s[i] == c)
                        return (char*)(s + i);

        return NULL;
}

int strncmp(const char *a, const char *b, size_t len)
{
        for (int i = 0; i < len; i++)
                if(a[i] == '\0' && b[i] == '\0') {
                        return 0;
                } else if(a[i] < b[i]) {
                        return -1;
                } else if(a[i] > b[i]) {
                        return 1;
                }

        return 0;
}

char* strdup(const char* s) {
        int len;

        for(len = 1; s[len]; len++);

        char* out = malloc(len);

        for(int i = 0; i < len; i++)
                out[i] = s[i];

        return out;
}

int bcmp(const void *a, const void *b, size_t len)
{
        const char *sa = a;
        const char *sb = b;

        for (int i = 0; i < len; i++)
                if(sa[i] != sb[i])
                        return 1;

        return 0;
}

int memcmp(const void *a, const void *b, size_t len)
{
        return bcmp(a, b, len);
}

void bcopy(const void *src, void *dst, size_t len)
{
	const char *s = src;
	char *d = dst;

	while (len-- != 0)
		*d++ = *s++;
}

void* memcpy(void *dst, const void *src, size_t len)
{
	bcopy(src, dst, len);
        return dst;
}


void* memset(void *b, int val, size_t len)
{
	char *p = b;

	while (len-- != 0)
		*p++ = val;

        return b;
}

int strcmp(const char *s1, const char *s2)
{
	for (; *s1 == *s2 && *s1; s1++, s2++)
		;
	return ((u_char)*s1 - (u_char)*s2);
}

int putchr(char c, void *arg)
{
	CHAR16 buf[2];

	if (c == '\n') {
		buf[0] = '\r';
		buf[1] = 0;
		systab->ConOut->OutputString(systab->ConOut, buf);
	}
	buf[0] = c;
	buf[1] = 0;
        systab->ConOut->OutputString(systab->ConOut, buf);
	return (1);
}

static EFI_GUID BlockIoProtocolGUID = BLOCK_IO_PROTOCOL;
static EFI_GUID DevicePathGUID = DEVICE_PATH_PROTOCOL;
static EFI_GUID ConsoleControlGUID = EFI_CONSOLE_CONTROL_PROTOCOL_GUID;
static EFI_GUID LoadedImageGUID = LOADED_IMAGE_PROTOCOL;

#define MAX_DEVS 128

void try_load(const boot_module_t* const mod,
              const dev_info_t devs[],
              size_t ndevs)
{
        int idx;
        size_t bufsize;
        void* const buffer = mod->load(devs, ndevs, _PATH_LOADER, &idx, &bufsize);
        EFI_HANDLE loaderhandle;
        EFI_LOADED_IMAGE *loaded_image;

        if (NULL == buffer) {
                printf("Could not load file\n");
                return;
        }
        //printf("Loaded file %s, image at %p\n"
        //       "Attempting to load as bootable image...",
        //       _PATH_LOADER, image);
        if (systab->BootServices->LoadImage(TRUE, image, devs[idx].devpath,
                                            buffer, bufsize, &loaderhandle) !=
            EFI_SUCCESS) {
                //printf("failed\n");
                return;
        }
        //printf("success\n"
        //       "Preparing to execute image...");

        if (systab->BootServices->HandleProtocol(loaderhandle,
                                                 &LoadedImageGUID,
                                                 (VOID**)&loaded_image) !=
            EFI_SUCCESS) {
                //printf("failed\n");
                return;
        }

        //printf("success\n");

        loaded_image->DeviceHandle =  devs[idx].devhandle;

	//printf("Image prepared, attempting to execute\n");
        // XXX Set up command args first
        if (systab->BootServices->StartImage(loaderhandle, NULL, NULL) !=
            EFI_SUCCESS) {
                //printf("Failed to execute loader\n");
                return;
        }
        //printf("Shouldn't be here!\n");
}

void efi_main(EFI_HANDLE Ximage, EFI_SYSTEM_TABLE* Xsystab)
{
	EFI_HANDLE handles[MAX_DEVS];
        dev_info_t module_devs[NUM_BOOT_MODULES][MAX_DEVS];
        size_t dev_offsets[NUM_BOOT_MODULES];
	EFI_BLOCK_IO *blkio;
	UINTN nparts = sizeof(handles);
	EFI_STATUS status;
	EFI_DEVICE_PATH *devpath;
	EFI_BOOT_SERVICES *BS;
	EFI_CONSOLE_CONTROL_PROTOCOL *ConsoleControl = NULL;
	SIMPLE_TEXT_OUTPUT_INTERFACE *conout = NULL;

        // Basic initialization
	systab = Xsystab;
	image = Ximage;

        for(int i = 0; i < NUM_BOOT_MODULES; i++)
        {
                dev_offsets[i] = 0;
        }

        // Set up the console, so printf works.
	BS = systab->BootServices;
	status = BS->LocateProtocol(&ConsoleControlGUID, NULL,
	    (VOID **)&ConsoleControl);
	if (status == EFI_SUCCESS)
		(void)ConsoleControl->SetMode(ConsoleControl,
		    EfiConsoleControlScreenText);
	/*
	 * Reset the console and find the best text mode.
	 */
        UINTN max_dim;
        UINTN best_mode;
        UINTN cols;
        UINTN rows;
	conout = systab->ConOut;
	conout->Reset(conout, TRUE);
	max_dim = best_mode = 0;
	for (int i = 0; ; i++) {
		status = conout->QueryMode(conout, i,
		    &cols, &rows);
		if (EFI_ERROR(status))
			break;
		if (cols * rows > max_dim) {
			max_dim = cols * rows;
			best_mode = i;
		}
	}

	if (max_dim > 0)
		conout->SetMode(conout, best_mode);
	conout->EnableCursor(conout, TRUE);
	conout->ClearScreen(conout);

	printf("\n"
	       ">> FreeBSD ZFS-enabled EFI boot block\n");
	printf("   Loader path: %s\n\n", _PATH_LOADER);

	printf("   Initializing modules:");
        for(int i = 0; i < NUM_BOOT_MODULES; i++)
        {
                if (NULL != boot_modules[i])
                {
                        printf(" %s", boot_modules[i]->name);
                        boot_modules[i]->init(image, systab, BS);
                }
        }
        putchr('\n', NULL);

        // Get all the device handles
	status = systab->BootServices->LocateHandle(ByProtocol,
	    &BlockIoProtocolGUID, NULL, &nparts, handles);
	nparts /= sizeof(handles[0]);
	//printf("   Scanning %lu device handles\n", nparts);

        // Scan all partitions, probing with all modules.
	for (int i = 0; i < nparts; i++) {
                dev_info_t devinfo;

                // Figure out if we're dealing with an actual partition
		status = systab->BootServices->HandleProtocol(handles[i],
		    &DevicePathGUID, (void **)&devpath);
		if (EFI_ERROR(status)) {
                        //printf("        Not a device path protocol\n");
			continue;
                }

		while (!IsDevicePathEnd(NextDevicePathNode(devpath))) {
                        //printf("        Advancing to next device\n");
			devpath = NextDevicePathNode(devpath);
                }

		status = systab->BootServices->HandleProtocol(handles[i],
		    &BlockIoProtocolGUID, (void **)&blkio);
		if (EFI_ERROR(status)) {
                        //printf("        Not a block device\n");
			continue;
                }

		if (!blkio->Media->LogicalPartition) {
                        //printf("        Logical partition\n");
			continue;
                }

                // Setup devinfo
                devinfo.dev = blkio;
                devinfo.devpath = devpath;
                devinfo.devhandle = handles[i];
                devinfo.devdata = NULL;

                // Run through each module, see if it can load this partition
                for (int j = 0; j < NUM_BOOT_MODULES; j++ )
                {
                        if (NULL != boot_modules[j] &&
                            boot_modules[j]->probe(&devinfo))
                        {
                                // If it can, save it to the device list for
                                // that module
                                module_devs[j][dev_offsets[j]++] = devinfo;
                        }
                }
	}

        // Select a partition to boot.  We do this by trying each
        // module in order.
        for (int i = 0; i < NUM_BOOT_MODULES; i++)
        {
                if (NULL != boot_modules[i])
                {
                        //printf("   Trying to load from %lu %s partitions\n",
                        //       dev_offsets[i], boot_modules[i]->name);
                        try_load(boot_modules[i], module_devs[i],
                                 dev_offsets[i]);
                        //printf("   Failed\n");
                }
        }

        // If we get here, we're out of luck...
        panic("No bootable partitions found!");
}

void panic(const char *fmt, ...)
{
	char buf[128];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof buf, fmt, ap);
	printf("panic: %s\n", buf);
	va_end(ap);

	while (1) {}
}

int printf(const char *fmt, ...)
{
	va_list ap;
	int ret;


	va_start(ap, fmt);
	ret = __printf(fmt, putchr, 0, ap);
	va_end(ap);
	return (ret);
}

void vprintf(const char *fmt, va_list ap)
{
	__printf(fmt, putchr, 0, ap);
}

int vsnprintf(char *str, size_t sz, const char *fmt, va_list ap)
{
	struct sp_data sp;
	int ret;

	sp.sp_buf = str;
	sp.sp_len = 0;
	sp.sp_size = sz;
	ret = __printf(fmt, __sputc, &sp, ap);
	return (ret);
}

static int
__printf(const char *fmt, putc_func_t *putc, void *arg, va_list ap)
{
	char buf[(sizeof(long) * 8) + 1];
	char *nbuf;
	u_long ul;
	u_int ui;
	int lflag;
	int sflag;
	char *s;
	int pad;
	int ret;
	int c;

	nbuf = &buf[sizeof buf - 1];
	ret = 0;
	while ((c = *fmt++) != 0) {
		if (c != '%') {
			ret += putc(c, arg);
			continue;
		}
		lflag = 0;
		sflag = 0;
		pad = 0;
reswitch:	c = *fmt++;
		switch (c) {
		case '#':
			sflag = 1;
			goto reswitch;
		case '%':
			ret += putc('%', arg);
			break;
		case 'c':
			c = va_arg(ap, int);
			ret += putc(c, arg);
			break;
		case 'd':
			if (lflag == 0) {
				ui = (u_int)va_arg(ap, int);
				if (ui < (int)ui) {
					ui = -ui;
					ret += putc('-', arg);
				}
				s = __uitoa(nbuf, ui, 10);
			} else {
				ul = (u_long)va_arg(ap, long);
				if (ul < (long)ul) {
					ul = -ul;
					ret += putc('-', arg);
				}
				s = __ultoa(nbuf, ul, 10);
			}
			ret += __puts(s, putc, arg);
			break;
		case 'l':
			lflag = 1;
			goto reswitch;
		case 'o':
			if (lflag == 0) {
				ui = (u_int)va_arg(ap, u_int);
				s = __uitoa(nbuf, ui, 8);
			} else {
				ul = (u_long)va_arg(ap, u_long);
				s = __ultoa(nbuf, ul, 8);
			}
			ret += __puts(s, putc, arg);
			break;
		case 'p':
			ul = (u_long)va_arg(ap, void *);
			s = __ultoa(nbuf, ul, 16);
			ret += __puts("0x", putc, arg);
			ret += __puts(s, putc, arg);
			break;
		case 's':
			s = va_arg(ap, char *);
			ret += __puts(s, putc, arg);
			break;
		case 'u':
			if (lflag == 0) {
				ui = va_arg(ap, u_int);
				s = __uitoa(nbuf, ui, 10);
			} else {
				ul = va_arg(ap, u_long);
				s = __ultoa(nbuf, ul, 10);
			}
			ret += __puts(s, putc, arg);
			break;
		case 'x':
			if (lflag == 0) {
				ui = va_arg(ap, u_int);
				s = __uitoa(nbuf, ui, 16);
			} else {
				ul = va_arg(ap, u_long);
				s = __ultoa(nbuf, ul, 16);
			}
			if (sflag)
				ret += __puts("0x", putc, arg);
			ret += __puts(s, putc, arg);
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			pad = pad * 10 + c - '0';
			goto reswitch;
		default:
			break;
		}
	}
	return (ret);
}

static int
__sputc(char c, void *arg)
{
	struct sp_data *sp;

	sp = arg;
	if (sp->sp_len < sp->sp_size)
		sp->sp_buf[sp->sp_len++] = c;
	sp->sp_buf[sp->sp_len] = '\0';
	return (1);
}

static int
__puts(const char *s, putc_func_t *putc, void *arg)
{
	const char *p;
	int ret;

	ret = 0;
	for (p = s; *p != '\0'; p++)
		ret += putc(*p, arg);
	return (ret);
}

static char *
__uitoa(char *buf, u_int ui, int base)
{
	char *p;

	p = buf;
	*p = '\0';
	do
		*--p = digits[ui % base];
	while ((ui /= base) != 0);
	return (p);
}

static char *
__ultoa(char *buf, u_long ul, int base)
{
	char *p;

	p = buf;
	*p = '\0';
	do
		*--p = digits[ul % base];
	while ((ul /= base) != 0);
	return (p);
}
