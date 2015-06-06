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
/*
 * MkLinux
 */
/* CMU_HIST */
/*
 * Revision 2.6.3.1  92/03/03  16:21:46  jeffreyh
 * 	Changes from TRUNK
 * 	[92/02/26  11:59:48  jeffreyh]
 * 
 * Revision 2.7  92/01/03  20:20:19  dbg
 * 	Add REGS_SEGS flavor to get and set the segment registers.
 * 	Move fp_reg.h to mach/i386.
 * 	[91/10/18            dbg]
 * 
 * Revision 2.6  91/07/31  17:52:29  dbg
 * 	Add V86 mode interrupt assist.
 * 	[91/07/30  17:09:37  dbg]
 * 
 * Revision 2.5  91/05/14  16:52:33  mrt
 * 	Correcting copyright
 * 
 * Revision 2.4  91/02/05  17:32:23  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  17:10:12  mrt]
 * 
 * Revision 2.3  91/01/08  17:33:44  rpd
 * 	Two new flavors (from 2.5):
 * 	  #define i386_FLOAT_STATE	2
 * 		Hacked in not presently used, but someday...
 * 	 #define i386_ISA_PORT_MAP_STATE	3
 * 		Used 
 * 	[90/12/20  10:23:34  rvb]
 * 
 * Revision 2.2  90/05/03  15:48:05  dbg
 * 	Remove kernel-only definitions.
 * 	[90/02/05            dbg]
 * 
 * Revision 1.3  89/03/09  20:19:59  rpd
 * 	More cleanup.
 * 
 * Revision 1.2  89/02/26  13:01:07  gm0w
 * 	Changes for cleanup.
 * 
 * 24-Feb-89  Robert Baron (rvb) at Carnegie-Mellon University
 *	May need some work.
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
 *	File:	thread_status.h
 *	Author:	Avadis Tevanian, Jr.
 *	Date:	1985
 *
 *	This file contains the structure definitions for the thread
 *	state as applied to I386 processors.
 */

#ifndef	_MACH_I386_THREAD_STATUS_H_
#define _MACH_I386_THREAD_STATUS_H_

#include <machine/mach/fp_reg.h>
#include <machine/mach/thread_state.h>
/*
 *	i386_thread_state	this is the structure that is exported
 *				to user threads for use in status/mutate
 *				calls.  This structure should never
 *				change.
 *
 *	i386_float_state	exported to use threads for access to 
 *				floating point registers. Try not to 
 *				change this one, either.
 *
 *	i386_isa_port_map_state	exported to user threads to allow
 *				selective in/out operations
 *
 * 	i386_v86_assist_state 
 *
 *	thread_syscall_state 
 */

/*
 * THREAD_STATE_FLAVOR_LIST 0
 *      these are the supported flavors
 */
#define x86_THREAD_STATE32              1
#define x86_FLOAT_STATE32               2
#define x86_EXCEPTION_STATE32           3
#define x86_THREAD_STATE64              4
#define x86_FLOAT_STATE64               5
#define x86_EXCEPTION_STATE64           6
#define x86_THREAD_STATE                7
#define x86_FLOAT_STATE                 8
#define x86_EXCEPTION_STATE             9
#define x86_DEBUG_STATE32               10
#define x86_DEBUG_STATE64               11
#define x86_DEBUG_STATE                 12
#define THREAD_STATE_NONE               13
/* 14 and 15 are used for the internal x86_SAVED_STATE flavours */
#define x86_AVX_STATE32                 16
#define x86_AVX_STATE64                 17
#define x86_AVX_STATE                   18


/*
 * This structure is used for both
 * i386_THREAD_STATE and i386_REGS_SEGS_STATE.
 */
struct i386_thread_state {
	unsigned int	gs;
	unsigned int	fs;
	unsigned int	es;
	unsigned int	ds;
	unsigned int	edi;
	unsigned int	esi;
	unsigned int	ebp;
	unsigned int	esp;
	unsigned int	ebx;
	unsigned int	edx;
	unsigned int	ecx;
	unsigned int	eax;
	unsigned int	eip;
	unsigned int	cs;
	unsigned int	efl;
	unsigned int	uesp;
	unsigned int	ss;
};
#define i386_THREAD_STATE_COUNT	\
		(sizeof (struct i386_thread_state)/sizeof(unsigned int))

/*
 * Subset of saved state stored by processor on kernel-to-kernel
 * trap.  (Used by ddb to examine state guaranteed to be present
 * on all traps into debugger.)
 */
struct i386_saved_state_from_kernel {
	unsigned int	gs;
	unsigned int	fs;
	unsigned int	es;
	unsigned int	ds;
	unsigned int	edi;
	unsigned int	esi;
	unsigned int	ebp;
	unsigned int	esp;		/* kernel esp stored by pusha -
					   we save cr2 here later */
	unsigned int	ebx;
	unsigned int	edx;
	unsigned int	ecx;
	unsigned int	eax;
	unsigned int	trapno;
	unsigned int	err;
	unsigned int	eip;
	unsigned int	cs;
	unsigned int	efl;
};

/*
 * The format in which thread state is saved by Mach on this machine.  This
 * state flavor is most efficient for exception RPC's to kernel-loaded
 * servers, because copying can be avoided:
 */
struct i386_saved_state {
	unsigned int	gs;
	unsigned int	fs;
	unsigned int	es;
	unsigned int	ds;
	unsigned int	edi;
	unsigned int	esi;
	unsigned int	ebp;
	unsigned int	esp;		/* kernel esp stored by pusha -
					   we save cr2 here later */
	unsigned int	ebx;
	unsigned int	edx;
	unsigned int	ecx;
	unsigned int	eax;
	unsigned int	trapno;
	unsigned int	err;
	unsigned int	eip;
	unsigned int	cs;
	unsigned int	efl;
	unsigned int	uesp;
	unsigned int	ss;
	struct v86_segs {
	    unsigned int v86_es;	/* virtual 8086 segment registers */
	    unsigned int v86_ds;
	    unsigned int v86_fs;
	    unsigned int v86_gs;
	} v86_segs;
#define i386_SAVED_ARGV_COUNT	7
	unsigned int	argv_status;	/* Boolean flag indicating whether or
					 * not Mach copied in the args */
	unsigned int	argv[i386_SAVED_ARGV_COUNT];
					/* The return address, and the first several
					 * function call args from the stack, for
					 * efficient syscall exceptions */
};
#define i386_SAVED_STATE_COUNT	(sizeof (struct i386_saved_state)/sizeof(unsigned int))
#define i386_REGS_SEGS_STATE_COUNT	i386_SAVED_STATE_COUNT

/*
 * Machine-independent way for servers and Mach's exception mechanism to
 * choose the most efficient state flavor for exception RPC's:
 */
#define MACHINE_THREAD_STATE		i386_SAVED_STATE
#define MACHINE_THREAD_STATE_COUNT	i386_SAVED_STATE_COUNT

/*
 * Largest state on this machine:
 * (be sure mach/machine/thread_state.h matches!)
 */
#define THREAD_MACHINE_STATE_MAX	i386_SAVED_STATE_COUNT

/* 
 * Floating point state.
 *
 * fpkind tells in what way floating point operations are supported.  
 * See the values for fp_kind in <mach/i386/fp_reg.h>.
 * 
 * If the kind is FP_NO, then calls to set the state will fail, and 
 * thread_getstatus will return garbage for the rest of the state.
 * If "initialized" is false, then the rest of the state is garbage.  
 * Clients can set "initialized" to false to force the coprocessor to 
 * be reset.
 * "exc_status" is non-zero if the thread has noticed (but not 
 * proceeded from) a coprocessor exception.  It contains the status 
 * word with the exception bits set.  The status word in "fp_status" 
 * will have the exception bits turned off.  If an exception bit in 
 * "fp_status" is turned on, then "exc_status" should be zero.  This 
 * happens when the coprocessor exception is noticed after the system 
 * has context switched to some other thread.
 * 
 * If kind is FP_387, then "state" is a i387_state.  Other kinds might
 * also use i387_state, but somebody will have to verify it (XXX).
 * Note that the registers are ordered from top-of-stack down, not
 * according to physical register number.
 */

#define FP_STATE_BYTES \
	(sizeof (struct i386_fp_save) + sizeof (struct i386_fp_regs))

struct i386_float_state {
	int		fpkind;			/* FP_NO..FP_387 (readonly) */
	int		initialized;
	unsigned char	hw_state[FP_STATE_BYTES]; /* actual "hardware" state */
	int		exc_status;		/* exception status (readonly) */
};
#define i386_FLOAT_STATE_COUNT \
		(sizeof(struct i386_float_state)/sizeof(unsigned int))


#define PORT_MAP_BITS 0x400
struct i386_isa_port_map_state {
	unsigned char	pm[PORT_MAP_BITS>>3];
};

#define i386_ISA_PORT_MAP_STATE_COUNT \
		(sizeof(struct i386_isa_port_map_state)/sizeof(unsigned int))

/*
 * V8086 assist supplies a pointer to an interrupt
 * descriptor table in task space.
 */
struct i386_v86_assist_state {
	unsigned int	int_table;	/* interrupt table address */
	int		int_count;	/* interrupt table size */
};

struct v86_interrupt_table {
	unsigned int	count;	/* count of pending interrupts */
	unsigned short	mask;	/* ignore this interrupt if true */
	unsigned short	vec;	/* vector to take */
};

#define	i386_V86_ASSIST_STATE_COUNT \
	    (sizeof(struct i386_v86_assist_state)/sizeof(unsigned int))

struct thread_syscall_state {
	unsigned eax;
	unsigned edx;
	unsigned efl;
	unsigned eip;
	unsigned esp;
};

#define i386_THREAD_SYSCALL_STATE_COUNT \
		(sizeof(struct thread_syscall_state) / sizeof(unsigned int))

#endif	/* _MACH_I386_THREAD_STATUS_H_ */
