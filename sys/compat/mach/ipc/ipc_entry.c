/*
 * Copyright 1991-1998 by Open Software Foundation, Inc. 
 *              All Rights Reserved 
 *  
 * Permission to use, copy, modify, and distribute this software and 
 * its documentation for any purpose and without fee is hereby granted, 
 * provided that the above copyright notice appears in all copies and 
 * that both the copyright notice and this permission notice appear in 
 * supporting documentation. 
 *  
 * OSF DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE 
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE. 
 *  
 * IN NO EVENT SHALL OSF BE LIABLE FOR ANY SPECIAL, INDIRECT, OR 
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN ACTION OF CONTRACT, 
 * NEGLIGENCE, OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION 
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */
/* CMU_HIST */
/*
 * Revision 2.7  91/10/09  16:08:15  af
 * 	 Revision 2.6.2.1  91/09/16  10:15:30  rpd
 * 	 	Added <ipc/ipc_hash.h>.
 * 	 	[91/09/02            rpd]
 * 
 * Revision 2.6.2.1  91/09/16  10:15:30  rpd
 * 	Added <ipc/ipc_hash.h>.
 * 	[91/09/02            rpd]
 * 
 * Revision 2.6  91/05/14  16:31:38  mrt
 * 	Correcting copyright
 * 
 * Revision 2.5  91/03/16  14:47:45  rpd
 * 	Fixed ipc_entry_grow_table to use it_entries_realloc.
 * 	[91/03/05            rpd]
 * 
 * Revision 2.4  91/02/05  17:21:17  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  15:44:19  mrt]
 * 
 * Revision 2.3  91/01/08  15:12:58  rpd
 * 	Removed MACH_IPC_GENNOS.
 * 	[90/11/08            rpd]
 * 
 * Revision 2.2  90/06/02  14:49:36  rpd
 * 	Created for new IPC.
 * 	[90/03/26  20:54:27  rpd]
 * 
 */
/* CMU_ENDHIST */
/* 
 * Mach Operating System
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 */
/*
 *	File:	ipc/ipc_entry.c
 *	Author:	Rich Draves
 *	Date:	1989
 *
 *	Primitive functions to manipulate translation entries.
 */

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/eventhandler.h>
#include <sys/file.h>
#include <sys/filedesc.h>
#include <sys/fcntl.h>
#include <sys/kernel.h>
#include <sys/syscallsubr.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/user.h>

#include <sys/mach/mach_types.h>
#include <sys/mach/kern_return.h>
#include <sys/mach/port.h>
#if 0
#include <kern/assert.h>
#endif
#include <sys/mach/message.h>
#include <vm/uma.h>
#include <sys/mach/ipc/port.h>
#include <sys/mach/ipc/ipc_entry.h>
#include <sys/mach/ipc/ipc_space.h>
#include <sys/mach/ipc/ipc_object.h>
#include <sys/mach/ipc/ipc_hash.h>
#include <sys/mach/ipc/ipc_table.h>
#include <sys/mach/ipc/ipc_port.h>
#include <sys/mach/thread.h>



static void
ipc_entry_hash_delete(
	ipc_space_t space,
	ipc_entry_t entry)
{
	mach_port_index_t idx;
	ipc_entry_t entryp;

	if ((idx = entry->ie_index) == UINT_MAX)
		return;
	if ((entry->ie_bits & (MACH_PORT_TYPE_DEAD_NAME | MACH_PORT_TYPE_SEND)) !=
		MACH_PORT_TYPE_SEND)
		return;
	if (idx >= space->is_table_size)
		return;

	entryp = space->is_table[idx];
	assert(entryp);

	if (entryp == entry) {
		space->is_table[idx] = entry->ie_link;
		entry->ie_link = NULL;
		entry->ie_index = UINT_MAX;
	} else {
		while (entryp->ie_link != NULL) {
			if (entryp->ie_link == entry) {
				entryp->ie_link = entry->ie_link;
				entry->ie_link = NULL;
				entry->ie_index = UINT_MAX;
				break;
			}
			entryp = entryp->ie_link;
		}
	}
	/* assert that it was found */
	MPASS(entry->ie_index == UINT_MAX);
}

static fo_close_t mach_port_close;
static fo_stat_t mach_port_stat;
static fo_fill_kinfo_t mach_port_fill_kinfo;

struct fileops mach_fileops  = {
	.fo_close = mach_port_close,
	.fo_stat = mach_port_stat,
	.fo_fill_kinfo = mach_port_fill_kinfo,
	.fo_flags = 0,
};

static int
mach_port_close(struct file *fp, struct thread *td __unused)
{
	ipc_entry_t entry;

	MACH_VERIFY(fp->f_data != NULL, ("expected fp->f_data != NULL - got NULL\n"));
	entry = fp->f_data;
	if (entry == NULL)
		return (0);
#ifdef INVARIANTS
	printf("closing mach_port: %d\n", entry->ie_name);
#endif	
	ipc_entry_hash_delete(entry->ie_space, entry);
	MPASS(entry->ie_link == NULL);

	if (entry->ie_object != NULL) {
		ipc_object_release(entry->ie_object);
		entry->ie_object = NULL;
	}
	free(entry, M_MACH_IPC_ENTRY);
	fp->f_data = NULL;

	return (0);
}

static int
mach_port_stat(struct file *fp __unused, struct stat *sb,
			   struct ucred *active_cred __unused, struct thread *td __unused)
{
	ipc_entry_t entry;

	bzero((caddr_t)sb, sizeof(*sb));

	entry = fp->f_data;
	if (entry->ie_bits & MACH_PORT_TYPE_PORT_SET) {
		sb->st_mode = S_IFPSET;
	} else {
		sb->st_mode = S_IFPORT;
	}
	return (0);
}

static int
mach_port_fill_kinfo(struct file *fp, struct kinfo_file *kif,
					 struct filedesc *fdp __unused)
{
	ipc_entry_t entry;

	/* assume it's a port first */
	kif->kf_type = KF_TYPE_PORT;

	if ((entry = fp->f_data) == NULL)
		return (0);
	/* What else do we want from it? */
	if (entry->ie_bits & MACH_PORT_TYPE_PORT_SET) {
		kif->kf_type = KF_TYPE_PORTSET;
	}

	return (0);
}

/*
 *	Routine:	ipc_entry_release
 *	Purpose:
 *		Drops a reference to an entry.
 *	Conditions:
 *		The space must be locked.
 */

void
ipc_entry_release(ipc_entry_t entry)
{

	fdrop(entry->ie_fp, curthread);
}

/*
 *	Routine:	ipc_entry_lookup
 *	Purpose:
 *		Searches for an entry, given its name.
 *	Conditions:
 *		The space must be active.
 */

ipc_entry_t
ipc_entry_lookup(ipc_space_t space, mach_port_name_t name)
{
	struct file *fp;
	ipc_entry_t entry;

	assert(space->is_active);

	if (curthread->td_proc->p_fd == NULL)
		return (NULL);

	if (fget(curthread, name, NULL, &fp) != 0) {
		log(LOG_DEBUG, "entry for port name: %d not found\n", name);
		return (NULL);
	}
	if (fp->f_type != DTYPE_MACH_IPC) {
		log(LOG_DEBUG, "port name: %d is not MACH\n", name);
		fdrop(fp, curthread);
		return (NULL);
	}
	entry = fp->f_data;
	fdrop(fp, curthread);
	return (entry);
}


kern_return_t
ipc_entry_copyin(ipc_space_t space, mach_port_name_t name, void **fpp, mach_msg_type_name_t disp, ipc_object_t *objectp)
{
	struct file *fp;
	kern_return_t kr;

	if (fget(curthread, name, NULL, &fp) != 0) {
		log(LOG_DEBUG, "entry for port name: %d not found\n", name);
		return (KERN_INVALID_ARGUMENT);
	}
	*fpp = fp;
	if (fp->f_type == DTYPE_MACH_IPC) {
		if ((kr = ipc_object_copyin(space, name, disp, objectp)) != KERN_SUCCESS) {
			fdrop(fp, curthread);
			return (kr);
		}
	}
	return (KERN_SUCCESS);
}

kern_return_t
ipc_entry_copyout(ipc_space_t space, void *handle, mach_msg_type_name_t msgt_name, mach_port_name_t *namep)
{
	struct file *fp = handle;
	ipc_entry_t entry;
	ipc_object_t object;
	kern_return_t kr;

	MPASS(handle != NULL);

	if (fp->f_type == DTYPE_MACH_IPC) {
		entry = fp->f_data;
		MPASS(entry != NULL);
		object = entry->ie_object;
		MPASS(object != NULL);
		kr = ipc_object_copyout(space, object, msgt_name, namep);
		/* XXX this shouldn't be curthread it should be the originating thread - which we need
		 * to be able get to through ipc_space
		 * KMM
		 */
		fdrop(fp, curthread);
	} else {
		/* maintain the reference added at ipc_entry_copyin */
		/* Are sent file O_CLOEXEC? */
		if ((kr = finstall(curthread, fp, namep, FMINALLOC, NULL)) != 0) {
			fdrop(fp, curthread);
			kr = KERN_RESOURCE_SHORTAGE;
		}
	}
	return (kr);
}

ipc_object_t
ipc_entry_handle_to_object(void *handle)
{
	struct file *fp = handle;
	ipc_entry_t entry;

	if (fp == NULL)
		return (NULL);
	if (fp->f_type != DTYPE_MACH_IPC)
		return (NULL);
	if ((entry = fp->f_data) == NULL)
		return (NULL);

	return (entry->ie_object);
}


/*
 *	Routine:	ipc_entry_get
 *	Purpose:
 *		Tries to allocate an entry out of the space.
 *	Conditions:
 *		The space is active throughout.
 *		An object may be locked.  Will try to allocate memory.
 *	Returns:
 *		KERN_SUCCESS		A free entry was found.
 *		KERN_NO_SPACE		No entry allocated.
 */

kern_return_t
ipc_entry_get(
	ipc_space_t	space,
	boolean_t	is_send_once,
	mach_port_name_t	*namep,
	ipc_entry_t	*entryp)
{	
	ipc_entry_t free_entry;
	int fd, flags;
	struct file *fp;
	struct thread *td = curthread;

	assert(space->is_active);

	flags = 0;
	if ((free_entry = malloc(sizeof(*free_entry), M_MACH_IPC_ENTRY, M_WAITOK|M_ZERO)) == NULL)
		return KERN_RESOURCE_SHORTAGE;

	if (*namep != MACH_PORT_NAME_NULL) {
		fd = *namep;
		flags = FNOFDALLOC|O_CLOEXEC;
	} else
		flags = FMINALLOC|O_CLOEXEC;

	if (falloc(td, &fp, &fd, flags)) {
		free(free_entry, M_MACH_IPC_ENTRY);
		return KERN_RESOURCE_SHORTAGE;
	}
	free_entry->ie_bits = 0;
	free_entry->ie_request = 0;
	free_entry->ie_name = fd;
	free_entry->ie_fp = fp;
	free_entry->ie_index = UINT_MAX;
	free_entry->ie_link = NULL;
	free_entry->ie_space = current_space();

	finit(fp, 0, DTYPE_MACH_IPC, free_entry, &mach_fileops);
	fdrop(fp, td);
	assert(fp->f_count == 1);
	*namep = fd;
	*entryp = free_entry;

	return KERN_SUCCESS;
}

/*
 *	Routine:	ipc_entry_alloc
 *	Purpose:
 *		Allocate an entry out of the space.
 *	Conditions:
 *		The space is not locked before, but it is write-locked after
 *		if the call is successful.  May allocate memory.
 *	Returns:
 *		KERN_SUCCESS		An entry was allocated.
 *		KERN_INVALID_TASK	The space is dead.
 *		KERN_NO_SPACE		No room for an entry in the space.
 *		KERN_RESOURCE_SHORTAGE	Couldn't allocate memory for an entry.
 */

kern_return_t
ipc_entry_alloc(
	ipc_space_t	space,
	boolean_t	is_send_once,
	mach_port_name_t	*namep,
	ipc_entry_t	*entryp)
{
	kern_return_t kr;

	if (!space->is_active)
		return (KERN_INVALID_TASK);

	*namep = MACH_PORT_NAME_NULL;
	if ((kr = ipc_entry_get(space, is_send_once, namep, entryp)) != KERN_SUCCESS)
		return (kr);

	is_write_lock(space);
	return (0);
}

/*
 *	Routine:	ipc_entry_alloc_name
 *	Purpose:
 *		Allocates/finds an entry with a specific name.
 *		If an existing entry is returned, its type will be nonzero.
 *	Conditions:
 *		The space is not locked before, but it is write-locked after
 *		if the call is successful.  May allocate memory.
 *	Returns:
 *		KERN_SUCCESS		Found existing entry with same name.
 *		KERN_SUCCESS		Allocated a new entry.
 *		KERN_INVALID_TASK	The space is dead.
 *		KERN_RESOURCE_SHORTAGE	Couldn't allocate memory.
 */

kern_return_t
ipc_entry_alloc_name(
	ipc_space_t	space,
	mach_port_name_t	name,
	ipc_entry_t	*entryp)
{
	mach_port_name_t newname;
	struct file *fp;
	kern_return_t kr;
	struct thread *td = curthread;

	assert(MACH_PORT_NAME_VALID(name));
	is_write_lock(space);
	if ((*entryp = ipc_entry_lookup(space, name)) != NULL)
		return (KERN_SUCCESS);
	is_write_unlock(space);

	/* name could technically be a ridiculously large value */
	if (kern_fdalloc(td, name, &newname))
		return (KERN_RESOURCE_SHORTAGE);

	if (newname != name) {
		kern_fddealloc(td, newname);
		return (KERN_NAME_EXISTS);
	}
	if (falloc(td, &fp, &newname, FNOFDALLOC|O_CLOEXEC)) {
		kern_fddealloc(td, newname);
		return (KERN_RESOURCE_SHORTAGE);
	}

	if (!space->is_active) {
		kern_fddealloc(td, newname);
		return (KERN_INVALID_TASK);
	}
	kr = ipc_entry_get(space, 0, &name, entryp);
	if (kr != KERN_SUCCESS) {
		kern_fddealloc(td, newname);
		return (KERN_INVALID_TASK);
	}
	is_write_lock(space);
	return (kr);
}

static void
ipc_entry_close(
	ipc_space_t space __unused,
	mach_port_name_t name)
{

	kern_close(curthread, name);
}

int
ipc_entry_refs(
	ipc_entry_t entry)
{

	return (entry->ie_fp->f_count);
}

void
ipc_entry_add_refs(
	ipc_entry_t entry,
	int delta)
{

	atomic_add_acq_int(&entry->ie_fp->f_count, delta);
}

void
ipc_entry_hold(ipc_entry_t entry)
{

	fhold(entry->ie_fp);
}

/*
 *	Routine:	ipc_entry_dealloc
 *	Purpose:
 *		Deallocates an entry from a space.
 *	Conditions:
 *		The space must be write-locked throughout.
 *		The space must be active.
 */

void
ipc_entry_dealloc(
	ipc_space_t	space,
	mach_port_name_t	name,
	ipc_entry_t	entry)
{
	assert(space->is_active);
	assert(entry->ie_object == IO_NULL);
	assert(entry->ie_request == 0);

	ipc_entry_hash_delete(space, entry);
	MPASS(entry->ie_link == NULL);

	ipc_entry_close(space, name);
}

#define NDFILE		20
#define NDSLOTSIZE	sizeof(NDSLOTTYPE)
#define	NDENTRIES	(NDSLOTSIZE * __CHAR_BIT)
#define NDSLOT(x)	((x) / NDENTRIES)
#define NDBIT(x)	((NDSLOTTYPE)1 << ((x) % NDENTRIES))
#define	NDSLOTS(x)	(((x) + NDENTRIES - 1) / NDENTRIES)

/*
 * Find the highest non-zero bit in the given bitmap, starting at 0 and
 * not exceeding size - 1. Return -1 if not found.
 */
static int
fd_last_used(struct filedesc *fdp, int size)
{
	NDSLOTTYPE *map = fdp->fd_map;
	NDSLOTTYPE mask;
	int off, minoff;

	off = NDSLOT(size);
	if (size % NDENTRIES) {
		mask = ~(~(NDSLOTTYPE)0 << (size % NDENTRIES));
		if ((mask &= map[off]) != 0)
			return (off * NDENTRIES + flsl(mask) - 1);
		--off;
	}
	for (minoff = NDSLOT(0); off >= minoff; --off)
		if (map[off] != 0)
			return (off * NDENTRIES + flsl(map[off]) - 1);
	return (-1);
}

/*
 * Function drops the filedesc lock on return.
 */
static int
ipc_entry_closefp(struct filedesc *fdp, int fd, struct file *fp, struct thread *td,
    int holdleaders)
{
	int error;

	FILEDESC_XLOCK_ASSERT(fdp);

	if (holdleaders) {
		if (td->td_proc->p_fdtol != NULL) {
			/*
			 * Ask fdfree() to sleep to ensure that all relevant
			 * process leaders can be traversed in closef().
			 */
			fdp->fd_holdleaderscount++;
		} else {
			holdleaders = 0;
		}
	}

	/*
	 * We now hold the fp reference that used to be owned by the
	 * descriptor array.  We have to unlock the FILEDESC *AFTER*
	 * knote_fdclose to prevent a race of the fd getting opened, a knote
	 * added, and deleteing a knote for the new fd.
	 */
	knote_fdclose(td, fd);

	FILEDESC_XUNLOCK(fdp);

	error = fdrop(fp, td);
	if (holdleaders) {
		FILEDESC_XLOCK(fdp);
		fdp->fd_holdleaderscount--;
		if (fdp->fd_holdleaderscount == 0 &&
		    fdp->fd_holdleaderswakeup != 0) {
			fdp->fd_holdleaderswakeup = 0;
			wakeup(&fdp->fd_holdleaderscount);
		}
		FILEDESC_XUNLOCK(fdp);
	}
	return (error);
}

/*
 * Mark a file descriptor as unused.
 */
static void
fdunused(struct filedesc *fdp, int fd)
{

	FILEDESC_XLOCK_ASSERT(fdp);

	KASSERT(fdp->fd_ofiles[fd].fde_file == NULL,
	    ("fd=%d is still in use", fd));

	fdp->fd_map[NDSLOT(fd)] &= ~NDBIT(fd);
	if (fd < fdp->fd_freefile)
		fdp->fd_freefile = fd;
	if (fd == fdp->fd_lastfile)
		fdp->fd_lastfile = fd_last_used(fdp, fd);
}

/*
 * Free a file descriptor.
 *
 * Avoid some work if fdp is about to be destroyed.
 */
static inline void
fdefree_last(struct filedescent *fde)
{

	filecaps_free(&fde->fde_caps);
}

static inline void
fdfree(struct filedesc *fdp, int fd)
{
	struct filedescent *fde;

	fde = &fdp->fd_ofiles[fd];
#ifdef CAPABILITIES
	seq_write_begin(&fde->fde_seq);
#endif
	fdefree_last(fde);
	bzero(fde, fde_change_size);
	fdunused(fdp, fd);
#ifdef CAPABILITIES
	seq_write_end(&fde->fde_seq);
#endif
}

static void
ipc_entry_exit(void *arg __unused, struct proc *p)
{
	struct filedesc *fdp;
	struct filedescent *fde;
	struct file *fp;
	struct thread *td;
	int i;

	fdp = p->p_fd;
	td = curthread;
	/* do we want to just return if the refcount is > 1 or should we
	 * bar this from happening in the first place?
	 **/
	KASSERT(fdp->fd_refcnt == 1, ("the fdtable should not be shared"));
	for (i = 0; i <= fdp->fd_lastfile; i++) {
		fde = &fdp->fd_ofiles[i];
		fp = fde->fde_file;
		if (fp != NULL && (fp->f_type == DTYPE_MACH_IPC)) {
			/* mach file pointers cannot be shared across processes
			 * or fds - but they can have arbitrarily large refcounts
			 */
			fp->f_count = 1;
			FILEDESC_XLOCK(fdp);
			fdfree(fdp, i);
			(void) ipc_entry_closefp(fdp, i, fp, td, 0);
			/* closefp() drops the FILEDESC lock. */
		}
	}
}

static void
ipc_entry_sysinit(void *arg __unused)
{

	EVENTHANDLER_REGISTER(process_exit, ipc_entry_exit, NULL, EVENTHANDLER_PRI_ANY);
}

SYSINIT(ipc_entry, SI_SUB_KLD, SI_ORDER_ANY, ipc_entry_sysinit, NULL);


#if	MACH_KDB
#include <ddb/db_output.h>
#define	printf	kdbprintf

ipc_entry_t	db_ipc_object_by_name(
			task_t		task,
			mach_port_name_t	name);


ipc_entry_t
db_ipc_object_by_name(
	task_t		task,
	mach_port_name_t	name)
{
        ipc_space_t space = task->itk_space;
        ipc_entry_t entry;
 
 
        entry = ipc_entry_lookup(space, name);
        if(entry != IE_NULL) {
                iprintf("(task 0x%x, name 0x%x) ==> object 0x%x\n",
			task, name, entry->ie_object);
                return (ipc_entry_t) entry->ie_object;
        }
        return entry;
}
#endif	/* MACH_KDB */
