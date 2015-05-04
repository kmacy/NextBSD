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
#include <sys/file.h>
#include <sys/filedesc.h>
#include <sys/fcntl.h>
#include <sys/syscallsubr.h>
#include <sys/syslog.h>

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


static fo_close_t mach_port_close;
static fo_stat_t mach_port_stat;
static fo_fill_kinfo_t mach_port_fill_kinfo;

struct fileops mach_fileops  = {
	.fo_close = mach_port_close,
	.fo_stat = mach_port_stat,
	.fo_fill_kinfo = mach_port_fill_kinfo,
	.fo_flags = DFLAG_PASSABLE
};

static int
mach_port_close(struct file *fp, struct thread *td __unused)
{
	ipc_entry_t entry;

	MACH_VERIFY(fp->f_data != NULL, ("expected fp->f_data != NULL - got NULL\n"));
	if (fp->f_data == NULL)
		return (0);
	entry = fp->f_data;
	if (entry->ie_object != NULL) {
		ipc_object_release(entry->ie_object);
		entry->ie_object = NULL;
	}
	free(entry, M_MACH_IPC_ENTRY);
	fp->f_data = NULL;

	return (0);
}

static int
mach_port_stat(struct file *fp __unused, struct stat *sb __unused,
			   struct ucred *active_cred __unused, struct thread *td __unused)
{
	printf("Now WHY are you statting a mach port?\n");
	return (0);
}

static int
mach_port_fill_kinfo(struct file *fp __unused, struct kinfo_file *kif __unused,
					 struct filedesc *fdp __unused)
{
	/* placeholder to prevent us from panicking */
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
		flags = FNOFDALLOC;
	} else
		flags = FMINALLOC;

	if (falloc(td, &fp, &fd, flags)) {
		free(free_entry, M_MACH_IPC_ENTRY);
		return KERN_RESOURCE_SHORTAGE;
	}
	free_entry->ie_bits = 0;
	free_entry->ie_request = 0;
	free_entry->ie_name = fd;
	free_entry->ie_fp = fp;
	free_entry->ie_index = UINT_MAX;

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
	if (falloc(td, &fp, &newname, FNOFDALLOC)) {
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
	mach_port_index_t idx;
	ipc_entry_t entryp;

	assert(space->is_active);
	assert(entry->ie_object == IO_NULL);
	assert(entry->ie_request == 0);

	idx = entry->ie_index;
	if (idx < space->is_table_size) {
		entryp = space->is_table[idx];
		assert(entryp);

		if (entryp == entry)
			space->is_table[idx] = entry->ie_link;
		else {
			while (entryp->ie_link != NULL) {
				if (entryp->ie_link == entry) {
					entryp->ie_link = entry->ie_link;
					break;
				}
				entryp = entryp->ie_link;
			}
		}
	}
	kern_close(curthread, name);
}

/*
 *	Routine:	ipc_entry_grow_table
 *	Purpose:
 *		Grows the table in a space.
 *	Conditions:
 *		The space must be write-locked and active before.
 *		If successful, it is also returned locked.
 *		Allocates memory.
 *	Returns:
 *		KERN_SUCCESS		Grew the table.
 *		KERN_SUCCESS		Somebody else grew the table.
 *		KERN_SUCCESS		The space died.
 *		KERN_NO_SPACE		Table has maximum size already.
 *		KERN_RESOURCE_SHORTAGE	Couldn't allocate a new table.
 */

kern_return_t
ipc_entry_grow_table(
	ipc_space_t	space,
	int		target_size)
{
	ipc_entry_num_t osize, size, nsize, psize;

	do {
		ipc_entry_t *otable, *table;
		ipc_table_size_t oits, its, nits;
		mach_port_index_t i;

		assert(space->is_active);

		if (space->is_growing) {
			/*
			 *	Somebody else is growing the table.
			 *	We just wait for them to finish.
			 */
			assert_wait((event_t) space, FALSE);
			is_write_unlock(space);
			thread_block();
			is_write_lock(space);
			return KERN_SUCCESS;
		}

		otable = space->is_table;
		
		its = space->is_table_next;
		size = its->its_size;
		
		/*
		 * Since is_table_next points to the next natural size
		 * we can identify the current size entry.
		 */
		oits = its - 1;
		osize = oits->its_size;
		
		/*
		 * If there is no target size, then the new size is simply
		 * specified by is_table_next.  If there is a target
		 * size, then search for the next entry.
		 */
		if (target_size != ITS_SIZE_NONE) {
			if (target_size <= osize) {
				is_write_unlock(space);
				return KERN_SUCCESS;
			}

			psize = osize;
			while ((psize != size) && (target_size > size)) {
				psize = size;
				its++;
				size = its->its_size;
			}
			if (psize == size) {
				is_write_unlock(space);
				return KERN_NO_SPACE;
			}
		}
		nits = its + 1;
		nsize = nits->its_size;

		if (osize == size) {
			is_write_unlock(space);
			return KERN_NO_SPACE;
		}

		assert((osize < size) && (size <= nsize));

		/*
		 *	OK, we'll attempt to grow the table.
		 *	The realloc requires that the old table
		 *	remain in existence.
		 */

		space->is_growing = TRUE;
		is_write_unlock(space);
		if (it_entries_reallocable(oits))
			table = it_entries_realloc(oits, otable, its);
		else
			table = it_entries_alloc(its);
		is_write_lock(space);
		space->is_growing = FALSE;

		/*
		 *	We need to do a wakeup on the space,
		 *	to rouse waiting threads.  We defer
		 *	this until the space is unlocked,
		 *	because we don't want them to spin.
		 */

		if (table == NULL) {
			is_write_unlock(space);
#ifdef notyet
			thread_wakeup((event_t) space);
			/* XXX cv_signal */
#endif			
			return KERN_RESOURCE_SHORTAGE;
		}

		if (!space->is_active) {
			/*
			 *	The space died while it was unlocked.
			 */

			is_write_unlock(space);
#ifdef notyet
			thread_wakeup((event_t) space);
			/* XXX cv_signal */
#endif
			it_entries_free(its, table);
			is_write_lock(space);
			return KERN_SUCCESS;
		}

		assert(space->is_table == otable);
		assert((space->is_table_next == its) ||
		       (target_size != ITS_SIZE_NONE));
		assert(space->is_table_size == osize);

		space->is_table = table;
		space->is_table_size = size;
		space->is_table_next = nits;

		/*
		 *	If we did a realloc, it remapped the data.
		 *	Otherwise we copy by hand first.  Then we have
		 *	to clear the index fields in the old part and
		 *	zero the new part.
		 */

		if (!it_entries_reallocable(oits))
			(void) memcpy((void *) table, (const void *) otable,
			      osize * sizeof(struct ipc_entry));

		(void) memset((void *) (table + osize), 0,
		      (size - osize) * sizeof(struct ipc_entry *));

		/*
		 *	Put old entries into the reverse hash table.
		 */

		for (i = 0; i < osize; i++) {
			ipc_entry_t entry = table[i];

			if (IE_BITS_TYPE(entry->ie_bits) ==
						MACH_PORT_TYPE_SEND)
				ipc_hash_insert(space, entry->ie_object,
						      i, entry);
		}
	} while ((space->is_tree_small > 0) &&
		 (((nsize - size) * sizeof(struct ipc_entry)) <
		  (space->is_tree_small * sizeof(struct ipc_tree_entry))));

	return KERN_SUCCESS;
}


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
